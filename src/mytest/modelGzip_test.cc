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

void compress(const std::string& inputfile, const std::string& outfile)
{
    MemoryPool pool;
    int inputfd;
    int read_len;
    char* read_buf;

    // appointed configure file and select 'develop' section.
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile("../../TinyWeb.conf");
    configer.loadConfig(true);

    //get and add data
    gzip_config_t config;
    gzip_context_t context;
    gzip_status res;

    get_gzip_config(&config);
    gzip_context_init(&pool, &config, &context);

    //open input file
    inputfd = open(inputfile.c_str(), O_RDONLY);
    if (-1 == inputfd) {
        printf("open input-file(%s) error\n", inputfile.c_str());
        return;
    }
    read_buf = (char*)malloc(1024);
    while ((read_len = read(inputfd, read_buf, 1024)) > 0) {
        LOG(Debug) << "read data size(" << read_len << "),and write to chain" << std::endl;
        gzip_add_data(&context, read_buf, read_len);
    }

    // gzip
    res = gzip_body(&context);
    if (gzip_error == res) {
        std::cout << "gzip data error" << std::endl;
    }
    gzip_out(&context, outfile);

    free((void*)read_buf);
}

void test1()
{
    std::string basePath;
    std::cin >> basePath;
    std::vector<std::string> files;
    readFileList(basePath, files);

    int alltest = 0;
    int passtest = 0;
    for (auto file : files) {
        alltest++;
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";
        std::cout << "test " << file << std::endl;
        compress(file, gzfile);
        // compress(file, gzfile);
        // uncompress(gzfile, ungzfile);
        if (0 == diffgzip(file, ungzfile))
            passtest++;
        else
            printf("not pass file(%s)\n", file.c_str());
    }

    for (auto file : files) {
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";
        // remove(gzfile.c_str());
        remove(ungzfile.c_str());
    }

    std::cout << "[gzip Test] pass/all = " << passtest << "/" << alltest << std::endl;
}
int main()
{
    test1();
    return 0;
}
