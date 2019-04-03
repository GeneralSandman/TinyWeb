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

#include <tiny_struct/sdstr_t.h>

#include <climits>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#define MAX_DISK_CACHE_SIZE (50 * 1024 * 1024 * 1024)

typedef struct cache_t {
    std::string file_name;
    time_t invalid_time;
    unsigned int used_times;
    struct stat file_info;

    unsigned int index;
} cache_t;

inline bool cache_comp(const cache_t* a, const cache_t* b)
{
    if (a->invalid_time != b->invalid_time) {
        return a->invalid_time < b->invalid_time;
    } else {
        return a->used_times < b->used_times;
    }
}

class CacheHeap {

private:
    std::vector<cache_t*> m_nVector;
    std::unordered_map<std::string, cache_t*> maps;

    void m_fFloat(int i);
    void m_fSink(int i);
    void m_fSwap(int parent, int child);

public:
    CacheHeap();
    void push(cache_t* file);
    cache_t* top();
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
    void push(cache_t* cache)
    {
        if (nullptr == cache) {
            return;
        }
        heap.push(cache);
        all_cache_size += cache->file_info.st_size;
    }

    cache_t* remove()
    {
        cache_t* result = nullptr;
        result = heap.top();
        heap.pop();
        if (nullptr != result)
            all_cache_size -= result->file_info.st_size;

        return result;
    }

    void addUsedTime(const std::string& file)
    {
        heap.changeNode(file, 1);
    }

    void test()
    {
        cache_t* a1 = new cache_t;
        cache_t* a2 = new cache_t;
        cache_t* a3 = new cache_t;
        cache_t* a4 = new cache_t;
        cache_t* a5 = new cache_t;
        cache_t* a6 = new cache_t;
        cache_t* a7 = new cache_t;
        cache_t* a8 = new cache_t;
        cache_t* a9 = new cache_t;
        cache_t* a10 = new cache_t;

        a1->file_name = "a1";
        a1->invalid_time = 1;
        a1->used_times = 1;
        push(a1);

        a2->file_name = "a2";
        a2->invalid_time = 2;
        a2->used_times = 1;
        push(a2);

        a3->file_name = "a3";
        a3->invalid_time = 3;
        a3->used_times = 1;
        push(a3);

        a4->file_name = "a4";
        a4->invalid_time = 5;
        a4->used_times = 4;
        push(a4);

        a5->file_name = "a5";
        a5->invalid_time = 5;
        a5->used_times = 5;
        push(a5);

        a6->file_name = "a6";
        a6->invalid_time = 5;
        a6->used_times = 3;
        push(a6);

        a7->file_name = "a7";
        a7->invalid_time = 6;
        a7->used_times = 10;
        push(a7);

        a8->file_name = "a8";
        a8->invalid_time = 5;
        a8->used_times = 10;
        push(a8);

        a9->file_name = "a9";
        a9->invalid_time = 6;
        a9->used_times = 100;
        push(a9);

        a10->file_name = "a10";
        a10->invalid_time = 7;
        a10->used_times = 99;
        push(a10);

        addUsedTime("a6");
        addUsedTime("a6");
        addUsedTime("a6");

        for (auto i = 0; i < 10; i++) {
            cache_t* file = remove();
            if (nullptr == file) {
                break;
            }
            std::cout << file->file_name << ":"
                      << file->invalid_time << "-" << file->used_times << std::endl;
        }
    }
    ~HttpModelCache();
};

#endif // !MEMORY_POOL_H
