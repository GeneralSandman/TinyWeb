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

#ifndef HTTP_MODEL_FCGI_H
#define HTTP_MODEL_FCGI_H

#include <tiny_base/log.h>
#include <tiny_http/http_model_fcgi.h>
#include <tiny_struct/sdstr_t.h>

#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                // 内部缓冲区对应的描述符
    int rio_cnt;               // 可以读取的字节数
    char* rio_bufptr;          // 下一个可以读取的字节地址
    char rio_buf[RIO_BUFSIZE]; // 内部缓冲区
} rio_t;

/*
 * 从描述符fd中读取n个字节到存储器位置usrbuf
 */
inline ssize_t rio_readn(int fd, void* usrbuf, size_t n)
{
    size_t nleft = n; // 剩下的未读字节数
    ssize_t nread;
    char* bufp = (char*)usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) { // 被信号处理函数中断返回
                nread = 0;
            } else {
                return -1; // read出错
            }
        } else if (nread == 0) { // EOF
            break;
        }

        LOG(Debug) << "read:" << nread << std::endl;
        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft); // 返回已经读取的字节数
}

#define FCGI_MAX_LENGTH 0xFFFF
#define FCGI_HOST "172.17.0.3"
#define FCGI_PORT 9000

#define FCGI_VERSION_1 1

#define FCGI_HEADER_LEN 8

// FCGI type
#define FCGI_BEGIN_REQUEST 1
#define FCGI_ABORT_REQUEST 2
#define FCGI_END_REQUEST 3
#define FCGI_PARAMS 4
#define FCGI_STDIN 5
#define FCGI_STDOUT 6
#define FCGI_STDERR 7
#define FCGI_DATA 8

// FCGI role
#define FCGI_RESPONDER 1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER 3

// FCGI connection
#define FCGI_CLOSE 0
#define FCGI_KEEP_CONN 1

#define FCGI_REQUEST_COMPLETE 1 // 正常结束
#define FCGI_CANT_MPX_CONN 2    // 拒绝新请求，无法并发处理
#define FCGI_OVERLOADED 3       // 拒绝新请求，资源超负载
#define FCGI_UNKNOWN_ROLE 4     // 不能识别的角色

typedef struct fcgi_config_t {
    bool enable;
    unsigned int connect_timeout;
    unsigned int send_timeout;
    unsigned int read_timeout;
    sdstr listen_add;
} fcgi_config_t;

typedef struct fcgi_header_t {
    unsigned char version;
    unsigned char type;
    unsigned char request_id_hi;
    unsigned char request_id_lo;
    unsigned char content_length_hi;
    unsigned char content_length_lo;
    unsigned char padding_length;
    unsigned char reserved;
} fcgi_header_t;

typedef struct fcgi_begin_request_body_t {
    unsigned char role_hi;
    unsigned char role_lo;
    unsigned char flags;
    unsigned char reserved[5];
} fcgi_begin_request_body_t;

typedef struct fcgi_begin_request_t {
    fcgi_header_t header;
    fcgi_begin_request_body_t body;
} fcgi_begin_request_t;

typedef struct fcgi_end_request_body_t {
    unsigned char appStatusB3;
    unsigned char appStatusB2;
    unsigned char appStatusB1;
    unsigned char appStatusB0;
    unsigned char protocolStatus; // 协议级别的状态码
    unsigned char reserved[3];
} fcgi_end_request_body_t;

typedef struct fcgi_end_request_t {
    fcgi_header_t header;
    fcgi_end_request_body_t body;
} fcgi_end_request_t;

typedef struct http_header {
    char uri[256];
    char method[16];
    char version[16];
    char filename[256];
    char name[256];
    char cgiargs[256];
    char contype[256];
    char conlength[16];
} http_header;

typedef ssize_t (*read_record)(int, void*, size_t);

void makeHeader(
    fcgi_header_t* header,
    int type,
    int requestId,
    int contentLength,
    int paddingLength);

void makeBeginRequestBody(
    fcgi_begin_request_body_t* body,
    int role,
    int keepConn);

int sendBeginRequestRecord(
    int sockfd,
    int requestId);

int sendParamsRecord(
    int sockfd,
    int requestId,
    char* name,
    int nlen,
    char* value,
    int vlen);

int sendEmptyParamsRecord(
    int sockfd,
    int requestId);

int sendStdinRecord(
    int sockfd,
    int requestId,
    char* data,
    int len);

int sendEmptyStdinRecord(
    int sockfd,
    int requestId);

int recvRecord(
    read_record rr,
    int fd,
    int requestId);

int send_fcgi(http_header* hp, int sock);
void recv_fcgi(int sock);

int open_clientfd();

#endif
