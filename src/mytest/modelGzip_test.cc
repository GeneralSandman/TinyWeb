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
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_base/file.h>
#include <tiny_http/http_model_gzip.h>

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

int diff(const std::string& a, const std::string& b)
{
    std::string command = "diff " + a + " " + b;
    int ret = system(command.c_str());
    return ret;
}

int diffgzip(const std::string& a, const std::string& b)
{
    std::string gzip_name = b + ".gz";
    std::string command = "gunzip " + gzip_name + " && diff " + a + " " + b;
    int ret = system(command.c_str());
    return ret;
}

int readFileList(const std::string& basePath, std::vector<std::string>& files)
{
    DIR* dir;
    struct dirent* ptr;

    if ((dir = opendir(basePath.c_str())) == nullptr) {
        printf("open dir(%s) error\n", basePath.c_str());
        return -1;
    }

    while ((ptr = readdir(dir)) != nullptr) {
        if (ptr->d_type == 8) {
            std::string tmp = basePath + "/" + std::string(ptr->d_name);
            files.push_back(tmp);
        }
    }

    closedir(dir);
    return 0;
}

gzip_status gzip_out(chain_t* chain,
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
        return gzip_error;
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
}

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

void compress(const std::string& inputfile, const std::string& outfile)
{
    MemoryPool pool;

    // appointed configure file and select 'develop' section.
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile("../../TinyWeb.conf");
    configer.loadConfig(true);

    //get and add data
    gzip_status res;

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

    HttpModelGzip gzip(&pool);
    gzip.init();
    while(!file.noMoreData()) {
        chain_t* output = nullptr;
        file.getData(input);
        // print_chain(input);
        bool endData = file.noMoreData();
        res = gzip.compress(input, output, endData);
        if (res == gzip_error)
            break;

        gzip_out(output, outfile);

        clearData(input);
        clearData(output);

    }
}

void readFileList2(std::vector<std::string>& files, int max)
{
    std::string a = "/home/tinyweb/www/1-63k_files/";
    std::string b = "k.txt";

    for (int i=1 ;i<=max;i++)
    {
        std::string file = a + std::to_string(i) + b;
        files.push_back(file);
    }
}

void test1()
{
    int max = 4;

    std::string basePath;
    std::cout << "input file-path:";
    std::cin >> basePath;
    std::vector<std::string> files;
    std::vector<std::string> nopass_files;
    readFileList(basePath, files);
    // readFileList2(files, max);
    

    int alltest = 0;
    int passtest = 0;
    std::cout << "begin test" << std::endl;
    for (auto file : files) {

        std::cout << file << std::endl;

        alltest++;
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";

        compress(file, gzfile);
        if (0 == diffgzip(file, ungzfile)) {
            passtest++;
        }
        else {
            printf("not pass file(%s)\n", file.c_str());
            nopass_files.push_back(file);
        }

        remove(ungzfile.c_str());

        // if (alltest == 100) 
            // break;
    }

    std::cout << "[gzip Test] pass/all = " << passtest << "/" << alltest << std::endl;
    if (alltest - passtest) {
        std::cout << "no pass files:" << std::endl;
        for (auto t: nopass_files) {
            std::cout << t << std::endl;
        }
    }
}

void test12()
{
    int max = 4;

    std::vector<std::string> files;
    std::vector<std::string> nopass_files;
    

files.push_back("/home/tinyweb/www/1-63k_files//1095k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1400k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//2040k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1800k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1640k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1600k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//2000k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1440k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1840k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//5120k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1880k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1480k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1680k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1920k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1520k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1760k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1720k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1960k.txt");
files.push_back("/home/tinyweb/www/1-63k_files//1560k.txt");

    int alltest = 0;
    int passtest = 0;
    std::cout << "begin test" << std::endl;
    for (auto file : files) {

        std::cout << file << std::endl;

        alltest++;
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";

        compress(file, gzfile);
        if (0 == diffgzip(file, ungzfile)) {
            passtest++;
        }
        else {
            printf("not pass file(%s)\n", file.c_str());
            nopass_files.push_back(file);
        }

        remove(ungzfile.c_str());

        // if (alltest == 100) 
            // break;
    }

    std::cout << "[gzip Test] pass/all = " << passtest << "/" << alltest << std::endl;
    if (alltest - passtest) {
        std::cout << "no pass files:" << std::endl;
        for (auto t: nopass_files) {
            std::cout << t << std::endl;
        }
    }
}

void test2()
{
    std::string file;
    std::cout << "input file name:";
    std::cin >> file;

    std::string gzfile = file + ".tmp.gz";
    std::string ungzfile = file + ".tmp";

    compress(file, gzfile);
    // compress(file, gzfile);
    // uncompress(gzfile, ungzfile);
    if (0 == diffgzip(file, ungzfile))
        printf("pass file(%s)\n", file.c_str());
    else
        printf("not pass file(%s)\n", file.c_str());

    // remove(gzfile.c_str());
    remove(ungzfile.c_str());

    // std::cout << "[gzip Test] pass/all = " << passtest << "/" << alltest << std::endl;
}

int main()
{
    test1();
    // test12();
    // test2();
    return 0;
}
