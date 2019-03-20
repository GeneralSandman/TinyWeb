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


#include <tiny_base/memorypool.h>
#include <tiny_base/file.h>
#include <tiny_http/http_model_chunked.h>


#include <iostream>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <zlib.h>

using namespace std;

void print_chain(chain_t* chain)
{
    std::cout << "chain-status(";
    while(nullptr != chain) {
        buffer_t* buffer = chain->buffer;

        if(buffer->islast)
            std::cout << "-";
        else
        {
            std::cout << "+";
        }
        
        chain = chain->next;
    }
    std::cout << ")\n";
}

int gzip_out(chain_t* chain,
    const std::string& outputfile)
{
    buffer_t* buffer;
    unsigned int size;
    int outputfd;

    chain_t* in_chain;
    buffer_t* in_buffer;

    //open output file
    outputfd = open(outputfile.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
    if (-1 == outputfd) {
        printf("create input-file(%s) error\n", outputfile.c_str());
        return -1;
    }

    // LOG(Debug) << "out size:" << countChain(chain) << std::endl;
    // for (auto t = context->out; t != nullptr; t = t->next) {
    // if (t->buffer->islast) {
    // std::cout << "-";
    // } else {
    // std::cout << "+";
    // }
    // }
    while (chain) {
        buffer = chain->buffer;
        size = buffer->used - buffer->begin;

        if (!size) {
            break;
        }

        write(outputfd, (char*)buffer->begin, size);
        // printf("comperss-data[%p,%u](%.*s)\n", buffer->begin, size, size, buffer->begin);

        if (buffer->islast) {
            break;
        }
        chain = chain->next;
    }

    close(outputfd);
    return 0;
}

void test1()
{
    std::string inputfile;
    std::cout << "input file:";
    std::cin >> inputfile;

    std::string chunked_file;
    chunked_file = inputfile + ".chunked.tmp";

    MemoryPool pool;

    //get and add data
    unsigned int chain_size = 10;
    unsigned int buffer_size = 4 * 1024;
    chain_t* input = pool.getNewChain(chain_size);
    if (input == nullptr) {
        std::cout << "getNewChain failed" << std::endl;
    }
    if (!pool.mallocSpace(input, buffer_size)) {
        std::cout << "mallocSpace failed" << std::endl;
    }

    File file;
    file.setFile(inputfile);

    HttpModelChunk chunk(&pool);
    chain_t* output;

    unsigned int file_size;
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;

    while(true) {
        bool tail = file.noMoreData();

        void* address = chunk.chunked_begin(input);
        a = countAllDataSize(input);

        file.getData(input);
        b = countAllDataSize(input);

        chunk.chunked_end(input, address, b - a);
        c = countAllDataSize(input);

        gzip_out(input, chunked_file);

        clearData(input);

        std::cout << "begin-chunked:" << a << ",get-data:" << b - a << ",end-chunked:" << c << std::endl;
        if (a == 18 && c - b ==2) {
            std::cout << "pass test\n";
        }

        std::cout << "-------\n";
        if (tail)
            break;
    }
}

int main()
{
    test1();

    return 0;
}
