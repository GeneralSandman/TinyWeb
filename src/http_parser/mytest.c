/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>

#include "http_parser.h"

struct http_client
{
    int port;
    char *host;
    char *path;
};

void printHttpClient(const struct http_client *h)
{
    printf("port:%d,host:%s,path:%s\n", h->port, h->host, h->path);
}

static int parse_url(struct http_client *httpc, const char *url)
{
    struct http_parser_url u;
    http_parser_url_init(&u);
    if (0 == http_parser_parse_url(url, strlen(url), 0, &u))
    {
        if (u.field_set & (1 << UF_PORT))
        {
            httpc->port = u.port;
        }
        else
        {
            httpc->port = 80;
        }

        if (httpc->host != NULL)
            free(httpc->host);
        if (u.field_set & (1 << UF_HOST))
        {
            httpc->host = (char *)malloc(u.field_data[UF_HOST].len + 1);
            strncpy(httpc->host, url + u.field_data[UF_HOST].off, u.field_data[UF_HOST].len);
            httpc->host[u.field_data[UF_HOST].len] = 0;
        }

        if (httpc->path)
            free(httpc->path);
        if (u.field_set & (1 << UF_PATH))
        {
            httpc->path = (char *)malloc(u.field_data[UF_PATH].len + 1);
            strncpy(httpc->path, url + u.field_data[UF_PATH].off, u.field_data[UF_PATH].len);
            httpc->path[u.field_data[UF_PATH].len] = 0;
        }

        return 0;
    }

    return -1;
}

void testParseUrl(void)
{
    //replace urls with urls.txt
    char *urls[] = {
        "https://blog.csdn.net/foruok/article/details/8954726",
        "www.baidu.com",
        "https://www.baidu.com",
        "http://127.0.0.1:9999/",
        "http://www.dissigil.cn/resume.pdf",
        "https::///ksdjf.cn/",
        "https::/dsjfll.cb.cb//",
        "https://did.cn/../dsf"};

    for (int i = 0; i < sizeof(urls) / sizeof(char *); i++)
    {
        struct http_client clientData;
        clientData.port = 0;
        clientData.host = NULL;
        clientData.path = NULL;
        int res = 0;
        res = parse_url(&clientData, urls[i]);
        printf("%s\n", urls[i]);

        if (res != 0)
        {
            printf("parse url error\n\n");
            continue;
        }

        printHttpClient(&clientData);
        printf("\n");

        if (clientData.host != NULL)
            free(clientData.host);
        if (clientData.path != NULL)
            free(clientData.path);
    }
}

/*
typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
typedef int (*http_cb) (http_parser*);
*/

typedef struct
{
    // socket_t sock;
    void *buffer;
    int buf_len;
} custom_data_t;

int my_url_callback(http_parser *parser, const char *at, size_t length)
{
    printf("invoke function my_url_callback()\n");
    printf("%s\n", at);
    return 0;
}

int my_header_field_callback(http_parser *parser, const char *at, size_t length)
{
    printf("invoke function my_header_field_callback()\n");
    printf("%s\n", at);    
    return 0;
}

void testHttpParser(void)
{
    http_parser_settings settings;
    settings.on_url = my_url_callback;
    settings.on_header_field = my_header_field_callback;

    http_parser *parser = malloc(sizeof(http_parser));
    http_parser_init(parser, HTTP_REQUEST);
    custom_data_t *my_data = malloc(sizeof(custom_data_t));
    // parser->data = my_socket;

    size_t len = 80 * 1024;
    size_t nparsed;
    char buf[len];
    ssize_t recved;

    char *tmp = "GET / HTTP/1.1\r\n"
                "Host: 127.0.0.1:9999\r\n"
                "Connection: keep-alive\r\n"
                "Cache-Control: max-age=0\r\n"
                "Upgrade-Insecure-Requests: 1\r\n"
                "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n";

    // @parser 解析器对象
    // @&settings 解析时的回调函数
    // @buf 要解析的数据
    // @receved 要解析的数据大小
    nparsed = http_parser_execute(parser, &settings, tmp, strlen(tmp));

    // 如果解析到websocket请求
    if (parser->upgrade)
    {
        /* handle new protocol */
        // 如果解析出错，即解析完成的数据大小不等于传递给http_parser_execute的大小
    }
    else if (nparsed != recved)
    {
        /* Handle error. Usually just close the connection. */
    }

    free(parser);
    free(my_data);
}

int main()
{
    // testParseUrl();
    testHttpParser();

    return 0;
}

int main__(int argc, char **argv)
{
    int port = 9999;

    struct sockaddr_in serveraddr, clientaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(port); //或者htons(SERV_PORT);
    int res = bind(listenfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (res == -1)
    {
        printf("bind error\n");
        exit(-1);
    }

    listen(listenfd, 8);
    for (;;)
    {
        socklen_t clilen;
        int connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clilen);
        if (connfd > 0)
        {
            printf("get a connection\n");
        }
        char *str = inet_ntoa(clientaddr.sin_addr);
        char line[8192];
        int n;
        if ((n = read(connfd, line, 8192)) < 0)
            close(connfd);
        printf("%s\n", line);

        char buf[8192];
        sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
        sprintf(buf, "%sContent-Type: text/html\r\n\r\n", buf);
        write(connfd, buf, strlen(buf));
    }
}
