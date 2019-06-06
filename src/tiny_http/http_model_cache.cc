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

#include <tiny_base/log.h>
#include <tiny_http/http_model_cache.h>

int writeHeaders(disk_cache_t* cache, unsigned int code, const std::string& message)
{
    if (nullptr == cache)
        return -1;

    int return_val;

    sdstr res;
    sdsnewempty(&res, 1024);

    sdscatsprintf(&res, "HTTP/1.1 %3d %s", code, message);

    char* location = "http://dissigil.cn/ss.html";
    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Location: %s", location);

    time_t access = 0;
    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Access: ");
    formatHttpTime(access, &res);

    time_t data = 0;
    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Date: ");
    formatHttpTime(data, &res);

    unsigned int body_offset = res.len + 2 + 27 + 7 + 2 + 2;
    sdscatsprintf(&res, "\r\nTinyWeb-Cache-Body-Offset: %7d", body_offset);

    sdscat(&res, "\r\n\r\n");

    std::cout << "body offset:" << body_offset << "-" << res.len << std::endl;

    int fd = 1;
again:
    return_val = write(fd, res.data, res.len);
    if (return_val < 0 && errno == EINTR)
        goto again;
    if (return_val < res.len)
        goto fail;

    // TODO:copy to chain.
    // invoke after-callback write to file.
    sdsdestory(&res);
    return 0;
fail:
    sdsdestory(&res);
    return -1;
}

#if (SYSTEM_WRITEV)

int writeBody(disk_cache_t* cache, chain_t* body)
{
    if (nullptr == body)
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

    int fd = 1;
    return_val = writev(fd, iovs, chain_len);

    if (return_val < 0 && errno == EINTR) {
        go again;
        continue;
    }
    if (return_val < buffer->used - buffer->begin) {
        return -1;
    }
}

#else // using write() store chain.

int writeBody(disk_cache_t* cache, chain_t* body)
{
    if (nullptr == body)
        return 0;

    chain_t* tmp = body;
    buffer_t* buffer = nullptr;
    int return_val;
    int fd = 1;

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

void CacheHeap::m_fFloat(int i)
{
    int index = i;
    while (index > 1) {
        int parent = index / 2;
        if (cache_comp(m_nVector[parent], m_nVector[index]))
            break;
        m_fSwap(index, parent);
        index = parent;
    }
}

void CacheHeap::m_fSink(int i)
{
    int index = i;
    while (2 * index <= m_nVector.size() - 1) {
        int left_child = 2 * index;
        int min_child = left_child;
        int right_child = 2 * index + 1;
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

void CacheHeap::m_fSwap(int parent, int child)
{
    disk_cache_t* tmp = m_nVector[parent];
    m_nVector[parent] = m_nVector[child];
    m_nVector[child] = tmp;

    m_nVector[parent]->index = parent;
    m_nVector[child]->index = child;
}

CacheHeap::CacheHeap()
{
    LOG(Debug) << "class CacheHeap constructor\n";
    disk_cache_t* min = new disk_cache_t;
    min->invalid_time = INT_MAX;
    min->used_times = INT_MAX;
    min->index = 0;
    m_nVector.push_back(min);
}

void CacheHeap::push(disk_cache_t* cache)
{
    cache->index = m_nVector.size() - 1;
    m_nVector.push_back(cache);
    maps[cache->location] = cache;
    m_fFloat(cache->index);
}

disk_cache_t* CacheHeap::top()
{
    if (m_nVector.size() == 1) {
        return nullptr;
    }
    return m_nVector[1];
}

void CacheHeap::pop()
{
    if (m_nVector.size() == 1)
        return;
    m_nVector[1] = *(m_nVector.end() - 1);
    m_nVector.erase(m_nVector.end() - 1);
    if (m_nVector.size() > 1)
        m_fSink(1);
}

int CacheHeap::changeNode(const std::string& file, unsigned int add_used)
{
    auto p = maps.find(file);
    if (maps.end() == p) {
        return -1;
    }

    disk_cache_t* cache = maps[file];
    cache->used_times += add_used;
    unsigned int index = cache->index;
    m_fSink(index);
    return 0;
}

CacheHeap::~CacheHeap()
{
    delete m_nVector[0];
    LOG(Debug) << "class CacheHeap destructor\n";
}

HttpModelCache::HttpModelCache()
    : all_cache_size(0)
{
    LOG(Debug) << "class HttpModelCache constructor\n";
}

void HttpModelCache::push(disk_cache_t* cache)
{
    if (nullptr == cache) {
        return;
    }
    heap.push(cache);
    all_cache_size += cache->size;
}

disk_cache_t* HttpModelCache::remove()
{
    disk_cache_t* result = nullptr;
    result = heap.top();
    heap.pop();
    if (nullptr != result)
        all_cache_size -= result->size;
    return result;
}

void HttpModelCache::addUsedTime(const std::string& file)
{
    heap.changeNode(file, 1);
}

HttpModelCache::~HttpModelCache()
{
    LOG(Debug) << "class HttpModelCache destructor\n";
}
