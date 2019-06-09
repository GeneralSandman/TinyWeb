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

#include <tiny_struct/sdstr_t.h>
#include <tiny_base/log.h>
#include <tiny_http/http_model_fcgi.h>

// FCGI basic macro
#define FCGI_MAX_LENGTH 0xFFFF
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

#define FCGI_REQUEST_COMPLETE 1 // request complete
#define FCGI_CANT_MPX_CONN 2    // max connection
#define FCGI_OVERLOADED 3       // connection overload
#define FCGI_UNKNOWN_ROLE 4     // unknow role

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
    unsigned char protocolStatus;
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

class HttpModelFcgi {
private:
    unsigned short requestId;

public:
    HttpModelFcgi(unsigned short request_id)
    {
        requestId = request_id;
        LOG(Debug) << "class HttpModelFcgi constructor\n";
    }

    void makeHeader(
        fcgi_header_t* header,
        int type,
        int contentLength,
        int paddingLength);
    void makeBeginRequestBody(
        fcgi_begin_request_body_t* body,
        int role,
        int keepConn);
    void makeBeginRequestRecord(
        std::string& data);

    void makeParamsRecord(
        char* name,
        int nlen,
        char* value,
        int vlen,
        std::string& data);
    void makeEmptyParamsRecord(
        std::string& data);

    void makeStdinRecord(
        char* data_,
        int len,
        std::string& data);
    void makeEmptyStdinRecord(
        std::string& data);

    void buildFcgiRequest(http_header* hp, const std::string& content, std::string& data);
    int parseRecord(
        const std::string& data);
    int parseFcgiResponse(const std::string& data);

    ~HttpModelFcgi()
    {
        LOG(Debug) << "class HttpModelFcgi destructor\n";
    }
};

#endif
