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
#include <tiny_base/memorypool.h>
#include <tiny_http/http_model_gzip.h>

#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

void test1()
{
    std::string inputfile;
    std::cout << "inputfile=";
    std::cin >> inputfile;
    //open input file
    int inputfd = open(inputfile.c_str(), O_RDONLY);
    if (-1 == inputfd)
    {
        printf("open input-file(%s) error\n", inputfile.c_str());
        return; 
    }

    char* read_buf = (char *)malloc(1024);
    int read_len = read(inputfd, read_buf, 1024);

    {
        MemoryPool pool;
        gzip_context_t context;
        gzip_status res;
        std::string outfile;
        std::cout << "outfile=";
        std::cin >> outfile;

        std::string data = "123456789";
        res = gzip_body(&pool, &context, read_buf, read_len);
        if (gzip_error == res) {
            std::cout << "gzip data error" << std::endl;
        }
        gzip_out(&context, outfile);
    }
}

int main()
{
    test1();

    return 0;
}
