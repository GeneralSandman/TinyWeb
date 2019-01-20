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

#include <iostream>
#include <climits> 
#include <arpa/inet.h>
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

    printf("connect 172.17.0.3:9000 success\n");
    printf("connect fcgi-server(%s:%d) success\n", fcgiIp, fcgiPort);

    return sock;
}

void test1()
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
        strcpy(header.method, "GET");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_get.php");
        strcpy(header.name, "");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "");
        strcpy(header.conlength, "0");

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(121412);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, data);
        int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char *read_buffer;
        read_buffer=(char*)malloc(4*1024);
        int read_len = read(sock, read_buffer, 4*1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);


        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字

}

void test2()
{
    int sock;

    std::string cgiargs;
    std::cout << "input cgiargs=";
    std::cin >> cgiargs;

    sock = open_clientfd();

    for (int i = 0; i < 1; i++) {

        http_header header;
        memset((void*)&header, 0, sizeof(http_header));
        strcpy(header.uri, "/var/www/html/test/dynamic_post.php");
        strcpy(header.method, "POST");
        strcpy(header.version, "HTTP/1.1");
        strcpy(header.filename, "/var/www/html/test/dynamic_post.php");
        strcpy(header.name, "/var/www/html/test/dynamic_post.php");
        strcpy(header.cgiargs, cgiargs.c_str());
        strcpy(header.contype, "application/x-www-form-urlencoded");
        strcpy(header.conlength, "23");

        LOG(Debug) << std::endl;
        HttpModelFcgi fcgiModel(121412);
        std::string data;
        fcgiModel.buildFcgiRequest(&header, data);
        int write_len = write(sock, data.c_str(), data.size());
        if (write_len != data.size()) {
            std::cout << "send fcgi request error\n";
        }

        LOG(Debug) << std::endl;
        char *read_buffer;
        read_buffer=(char*)malloc(4*1024);
        int read_len = read(sock, read_buffer, 4*1024);
        std::string response;
        response.append(read_buffer, read_len);
        fcgiModel.parseFcgiResponse(response);


        free((void*)read_buffer);

        LOG(Debug) << std::endl;
    }
    close(sock); // 关闭与fastcgi服务器连接的套接字

}

int main()
{
    // std::cout << INT_MAX << std::endl;
    // std::cout << USHRT_MAX << std::endl;
    // std::cout << UCHAR_MAX << std::endl;
    // test1();
    test2();
}
