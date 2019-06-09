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

#include <tiny_base/file.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http_model_cache.h>

void test1()
{
    HttpModelCache cache;

    disk_cache_t* cache1 = new disk_cache_t;
    disk_cache_t* cache2 = new disk_cache_t;
    disk_cache_t* cache3 = new disk_cache_t;
    disk_cache_t* cache4 = new disk_cache_t;
    disk_cache_t* cache5 = new disk_cache_t;
    disk_cache_t* cache6 = new disk_cache_t;
    disk_cache_t* cache7 = new disk_cache_t;
    disk_cache_t* cache8 = new disk_cache_t;
    disk_cache_t* cache9 = new disk_cache_t;
    disk_cache_t* cache10 = new disk_cache_t;

    cache1->location = "http://dissigil.cn/1.html";
    cache2->location = "http://dissigil.cn/2.html";
    cache3->location = "http://dissigil.cn/3.html";
    cache4->location = "http://dissigil.cn/4.html";
    cache5->location = "http://dissigil.cn/5.html";
    cache6->location = "http://dissigil.cn/6.html";
    cache7->location = "http://dissigil.cn/7.html";
    cache8->location = "http://dissigil.cn/8.html";
    cache9->location = "http://dissigil.cn/9.html";
    cache10->location = "http://dissigil.cn/10.html";

    delete cache1;
    delete cache2;
    delete cache3;
    delete cache4;
    delete cache5;
    delete cache6;
    delete cache7;
    delete cache8;
    delete cache9;
    delete cache10;
}

void test2()
{
    MemoryPool pool;
    HttpModelCache cache;

    std::string url = "/home/tinyweb/www/1-63k_files/1k.txt";
    chain_t* input = nullptr;
    chain_t* header = nullptr;
    chain_t* output = nullptr;

    // malloc space for input chain.
    input = pool.getNewChain(20);
    pool.mallocSpace(input, 4 * 1024);
    header = pool.getNewChain(20);
    pool.mallocSpace(header, 1 * 1024);

    std::cout << "input all buffer size:" << countAllBufferSize(input) << std::endl
              << "input all data size:" << countAllDataSize(input) << std::endl;
    std::cout << "header all buffer size:" << countAllBufferSize(header) << std::endl
              << "header all data size:" << countAllDataSize(header) << std::endl;

    // read data from file.
    std::string input_file_name = "/home/tinyweb/www/1-63k_files/1k.txt";
    File input_file;
    input_file.setFile(input_file_name);
    std::cout << "input file (" << input_file_name << ") size"
              << input_file.getFileSize() << std::endl;
    input_file.getData(input);
    std::cout << "input all buffer size:" << countAllBufferSize(input) << std::endl
              << "input all data size:" << countAllDataSize(input) << std::endl;

    // init header-chain.
    std::string header_str = "HTTP/1.1 200 OK\r\n"
                             "Server: TinyWeb/1.1\r\n"
                             "\r\n";
    appendData(header, header_str.c_str(), header_str.size());

    // Test HttpModelCache.
    std::string message = "OK";
    
    if (!cache.haveDiskCache(url)) {
        std::cout << "haven't disk cache\n";
        cache.createDiskCache(url);
        cache.writeDiskCacheHeaders(url, 200, message);
        cache.writeDiskCacheBody(url, input);
    } else {
        std::cout << "have disk cache\n";
    }

    cache.debug();
    std::cout << cache.haveDiskCache(url) << std::endl;
    cache.getDiskCache(url, output);
    std::cout << "output all buffer size:" << countAllBufferSize(output) << std::endl
              << "output all data size:" << countAllDataSize(output) << std::endl;

    cache.debug();
}

int main()
{
    // test1();
    test2();
    return 0;
}
