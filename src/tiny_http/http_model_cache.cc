/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_base/md5.h>
#include <tiny_http/http_model_cache.h>

#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

static const unsigned long long MAX_DISK_CACHE_SIZE = 1024 * 1024 * 1024;

inline bool cache_comp(const disk_cache_t* a, const disk_cache_t* b)
{
    if (a->invalid_time != b->invalid_time) {
        return a->invalid_time < b->invalid_time;
    } else {
        return a->used_times < b->used_times;
    }
}

int initDiskCache(disk_cache_t* cache)
{
    if (nullptr == cache)
        return -1;

    int return_val;
    return_val = open(cache->filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (return_val < 0) {
        handle_error("create cache-file error:");
        if (errno == EACCES) {
            std::cout << "owner hasn't read permission\n";
        }
        return -2;
    }

    cache->valid = true;
    cache->fd = return_val;
    cache->size = 0;
    return 0;
}

int destroyDiskCache(disk_cache_t* cache)
{
    if (nullptr == cache)
        return -1;

    if (cache->valid == false) {
        return -1;
    }

    if (cache->fd != -1) {
        close(cache->fd);
    }

    int return_val;
    return_val = remove(cache->filename.c_str());
    if (return_val != 0) {
        LOG(Debug) << "remove file(" << cache->filename << ") field\n";
        return -1;
    }

    cache->valid = false;
    return 0;
}

int writeHeaders(disk_cache_t* cache, unsigned int code, const std::string& message)
{
    if (nullptr == cache || false == cache->valid)
        return -1;

    int return_val;

    sdstr res;
    sdsnewempty(&res, 1024);

    sdscatsprintf(&res, "HTTP/1.1 %d %s", code, message.c_str());

    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Location: %s", cache->location.c_str());

    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Access: ");
    formatHttpTime(cache->access, &res);

    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Date: ");
    formatHttpTime(cache->data, &res);

    unsigned int body_offset = res.len + 2 + 27 + 7 + 2 + 2;
    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Body-Offset: %7d", body_offset);

    sdscat(&res, "\r\n\r\n");

    std::cout << "body offset:" << body_offset << "-" << res.len << std::endl;

again:
    return_val = write(cache->fd, res.data, res.len);
    if (return_val < 0 && errno == EINTR)
        goto again;
    if (return_val < res.len)
        goto fail;

    sdsdestory(&res);
    return 0;
fail:
    sdsdestory(&res);
    return -1;
}

#if (SYSTEM_WRITEV)

int writeBody(disk_cache_t* cache, chain_t* body)
{
    if (nullptr == body || nullptr == cache || false == cache->valid)
        return 0;

    unsigned int chain_len;
    chain_t* tmp = body;
    buffer_t* buffer = nullptr;
    int return_val;

    chain_len = countChain(body);
    struct iovec* iovs = new struct iovec[chain_len];
    int index = 0;
    while (nullptr != tmp) {
        buffer = tmp->buffer;
        if (nullptr != buffer) {
            iovs[index].iov_base = buffer->begin;
            iovs[index].iov_len = buffer->used - buffer->begin;
        } else {
            iovs[index].iov_base = 0;
            iovs[index].iov_len = 0;
        }
        index++;
        tmp = tmp->next;
    }

again:
    int fd = cache->fd;
    return_val = writev(fd, iovs, chain_len);

    if (return_val < 0 && errno == EINTR) {
        goto again;
    }

    delete[] iovs;
    if (return_val < buffer->used - buffer->begin) {
        return -1;
    }
}

#else // using write() store chain.

int writeBody(disk_cache_t* cache, chain_t* body)
{
    if (nullptr == body || nullptr == cache || false == cache->valid)
        return 0;

    chain_t* tmp = body;
    buffer_t* buffer = nullptr;
    int return_val;
    int fd = cache->fd;

    while (nullptr != tmp) {
        buffer = tmp->buffer;
        if (nullptr != buffer) {
            return_val = write(fd, buffer->begin, buffer->used - buffer->begin);
            if (return_val < 0 && errno == EINTR) {
                continue;
            }
            if (return_val < buffer->used - buffer->begin) {
                return -1;
            } else if (return_val == buffer->used - buffer->begin) {
                tmp = tmp->next;
            }
        } else {
            tmp = tmp->next;
        }
    }
}

#endif

void HttpModelCache::m_fFloat(unsigned int i)
{
    unsigned int index = i;
    while (index > 1) {
        unsigned int parent = index / 2;
        if (cache_comp(m_nVector[parent], m_nVector[index]))
            break;
        m_fSwap(index, parent);
        index = parent;
    }
}

void HttpModelCache::m_fSink(unsigned int i)
{
    unsigned int index = i;
    while (2 * index <= m_nVector.size() - 1) {
        unsigned int left_child = 2 * index;
        unsigned int min_child = left_child;
        unsigned int right_child = 2 * index + 1;
        if (right_child <= m_nVector.size() - 1
            && cache_comp(m_nVector[right_child], m_nVector[left_child]))
            min_child = right_child;

        if (!cache_comp(m_nVector[index], m_nVector[min_child])) {
            m_fSwap(index, min_child);
            index = min_child;
        } else
            break;
    }
}

inline void HttpModelCache::m_fSwap(unsigned int parent, unsigned int child)
{
    disk_cache_t* tmp = m_nVector[parent];
    m_nVector[parent] = m_nVector[child];
    m_nVector[child] = tmp;

    m_nVector[parent]->index = parent;
    m_nVector[child]->index = child;
}

void HttpModelCache::m_fPush(disk_cache_t* cache)
{
    // cache->index = m_nVector.size() - 1;
    m_nVector.push_back(cache);
    m_nMaps[cache->filename] = cache;
    m_fFloat(cache->index);
}

disk_cache_t* HttpModelCache::m_fTop()
{
    if (m_nVector.size() == 1) {
        return nullptr;
    }
    return m_nVector[1];
}

void HttpModelCache::m_fPop()
{
    auto size = m_nVector.size();
    if (size == 1)
        return;
    if (size > 2)
        m_nVector[1] = *(m_nVector.end() - 1);
    m_nVector.erase(m_nVector.end() - 1);
    if (size > 2)
        m_fSink(1);
}

int HttpModelCache::m_fChangeNode(const std::string& filename, unsigned int add_used)
{
    if (m_nMaps.end() == m_nMaps.find(filename)) {
        return -1;
    }

    disk_cache_t* cache = m_nMaps[filename];
    if (nullptr == cache) {
        return -1;
    }
    cache->used_times += add_used;
    m_fSink(cache->index);
    return 0;
}

int HttpModelCache::m_fCheckSpace(unsigned int new_size)
{
    unsigned all_size = m_nDiskCacheSize + new_size;

    while (all_size > MAX_DISK_CACHE_SIZE) {
        if (m_nVector.size() == 1) {
            return -1;
            break;
        }

        disk_cache_t* old = nullptr;
        old = m_fTop();
        if (nullptr != old) {
            m_fPop();
            all_size -= old->size;
        } else {
            return -1;
            break;
        }
    }

    m_nDiskCacheSize = all_size - new_size;

    std::cout << "max-disk-space:" << MAX_DISK_CACHE_SIZE
              << ",max-long-long:" << LLONG_MAX
              << ",m_nDiskCacheSize:" << m_nDiskCacheSize
              << ", new_size:" << new_size << std::endl;
    return 0;
}

HttpModelCache::HttpModelCache()
{
    m_nDiskCacheRoot = "";
    m_nDiskCacheSize = 0;

    disk_cache_t* min = new disk_cache_t;
    min->invalid_time = INT_MAX;
    min->used_times = INT_MAX;
    min->index = 0;
    m_nVector.push_back(min);

    LOG(Debug) << "class HttpModelCache constructor\n";
}

void HttpModelCache::loadDiskCache()
{
    // load cache form disk_cache_root.
    // init disk_cache_t.(restart TinyWeb)
}

int HttpModelCache::createDiskCache(const std::string& url)
{
    unsigned int size = 1000;
    int return_val;
    return_val = m_fCheckSpace(size);
    if (0 != return_val) {
        return -1;
    }

    disk_cache_t* cache = new disk_cache_t;
    if (nullptr == cache) {
        return -1;
    }

    std::string filename;
    char* filename_tmp = new char[32];
    md5(url.c_str(), url.size(), filename_tmp);
    formatMd5Filename(filename_tmp, 32, filename);
    delete[] filename_tmp;

    cache->location = url;
    cache->filename = filename;
    cache->size = 1;
    cache->access = 10;
    cache->data = 10;
    cache->invalid_time = rand() % 10;
    cache->used_times = 0;
    cache->index = m_nVector.size();
    initDiskCache(cache);

    m_fPush(cache);
    m_nDiskCacheSize += size;
}

int HttpModelCache::writeDiskCacheHeaders(const std::string& url, unsigned int code, const std::string& message)
{
    disk_cache_t* cache = nullptr;
    std::string filename;

    char* filename_tmp = new char[32];
    md5(url.c_str(), url.size(), filename_tmp);
    formatMd5Filename(filename_tmp, 32, filename);
    delete[] filename_tmp;

    if (m_nMaps.end() == m_nMaps.find(filename)) {
        return -1;
    }

    cache = m_nMaps[filename];
    writeHeaders(cache, code, message);
}

int HttpModelCache::writeDiskCacheBody(const std::string& url, chain_t* body)
{
    disk_cache_t* cache = nullptr;
    std::string filename;

    char* filename_tmp = new char[32];
    md5(url.c_str(), url.size(), filename_tmp);
    formatMd5Filename(filename_tmp, 32, filename);
    delete[] filename_tmp;

    if (m_nMaps.end() == m_nMaps.find(filename)) {
        return -1;
    }

    cache = m_nMaps[filename];
    writeBody(cache, body);
}

bool HttpModelCache::haveDiskCache(const std::string& url)
{
    std::string filename;

    char* filename_tmp = new char[32];
    md5(url.c_str(), url.size(), filename_tmp);
    formatMd5Filename(filename_tmp, 32, filename);
    delete[] filename_tmp;

    LOG(Debug) << "location:(" << url << "),filename:(" << filename << ")\n";

    if (m_nMaps.end() == m_nMaps.find(filename)) {
        return false;
    } else {
        return true;
    }
}

void HttpModelCache::getDiskCache(const std::string& url, chain_t* chain)
{
    disk_cache_t* cache = nullptr;
    std::string filename;

    char* filename_tmp = new char[32];
    md5(url.c_str(), url.size(), filename_tmp);
    formatMd5Filename(filename_tmp, 32, filename);
    delete[] filename_tmp;

    m_fChangeNode(filename, 1);

    if (nullptr == chain) {
        return;
    }

    if (m_nMaps.end() == m_nMaps.find(filename)) {
        return;
    }

    cache = m_nMaps[filename];
    if (nullptr == cache) {
        return;
    }
    // TODO: fill chain with cache.
}

HttpModelCache::~HttpModelCache()
{
    for (auto t : m_nVector) {
        delete t;
    }

    LOG(Debug) << "class HttpModelCache destructor\n";
}
