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

#ifndef HTTP_MODEL_CACHE_H
#define HTTP_MODEL_CACHE_H

#include <tiny_base/memorypool.h>
#include <tiny_http/http_time.h>
#include <tiny_struct/sdstr_t.h>

#include <climits>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#define MAX_DISK_CACHE_SIZE (50 * 1024 * 1024 * 1024)

typedef struct disk_cache_t {
    char* location; // url location.
    char* filename; // filename for disk cache.

    chain_t* source;

    unsigned int body_offset; // body offset in cache file.
    unsigned int body_length; // body length in cache file.

    unsigned int size; // header_length + body_length;

    time_t age;
    time_t access;
    time_t data;
    time_t last_modified;
    time_t expires;

    time_t invalid_time;
    unsigned int used_times; // for client.

    unsigned int index; // index in cache heap.
} disk_cache_t;

inline bool cache_comp(const disk_cache_t* a, const disk_cache_t* b)
{
    if (a->invalid_time != b->invalid_time) {
        return a->invalid_time < b->invalid_time;
    } else {
        return a->used_times < b->used_times;
    }
}

int writeHeaders(disk_cache_t* cache, unsigned int code, const std::string& message);
int writeBody(disk_cache_t* cache, chain_t* body);

class CacheHeap {

private:
    std::vector<disk_cache_t*> m_nVector;
    std::unordered_map<std::string, disk_cache_t*> maps;

    void m_fFloat(int i);
    void m_fSink(int i);
    void m_fSwap(int parent, int child);

public:
    CacheHeap();
    void push(disk_cache_t* file);
    disk_cache_t* top();
    void pop();
    int changeNode(const std::string& file, unsigned int add_used);
    ~CacheHeap();
};

class HttpModelCache {

private:
    CacheHeap heap;
    unsigned long long all_cache_size;

public:
    HttpModelCache();

    void push(disk_cache_t* cache);
    disk_cache_t* remove();
    void addUsedTime(const std::string& file);

    ~HttpModelCache();
};

#endif // !HTTP_MODEL_CACHE_H
