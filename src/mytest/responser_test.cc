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
#include <tiny_http/http_parser.h>
#include <tiny_http/http_responser.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

typedef struct testBody {
    const char* str;
    bool valid;
    const char* body;

} testBody;

testBody bodys_gzip[] = {
    {
        .str = "GET /1-63k_files/1k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/2k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/3k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/4k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/5k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/6k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/7k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/8k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/9k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/10k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/11k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/12k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/13k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/14k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/15k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/16k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/17k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/18k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

};

testBody bodys[] = {
    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.html HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.htm HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.shtml HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.ico HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.js HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.css HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.gif HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.php HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.jpeg HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /filetypes/test.jpg HTTP/1.1\r\n"
               "Connection: keep-alive\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /image.jpg HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /xxxxxxxx.html HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET / HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /test HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /test/index.html HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /test/tinyweb.html HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /welcome.html HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7\r\n"
               "Cookie: _ga=GA1.2.853646746.1544367008; _gid=GA1.2.197760587.1546848699\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /index.htm HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk..\r\n"
               "1C\r\n"
               "and this is the second one..\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
                "and this is the second one..",
    },

    {
        .str = "GET http://127.0.0.1:9999/ HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk..\r\n"
               "1C\r\n"
               "and this is the second one..\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
                "and this is the second one..",
    },

    {
        .str = "GET http://127.0.0.1:9999 HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk..\r\n"
               "1C\r\n"
               "and this is the second one..\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
                "and this is the second one..",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Content-Length: 10\r\n"
               "\r\n"
               "helloworlddd",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk\r\n"
               "\r\n"
               "1C\r\n"
               "and this is the second one\r\n"
               "\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk\r\n"
               "\r\n"
               "1C\r\n"
               "and this is the second one\r\n"
               "\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk\r\n"
               "\r\n"
               "1C\r\n"
               "and this is the second one\r\n"
               "\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "2\r\n"
               "OK\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk78\r\n"
               "1C\r\n"
               "and this is the second one??\r\n"
               "2\r\n"
               "bo\r\n"
               "2\r\n"
               "dy\r\n"
               "0\r\n"
               "\r\n"
               "2\r\n"
               "OK\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },
};

void testResponser_gzip()
{
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys_gzip) / sizeof(bodys_gzip[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;

        int begin = 0;
        HttpParserSettings settings;
        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);
        HttpRequest* result = new HttpRequest;
        int tmp = parser.execute(bodys_gzip[i].str,
            begin,
            strlen(bodys_gzip[i].str),
            result);

        bool res = (tmp == -1) ? false : true;
        if (res) {
            HttpResponser responser;
            string data;

            responser.response(result, data);
            passtest++;

            // std::cout << data << std::endl;
        }

        delete result;
    }

    std::cout << passtest << "/" << alltest << std::endl;
}

void testResponser()
{
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys) / sizeof(bodys[0]);

    for (int i = 0; i < 16; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;

        int begin = 0;
        HttpParserSettings settings;
        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);
        HttpRequest* result = new HttpRequest;
        int tmp = parser.execute(bodys[i].str,
            begin,
            strlen(bodys[i].str),
            result);

        bool res = (tmp == -1) ? false : true;
        if (res) {
            HttpResponser responser;
            string data;

            responser.response(result, data);
            passtest++;

            std::cout << data << std::endl;
        }

        delete result;
    }

    std::cout << passtest << "/" << alltest << std::endl;
}

void initConfiger()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";
    std::cout << std::endl;
}

int main()
{
    initConfiger();
    headerMeaningInit();
    testResponser_gzip();
    // testResponser();
    return 0;
}
