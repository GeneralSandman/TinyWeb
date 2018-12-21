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


#include <tiny_http/http_model_fcgi.h>

#include <iostream>

using namespace std;

int main()
{
    int sock; 

    std::string url;
    std::cout << "input url=";
    std::cin >> url;
    
    sock = open_clientfd();

    char *uri = "/";
    char *method = "POST";
    char *version = "1.1";
    char *filename = "test.php";
    char *name = "";
    char *cgiargs = "";
    char *contype = "";
    char *conlength = "0";
    
    http_header header;
    memset((void*)&header, 0, sizeof(http_header));
    // header.uri = uri;
    // header.method = method;
    // header.version = version;
    // header.filename = filename;
    // header.name = name;
    // header.cgiargs = cgiargs;
    // header.contype = contype;
    // header.conlength = conlength;
    strcpy(header.uri, "http://127.0.0.1/dynamic/test.php?name=zhenhuli");
    strcpy(header.method, "GET");
    strcpy(header.version, "HTTP/1.1");
    strcpy(header.filename, "/var/www/html/dynamic/test.php");
    strcpy(header.name, "/dynamic/test.php");
    strcpy(header.cgiargs, "name=zhenhuli");
    strcpy(header.contype, "");
    strcpy(header.conlength, "0");

    LOG(Debug) << std::endl;
    // 发送http请求数据
    send_fcgi(&header, sock);

    LOG(Debug) << std::endl;
    // 接收处理结果
    recv_fcgi(sock);

    LOG(Debug) << std::endl;
    close(sock); // 关闭与fastcgi服务器连接的套接字  

    return 0;
}
