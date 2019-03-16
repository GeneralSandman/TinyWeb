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
    std::cout << std::endl;
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

    chunked_t chunk;
    chain_t* output;
    chunked_init(&chunk, &pool);
    while(!file.noMoreData()) {
        file.getData(input);

        std::cout << countAllDataSize(input) << std::endl;
        print_chain(input);

        bool endData = file.noMoreData();

        output = chunked_chain(&chunk, input);
        print_chain(output);
        std::cout << countAllDataSize(output) << std::endl;

        clearData(input);
    }
}

int main()
{
    test1();

    return 0;
}
