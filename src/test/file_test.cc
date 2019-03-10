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

bool file_test1(const std::string& fileName)
{
    int return_val;
    MemoryPool pool;
    File* file = new File();

    return_val = file->setFile(fileName);
    if (return_val != 0) {
        std::cout << "setFile error,file(" << fileName << ")\n";
        return false;
    }

    unsigned int file_size = file->getFileSize();
    unsigned int buffer_size = 1024 * 4;
    unsigned int buffer_num = 10;

    chain_t* chain = nullptr;
    chain = pool.getNewChain(buffer_num);
    pool.mallocSpace(chain, buffer_size);

    unsigned int all_buffer_size = 0;
    unsigned int all_data_size = 0;
    unsigned int get_times = 0;
    while (!file->noMoreData()) {
        unsigned int chain_size = 0;
        unsigned int data_size = 0;
        clearData(chain);
        file->getData(chain);
        chain_size = countAllBufferSize(chain);
        data_size = countAllNoDealSize(chain);
        all_buffer_size += chain_size;
        all_data_size += data_size;
        get_times += 1;
    }

    std::cout << "file(" << fileName << "),"
              << "size(" << file->getFileSize() << "),"
              << "all-buffer-size(" << all_buffer_size << "),"
              << "all-data-size(" << all_data_size << "),"
              << "times-of-get-data:" << get_times << "),\n";

    delete file;

    bool same_times = false;
    unsigned int target_get_times = 0;
    target_get_times = file_size / (buffer_num * buffer_size);
    if (file_size % (buffer_num * buffer_size))
        target_get_times++;

    return (file_size == all_buffer_size
        && file_size == all_data_size
        && target_get_times == get_times);
}

void file_test2()
{
}

void files_test1()
{
    std::string begin = "../../../www/1-63k_files/";
    std::string end = "k.txt";

    unsigned int all_test = 0;
    unsigned int pass_test = 0;
    for (int i = 1; i <= 7412; i++) {
        all_test++;
        std::string fileName = begin + std::to_string(i) + end;
        if (file_test1(fileName)) {
            pass_test++;
        }
    }

    std::cout << "class File test:pass/all=" << pass_test << "/" << all_test << std::endl;
}

void http_file_test1()
{
}

int main()
{
    // file_test1();
    // file_test2();
    files_test1();
    // http_file_test1();

    return 0;
}
