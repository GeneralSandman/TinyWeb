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

// gzip格式
// (u16)MAGIC + (u8)DEFLATE + (u8)flag + (u32)time + (u8)deflate_flag + (u8)OS_CODE

int compress(const std::string &inputfile, const std::string &outputfile)
{
    //open input file
    int inputfd = open(inputfile.c_str(), O_RDONLY);
    if (-1 == inputfd)
    {
        std::cout << "open input file error\n";
        return -1;
    }

    //open output file
    int outputfd = open(outputfile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (-1 == outputfd)
    {
        std::cout << "open output file error\n";
        return -1;
    }

    //malloc read buf
    char *read_buf = (char *)malloc(8192);
    if (NULL == read_buf)
    {
        std::cout << "create read buffer error\n";
        return -1;
    }

    //malloc write buf
    char *write_buf = (char *)malloc(8192 * 4);
    if (NULL == write_buf)
    {
        std::cout << "create write buffer error\n";
        return -1;
    }

    //init zlib
    z_stream stream;
    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.opaque = NULL;

    int ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           MAX_WBITS + 16,
                           8,
                           Z_DEFAULT_STRATEGY);

    if (Z_OK != ret)
    {
        printf("init deflate error\n");
        return ret;
    }

    //compress
    int read_len;
    int write_len;
AGAIN:
    while ((read_len = read(inputfd, read_buf, 8192)) >= 0)
    {
        int flush = Z_NO_FLUSH;
        if (read_len == 0)
        {
            flush = Z_FINISH;
        }

        stream.next_in = (unsigned char *)read_buf;
        stream.avail_in = read_len;

        do
        {
            write_len = 4 * 8192;
            stream.next_out = (unsigned char *)write_buf;
            stream.avail_out = write_len;
            ret = deflate(&stream, flush);
            if (flush == Z_NO_FLUSH && ret != Z_OK)
            {
                goto ERROR;
            }

            if (flush == Z_FINISH && ret != Z_STREAM_END)
            {
                goto ERROR;
            }

            int avail = write_len - stream.avail_out;
            write(outputfd, write_buf, avail);
        } while (stream.avail_out == 0); // 如果avail_out不够用了,说明输入的buffer还有没有压缩的部分,那么要继续压缩.

        // 如果输入长度不是0,说明压缩不完整,可以直接退出.
        assert(stream.avail_in == 0);

        if (read_len == 0)
        {
            break;
        }
    }

    if (read_len < 0 && errno == EINTR)
        goto AGAIN;

    deflateEnd(&stream);
    free((void *)read_buf);
    free((void *)write_buf);
    close(inputfd);
    close(outputfd);
    return 0;
ERROR:
    deflateEnd(&stream);
    free((void *)read_buf);
    free((void *)write_buf);
    close(inputfd);
    close(outputfd);
    return 0;
}

int uncompress(const std::string &inputfile, const std::string &outputfile)
{
    //open input file
    int inputfd = open(inputfile.c_str(), O_RDONLY);
    if (-1 == inputfd)
    {
        std::cout << "open input file error\n";
        return -1;
    }

    //open output file
    int outputfd = open(outputfile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (-1 == outputfd)
    {
        std::cout << "open output file error\n";
        return -1;
    }

    //malloc read buf
    char *read_buf = (char *)malloc(8192);
    if (NULL == read_buf)
    {
        std::cout << "create read buffer error\n";
        return -1;
    }

    //malloc write buf
    char *write_buf = (char *)malloc(8192 * 4);
    if (NULL == write_buf)
    {
        std::cout << "create write buffer error\n";
        return -1;
    }

    //init zlib
    z_stream stream;
    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.opaque = NULL;

    int ret = inflateInit2(&stream, MAX_WBITS + 16);
    if (Z_OK != ret)
    {
        printf("init deflate error\n");
        return ret;
    }

    //compress
    int read_len;
    int write_len;
AGAIN:
    while ((read_len = read(inputfd, read_buf, 8192)) >= 0)
    {
        int flush = Z_NO_FLUSH;
        if (read_len == 0)
        {
            flush = Z_FINISH;
        }

        stream.next_in = (unsigned char *)read_buf;
        stream.avail_in = read_len;

        do
        {
            write_len = 4 * 8192;
            stream.next_out = (unsigned char *)write_buf;
            stream.avail_out = write_len;
            ret = inflate(&stream, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            switch (ret)
            {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&stream);
                return ret;
            }

            int avail = write_len - stream.avail_out;
            write(outputfd, write_buf, avail);
        } while (stream.avail_out == 0); // 如果avail_out不够用了,说明输入的buffer还有没有压缩的部分,那么要继续压缩.

        // 如果输入长度不是0,说明压缩不完整,可以直接退出.
        assert(stream.avail_in == 0);

        if (read_len == 0)
        {
            break;
        }
    }

    if (read_len < 0)
        std::cout << "read input file error\n";

    if (read_len < 0 && errno == EINTR)
        goto AGAIN;

    deflateEnd(&stream);
    free((void *)read_buf);
    free((void *)write_buf);
    close(inputfd);
    close(outputfd);
    return 0;
ERROR:
    deflateEnd(&stream);
    free((void *)read_buf);
    free((void *)write_buf);
    close(inputfd);
    close(outputfd);
    return 0;
}

int diff(const std::string &a, const std::string &b)
{
    std::string command = "diff " + a + " " + b;
    int ret = system(command.c_str());
    return ret;
}

int readFileList(const std::string &basePath, std::vector<std::string> &files)
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(basePath.c_str())) == NULL)
    {
        std::cout << "open dir error\n";
        return 1;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type == 8)
        {
            std::string tmp = basePath + "/" + std::string(ptr->d_name);
            files.push_back(tmp);
        }
    }

    closedir(dir);
    return 1;
}

int main(void)
{
    DIR *dir;
    std::string basePath;
    std::cin >> basePath;
    std::vector<std::string> files;
    readFileList(basePath.c_str(), files);

    int alltest = 0;
    int passtest = 0;
    for (auto file : files)
    {
        alltest++;
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";
        compress(file, gzfile);
        uncompress(gzfile, ungzfile);
        if (0 == diff(file, ungzfile))
        {
            passtest++;
        }
        else
        {
            std::cout << "not pass:" << file << std::endl;
        }
    }

    for (auto file : files)
    {
        std::string gzfile = file + ".tmp.gz";
        std::string ungzfile = file + ".tmp";
        remove(gzfile.c_str());
        remove(ungzfile.c_str());
    }

    std::cout << "[gzip Test] pass/all = " << passtest << "/" << alltest << std::endl;
    return 0;
}
