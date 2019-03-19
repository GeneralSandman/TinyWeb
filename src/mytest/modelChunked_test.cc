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

void test1()
{
    std::string inputfile;
    std::cout << "input file:";
    std::cin >> inputfile;

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
    unsigned int before_chunked = 0;
    unsigned int after_chunked = 0;

    while(true) {
        bool tail = file.noMoreData();

        file.getData(input);
        print_chain(input);

        before_chunked = countAllDataSize(input);

        chunk.chunkedChain(input, output, tail);
        print_chain(output);

        after_chunked = countAllDataSize(output);
        
        
        clearData(input);
        clearData(output);

        std::cout << "before-chunked:" << before_chunked << ",after-chunked:" << after_chunked << std::endl;

        if (tail)
            break;
    }
}

int main()
{
    test1();

    return 0;
}
