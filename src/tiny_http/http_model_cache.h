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
#include <cstring>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>


typedef struct disk_cache_t {
    // char* location; // url location.
    // char filename[32]; // filename for disk cache.
    std::string location;
    std::string filename;
    bool valid;
    int fd;

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

struct str_comp {
    bool operator()(char* a, char* b)
    {
        return std::strcmp(a, b) < 0;
    }
};

inline bool cache_comp(const disk_cache_t* a, const disk_cache_t* b);
int initDiskCache(disk_cache_t* cache);
int destroyDiskCache(disk_cache_t* cache);
int writeHeaders(disk_cache_t* cache, unsigned int code, const std::string& message);
int writeBody(disk_cache_t* cache, chain_t* body);

inline void formatMd5Filename(const char* md5filename, unsigned int len, std::string& res)
{
    std::string dickCacheRoot = "/home/tinyweb/src/mytest/build";

    res = dickCacheRoot;

    if ('/' == res[res.size() - 1]) {
        res.erase(res.end() - 1);
    }

    std::vector<int> grades = { 1, 2 };
    unsigned int index = 0;
    for (int i = 0; i < grades.size(); i++) {
        res += '/';
        for (int j = 0; j < grades[i]; j++) {
            res += *(md5filename + index);
            index++;
        }
    }

    if (index < len) {
        res += '/';
        res.append(md5filename + index, len - index);
    }
}

class HttpModelCache {

private:
    std::string m_nDiskCacheRoot;
    unsigned long long m_nDiskCacheSize;
    std::vector<disk_cache_t*> m_nVector;
    // std::unordered_map<char*, disk_cache_t*, str_comp> m_nMaps;
    std::unordered_map<std::string, disk_cache_t*> m_nMaps;

    void m_fFloat(unsigned int i);
    void m_fSink(unsigned int i);
    inline void m_fSwap(unsigned int parent, unsigned int child);

    void m_fPush(disk_cache_t* cache);
    disk_cache_t* m_fTop();
    void m_fPop();
    int m_fChangeNode(const std::string& filename, unsigned int add_used);
    int m_fCheckSpace(unsigned int new_size);

public:
    HttpModelCache();

    void debug()
    {
        for (auto t : m_nVector) {
            LOG(Debug) << "location:" << t->location
                      << ",filename:" << t->filename
                      << ",used_times:" << t->used_times
                      << ",index:" << t->index << std::endl;
        }
    }

    void loadDiskCache();

    int createDiskCache(const std::string& url);
    int writeDiskCacheHeaders(const std::string& url, unsigned int code, const std::string& message);
    int writeDiskCacheBody(const std::string& url, chain_t* body);

    bool haveDiskCache(const std::string& url);
    void getDiskCache(const std::string& url, chain_t* chain);

    ~HttpModelCache();
};

#endif // !HTTP_MODEL_CACHE_H
