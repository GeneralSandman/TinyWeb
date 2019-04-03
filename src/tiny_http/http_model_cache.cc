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
    cache_t* tmp = m_nVector[parent];
    m_nVector[parent] = m_nVector[child];
    m_nVector[child] = tmp;

    m_nVector[parent]->index = parent;
    m_nVector[child]->index = child;
}

CacheHeap::CacheHeap()
{
    LOG(Debug) << "class CacheHeap constructor\n";
    cache_t* min = new cache_t;
    min->invalid_time = INT_MAX;
    min->used_times = INT_MAX;
    min->index = 0;
    m_nVector.push_back(min);
}

void CacheHeap::push(cache_t* file)
{
    file->index = m_nVector.size() - 1;
    m_nVector.push_back(file);
    maps[file->file_name] = file;
    m_fFloat(m_nVector.size() - 1);
}

cache_t* CacheHeap::top()
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

    cache_t* cache = maps[file];
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

HttpModelCache::~HttpModelCache()
{
    LOG(Debug) << "class HttpModelCache destructor\n";
}

