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
#include <tiny_base/log.h>
#include <tiny_http/http_model_fcgi.h>
#include <tiny_struct/sdstr_t.h>

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

void makeHeader(
    fcgi_header_t* header,
    int type,
    int requestId,
    int contentLength,
    int paddingLength)
{
    header->version = FCGI_VERSION_1;
    header->type = (unsigned char)type;
    header->request_id_hi = (unsigned char)((requestId >> 8) & 0xff);
    header->request_id_lo = (unsigned char)((requestId)&0xff);
    header->content_length_hi = (unsigned char)((contentLength >> 8) & 0xff);
    header->content_length_lo = (unsigned char)((contentLength)&0xff);
    header->padding_length = (unsigned char)paddingLength;
    header->reserved = 0;
}

void makeBeginRequestBody(
    fcgi_begin_request_body_t* body,
    int role,
    int keepConn)
{
    body->role_hi = (unsigned char)((role >> 8) & 0xff);
    body->role_lo = (unsigned char)(role & 0xff);
    body->flags = (unsigned char)((keepConn) ? 1 : 0); // 1为长连接，0为短连接
    memset(body->reserved, 0, sizeof(body->reserved));
}

int sendBeginRequestRecord(int sockfd, int requestId)
{
    int ret;
    fcgi_begin_request_t r;

    makeHeader(&r.header, FCGI_BEGIN_REQUEST, requestId, sizeof(r.body), 0);
    makeBeginRequestBody(&r.body, FCGI_RESPONDER, FCGI_CLOSE);
    ret = write(sockfd, &r, sizeof(r));

    if (ret == sizeof(r)) {
        return 0;
    } else {
        return -1;
    }
}

int sendParamsRecord(
    int sockfd,
    int requestId,
    char* name,
    int nlen,
    char* value,
    int vlen)
{
    unsigned char *buf, *old;
    int ret, pl, cl, buffer_size;

    // Count content length.
    cl = nlen + vlen;
    cl = (nlen < 128) ? (cl + 1) : (cl + 4);
    cl = (vlen < 128) ? (cl + 1) : (cl + 4);

    // Count padding length. Round up 8.
    pl = (cl % 8) == 0 ? 0 : (8 - cl % 8);
    buffer_size = FCGI_HEADER_LEN + cl + pl;
    old = buf = (unsigned char*)malloc(buffer_size);

    fcgi_header_t header;
    makeHeader(&header, FCGI_PARAMS, requestId, cl, pl);
    memcpy(buf, (char*)&header, FCGI_HEADER_LEN);
    buf += FCGI_HEADER_LEN;

    if (nlen < 128) {
        // Store nlen with one byte.
        *buf++ = (unsigned char)nlen;
    } else {
        // Store nlen with four byte.
        *buf++ = (unsigned char)((nlen >> 24) | 0x80);
        *buf++ = (unsigned char)(nlen >> 16);
        *buf++ = (unsigned char)(nlen >> 8);
        *buf++ = (unsigned char)nlen;
    }

    if (vlen < 128) {
        // Store nlen with one byte.
        *buf++ = (unsigned char)vlen;
    } else {
        // Store nlen with four byte.
        *buf++ = (unsigned char)((vlen >> 24) | 0x80);
        *buf++ = (unsigned char)(vlen >> 16);
        *buf++ = (unsigned char)(vlen >> 8);
        *buf++ = (unsigned char)vlen;
    }

    memcpy(buf, name, nlen);
    buf = buf + nlen;
    memcpy(buf, value, vlen);

    ret = write(sockfd, old, buffer_size);

    free((void*)old);

    if (ret == buffer_size) {
        return 0;
    } else {
        return -1;
    }
}

int sendEmptyParamsRecord(int sockfd, int requestId)
{
    int ret;
    fcgi_header_t header;

    makeHeader(&header, FCGI_PARAMS, requestId, 0, 0);
    ret = write(sockfd, (char*)&header, FCGI_HEADER_LEN);

    if (ret == FCGI_HEADER_LEN) {
        return 0;
    } else {
        return -1;
    }
}

