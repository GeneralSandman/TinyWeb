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

#include <tiny_base/configer.h>
#include <tiny_base/file.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http_model_file.h>

#include <iostream>

using namespace std;

void file_test1()
{
    int return_val;
    MemoryPool pool;
    std::string fileName = "../memorypool_test.cc";
    File* file = new File();

    return_val = file->setFile(fileName);
    if (return_val != 0) {
        std::cout << "setFile error,file(" << fileName << ")\n";
        return;
    }

    unsigned int file_size = file->getFileSize();
    std::cout << "file(" << fileName << "),size(" << file->getFileSize() << ")\n";

    unsigned int buffer_size = 1024 * 4;
    unsigned int buffer_num = file_size / buffer_size;
    if (file_size % buffer_size) {
        buffer_num += 1;
    }

    chain_t* chain = nullptr;
    chain = pool.getNewChain(buffer_num);
    pool.mallocSpace(chain, buffer_size);

    file->getData(chain);

    unsigned int all_buffer_size = 0;
    chain_t* tmp = chain;
    buffer_t* buffer;
    while (tmp != nullptr) {
        buffer = tmp->buffer;
        all_buffer_size += (buffer->used - buffer->begin);
        tmp = tmp->next;
    }

    std::cout << "all buffer size:" << all_buffer_size << std::endl;

    delete file;
}

void file_test2()
{
}

void http_file_test1()
{
}

int main()
{
    file_test1();
    // file_test2();
    // http_file_test1();

    return 0;
}
