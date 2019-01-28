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

#include <tiny_base/api.h>
#include <tiny_http/http_model_fcgi.h>

#include <arpa/inet.h>
#include <climits>
#include <ctype.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int open_clientfd()
{
    int sock;
    struct sockaddr_in serv_addr;

    char fcgiIp[] = "172.17.0.3";
    int fcgiPort = 9000;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock) {
        printf("socket error\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(fcgiIp);
    serv_addr.sin_port = htons(fcgiPort);

    if (-1 == connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        handle_error("connect error\n");
        return -1;
    }

    printf("[Info]connect fcgi-server(%s:%d) success\n", fcgiIp, fcgiPort);

    return sock;
}

void get_test()
{
    int sock;

    std::string cgiargs = "name=zhenhuli&age=99";

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "GET");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_get.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "");
        strcpy(header.conlength, "0");
        std::string content;

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字
}

typedef struct
{
    const char* method;
    const char* filename;
    const char* content_type;
    unsigned int content_length;
    const char* content;

    bool valid;
    const char* response_str;
} testPost;

testPost posts[] = {
    {
        .method = "POST",
        .filename = "/var/www/html/test/dynamic_post4.php",
        .content_type = "text/xml",
        .content_length = 79,
        .content = "<xml>\r\n"
                   "<name>zhenhuli</name>\r\n"
                   "<email>generalsandman@outlook.com</email>\r\n"
                   "</xml>",

        .valid = true,
        .response_str = "",
    },

    {
        .method = "POST",
        .filename = "/var/www/html/test/dynamic_post4.php",
        .content_type = "text/xml",
        .content_length = 75,
        .content = "<xml>\r\n"
                   "<name>zhenhuli</name>\r\n"
                   "<email>generalsandman@163.com</email>\r\n"
                   "</xml>",

        .valid = true,
        .response_str = "",
    },

    {
        .method = "POST",
        .filename = "/var/www/html/test/dynamic_post4.php",
        .content_type = "text/xml",
        .content_length = 67,
        .content = "<xml>\r\n"
                   "<name>zhenhuli</name>\r\n"
                   "<email>generalsandman</email>\r\n"
                   "</xml>",

        .valid = true,
        .response_str = "",
    },

    {
        .method = "",
        .filename = "",
        .content_type = "",
        .content_length = 0,
        .content = "",

        .valid = true,
        .response_str = "",
    },

    {
        .method = "",
        .filename = "",
        .content_type = "",
        .content_length = 0,
        .content = "",

        .valid = true,
        .response_str = "",
    },
};

void post_test1()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post1.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "application/x-www-form-urlencoded;charset=utf-8");
        strcpy(header.conlength, "23");
        std::string content = "name=zhenhuli&email=gsd";

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字
}

void post_test2()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post2.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "application/json");
        strcpy(header.conlength, "33");
        std::string content = "{\"name\":\"zhenhuli\",\"email\":\"gsd\"}";

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字
}

void post_test3()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post3.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "multipart/form-data; boundary=----WebKitFormBoundaryrGKCBY7qhFd3TrwA");
        strcpy(header.conlength, "255");
        char tmp[] = "------WebKitFormBoundaryrGKCBY7qhFd3TrwA\r\n"
                     "Content-Disposition: form-data; name=\"name\"\r\n"
                     "\r\n"
                     "zhenhuli\r\n"
                     "------WebKitFormBoundaryrGKCBY7qhFd3TrwA\r\n"
                     "Content-Disposition: form-data; name=\"email\"\r\n"
                     "\r\n"
                     "generalsandman@163.com\r\n"
                     "------WebKitFormBoundaryrGKCBY7qhFd3TrwA--";
        std::string content(tmp, strlen(tmp));

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字
}

void post_test4()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post4.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "text/xml");
        strcpy(header.conlength, "79");
        char tmp[] = "<xml>\r\n"
                     "<name>zhenhuli</name>\r\n"
                     "<email>generalsandman@outlook.com</email>\r\n"
                     "</xml>";
        std::string content(tmp, strlen(tmp));

        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);
    }
}

void post_test5()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post4.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "text/xml");
        strcpy(header.conlength, "89");
        char tmp[] = "<?xml version=\"1.0\"?>\r\n"
                     "<name>zhenhuli</name>\r\n"
                     "<email>generalsandman@outlook.com</email>\r\n";
        std::string content(tmp, strlen(tmp));
        std::cout << "len:" << strlen(tmp) << std::endl;

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(1212);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, content, data);
        unsigned int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char* read_buffer;
        read_buffer = (char*)malloc(4 * 1024);
        int read_len = read(sock, read_buffer, 4 * 1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);

        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
}

int main()
{
    // get_test();
    // post_test1();
    // post_test2();
    post_test3();
    // post_test4();
    // post_test5();
}