int sendStdinRecord(
    int sockfd,
    int requestId,
    char* data,
    int len)
{
    int cl = len, pl, ret;
    char padding_buf[8] = { 0 };

    while (len > 0) {

        if (len > FCGI_MAX_LENGTH) {
            cl = FCGI_MAX_LENGTH;
        }

        // Count padding length.
        pl = (cl % 8) == 0 ? 0 : 8 - (cl % 8);

        fcgi_header_t header;
        makeHeader(&header, FCGI_STDIN, requestId, cl, pl);
        ret = write(sockfd, (char*)&header, FCGI_HEADER_LEN);
        if (ret != FCGI_HEADER_LEN) {
            return -1;
        }

        // Send stdin data.
        ret = write(sockfd, data, cl);
        if (ret != cl) {
            return -1;
        }

        // Send padding content data.
        if (pl > 0) {
            ret = write(sockfd, padding_buf, pl);
            if (ret != pl) {
                return -1;
            }
        }

        len -= cl;
        data += cl;
    }

    return 0;
}

int sendEmptyStdinRecord(int sockfd, int requestId)
{
    int ret;
    fcgi_header_t header;

    makeHeader(&header, FCGI_STDIN, requestId, 0, 0);
    ret = write(sockfd, (char*)&header, FCGI_HEADER_LEN);

    if (ret == FCGI_HEADER_LEN) {
        return 0;
    } else {
        return -1;
    }
}

int recvRecord(
    read_record rr,
    int fd,
    int requestId)
{
    fcgi_header_t header;
    fcgi_end_request_body_t endr;

    char *con_buf = nullptr, *err_buf = nullptr;
    unsigned int con_len = 0, err_len = 0;
    int padding_buf[8] = { 0 };
    int ret;

    unsigned char type;
    unsigned int request_id;
    unsigned int content_length;
    unsigned char padding_length;

    while (rr(fd, &header, sizeof(header)) > 0) {

        type = header.type;
        request_id = (unsigned int)(header.request_id_hi << 8) + (unsigned int)(header.request_id_lo);
        content_length = (unsigned int)(header.content_length_hi << 8) + (unsigned int)(header.content_length_lo);
        padding_length = header.padding_length;

        printf("type:%d,requestId:%d,contentLength:%d,paddingLength:%d\n",
            type, request_id, content_length, padding_length);

        if (request_id != requestId) {
            LOG(Debug) << "requestId not same\n";
            continue;
        }

        if (type == FCGI_STDOUT) {

            con_len += content_length;

            if (con_buf != nullptr) {
                con_buf = (char*)realloc((void*)con_buf, con_len);
            } else {
                con_buf = (char*)malloc(content_length);
            }

            // FIXME: store date begin last used index.
            ret = rr(fd, con_buf, content_length);
            if (ret == -1 || ret != content_length) {
                printf("read fcgi_stdout record error\n");
                return -1;
            }

            printf("1zhenhuli out:%.*s\n", content_length, con_buf);

            if (padding_length > 0) {
                ret = rr(fd, padding_buf, padding_length);
                if (ret == -1 || ret != padding_length) {
                    printf("read fcgi_stdout padding error\n");
                    return -1;
                }
                printf("1zhenhuli padding:%.*s\n", padding_length, padding_buf);
            }

        } else if (type == FCGI_STDERR) {

            err_len += content_length;

            if (err_buf != NULL) {
                err_buf = (char*)realloc((void*)err_buf, err_len);
            } else {
                err_buf = (char*)malloc(content_length);
            }

            // FIXME: store date begin last used index.
            ret = rr(fd, err_buf, content_length);
            if (ret == -1 || ret != content_length) {
                return -1;
            }

            printf("2zhenhuli err:%.*s\n", content_length, err_buf);

            if (padding_length > 0) {
                ret = rr(fd, padding_buf, padding_length);
                if (ret == -1 || ret != padding_length) {
                    printf("read fcgi_stderr padding error\n");
                    return -1;
                }
                printf("2zhenhuli padding:%.*s\n", padding_length, padding_buf);
            }

        } else if (type == FCGI_END_REQUEST) {

            ret = rr(fd, &endr, sizeof(endr));

            if (ret == -1 || ret != sizeof(endr)) {
                free(con_buf);
                free(err_buf);
                return -1;
            }

            printf("3zhenhuli end\n");
            free(con_buf);
            free(err_buf);
            return 0;
        }
    }
    return 0;
}

int send_fcgi(http_header* hp, int sock)
{
    int requestId, i, l;
    char* buf;

    requestId = sock;

    char* paname[] = {
        "SCRIPT_FILENAME",
        "SCRIPT_NAME",
        "REQUEST_METHOD",
        "REQUEST_URI",
        "QUERY_STRING",
        "CONTENT_TYPE",
        "CONTENT_LENGTH"
    };

    int paoffset[] = {
        (size_t) & (((http_header*)0)->filename),
        (size_t) & (((http_header*)0)->name),
        (size_t) & (((http_header*)0)->method),
        (size_t) & (((http_header*)0)->uri),
        (size_t) & (((http_header*)0)->cgiargs),
        (size_t) & (((http_header*)0)->contype),
        (size_t) & (((http_header*)0)->conlength)
    };

    if (sendBeginRequestRecord(sock, requestId) < 0) {
        printf("sendBeginRequestRecord error\n");
        return -1;
    }

    l = sizeof(paoffset) / sizeof(paoffset[0]);
    for (i = 0; i < l; i++) {

        char* tmp = (char*)((char*)hp + paoffset[i]);
        int len = strlen(tmp);
        printf("paname:%.*s,offset:%d,len:%d,data:%.*s\n",
            strlen(paname[i]), paname[i], paoffset[i], len, len, tmp);
        if (len > 0) {

            if (sendParamsRecord(sock, requestId,
                    paname[i], strlen(paname[i]),
                    tmp, len)
                < 0) {
                printf("sendParamsRecord error\n");
                return -1;
            }
        }
    }

    if (sendEmptyParamsRecord(sock, requestId) < 0) {
        printf("sendEmptyParamsRecord error\n");
        return -1;
    }

    l = atoi(hp->conlength);
    if (l > 0) {
        buf = (char*)malloc(l + 1);
        memset(buf, '\0', l);
        // if (rio_readnb(rp, buf, l) < 0) {
        printf("rio_readn error\n");
        free(buf);
        return -1;
        // }

        if (sendStdinRecord(sock, requestId, buf, l) < 0) {
            printf("sendStdinRecord error\n");
            free(buf);
            return -1;
        }

        free(buf);
    }

    if (sendEmptyStdinRecord(sock, requestId) < 0) {
        printf("sendEmptyStdinRecord error\n");
        return -1;
    }

    return 0;
}

void recv_fcgi(int sock)
{
    int requestId;
    char* p;
    int n;

    requestId = sock;

    // 读取处理结果
    if (recvRecord(rio_readn, sock, requestId) < 0) {
        printf("recvRecord error\n");
        // return -1;
    }

    /*
    FCGI_EndRequestBody endr;
    char *out, *err;
    int outlen, errlen;

    requestId = sock;

    // 读取处理结果
    if (recvRecord(rio_readn, sock, requestId, &out, &outlen, &err, &errlen, &endr) < 0) {
        error_log("recvRecord error", DEBUGARGS);
        return -1;
    }

    if (outlen > 0) {
        p = index(out, '\r'); 
        n = (int)(p - out);
        rio_writen(fd, p + 3, outlen - n - 3);
        free(out);
    }

    if (errlen > 0) {
        rio_writen(fd, err, errlen);
        free(err);
    }
    */
}

int open_clientfd()
{
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock) {
        printf("socket error\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(FCGI_HOST);
    serv_addr.sin_port = htons(FCGI_PORT);

    if (-1 == connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        handle_error("connect error\n");
        return -1;
    }

    printf("connect 172.17.0.2:9000 success\n");

    return sock;
}
