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

#include <string>

void HttpModelFcgi::makeHeader(
    fcgi_header_t* header,
    int type,
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

void HttpModelFcgi::makeBeginRequestBody(
    fcgi_begin_request_body_t* body,
    int role,
    int keepConn)
{
    body->role_hi = (unsigned char)((role >> 8) & 0xff);
    body->role_lo = (unsigned char)(role & 0xff);
    body->flags = (unsigned char)((keepConn) ? 1 : 0);
    memset(body->reserved, 0, sizeof(body->reserved));
}

void HttpModelFcgi::makeBeginRequestRecord(std::string& data)
{
    fcgi_begin_request_t r;

    makeHeader(&r.header, FCGI_BEGIN_REQUEST, sizeof(r.body), 0);
    makeBeginRequestBody(&r.body, FCGI_RESPONDER, FCGI_CLOSE);

    data.append((char*)&r, sizeof(r));
}

void HttpModelFcgi::makeParamsRecord(
    char* name,
    int nlen,
    char* value,
    int vlen,
    std::string& data)
{
    unsigned char *buf, *old;
    int pl, cl, buffer_size;

    // Count content length.
    cl = nlen + vlen;
    cl = (nlen < 128) ? (cl + 1) : (cl + 4);
    cl = (vlen < 128) ? (cl + 1) : (cl + 4);

    // Count padding length. Round up 8.
    pl = ((cl % 8) == 0) ? 0 : (8 - cl % 8);
    buffer_size = FCGI_HEADER_LEN + cl + pl;
    old = buf = (unsigned char*)malloc(buffer_size);
    if (nullptr == old) {
        return;
        // FIXME: return TINY_WEB_ERROR
    }

    fcgi_header_t header;
    makeHeader(&header, FCGI_PARAMS, cl, pl);
    memcpy(buf, (char*)&header, FCGI_HEADER_LEN);
    buf += FCGI_HEADER_LEN;

    if (nlen < 128) {
        // Store nlen with one byte.
        *buf++ = (unsigned char)nlen;
    } else {
        // Store nlen with four bytes.
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

    data.append((char*)old, buffer_size);

    free((void*)old);
}

void HttpModelFcgi::makeEmptyParamsRecord(std::string& data)
{
    fcgi_header_t header;

    makeHeader(&header, FCGI_PARAMS, 0, 0);
    data.append((char*)&header, FCGI_HEADER_LEN);
}

void HttpModelFcgi::makeStdinRecord(
    char* data_,
    int len,
    std::string& data)
{
    int cl = len, pl;
    char padding_buf[8] = { 0 };

    while (len > 0) {

        if (len > FCGI_MAX_LENGTH) {
            cl = FCGI_MAX_LENGTH;
        }

        // Count padding length.
        pl = (cl % 8) == 0 ? 0 : 8 - (cl % 8);

        fcgi_header_t header;
        makeHeader(&header, FCGI_STDIN, cl, pl);
        data.append((char*)&header, FCGI_HEADER_LEN);

        // Send stdin data.
        data.append(data_, cl);

        // Send padding content data.
        if (pl > 0) {
            data.append(padding_buf, pl);
        }

        len -= cl;
        data_ += cl;
    }
}

void HttpModelFcgi::makeEmptyStdinRecord(
    std::string& data)
{
    fcgi_header_t header;

    makeHeader(&header, FCGI_STDIN, 0, 0);
    data.append((char*)&header, FCGI_HEADER_LEN);
}

void HttpModelFcgi::buildFcgiRequest(http_header* hp,
    const std::string& content,
    std::string& data)
{
    int i, l;
    char* buf;

    char* paname[] = {
        "SCRIPT_FILENAME",
        "SCRIPT_NAME",
        "REQUEST_METHOD",
        "REQUEST_URI",
        "QUERY_STRING",
        "CONTENT_TYPE",
        "CONTENT_LENGTH"
    };

    // TODO: add SERVER_SOFTWARE

    int paoffset[] = {
        (size_t) & (((http_header*)0)->filename),
        (size_t) & (((http_header*)0)->name),
        (size_t) & (((http_header*)0)->method),
        (size_t) & (((http_header*)0)->uri),
        (size_t) & (((http_header*)0)->cgiargs),
        (size_t) & (((http_header*)0)->contype),
        (size_t) & (((http_header*)0)->conlength)
    };

    makeBeginRequestRecord(data);

    l = sizeof(paoffset) / sizeof(paoffset[0]);
    for (i = 0; i < l; i++) {

        char* tmp = (char*)((char*)hp + paoffset[i]);
        int len = strlen(tmp);
        printf("[Debug]paname:(%.*s),len:(%d),data:(%.*s)\n",
            strlen(paname[i]), paname[i], len, len, tmp);
        if (len > 0) {

            makeParamsRecord(
                paname[i], strlen(paname[i]),
                tmp, len,
                data);
        }
    }

    makeEmptyParamsRecord(data);

    l = atoi(hp->conlength);
    if (l > 0) {
        buf = (char*)malloc(l + 1);
        memset(buf, '\0', l + 1);
        strcpy(buf, content.c_str());

        makeStdinRecord(buf, l, data);

        free(buf);
    }

    makeEmptyStdinRecord(data);
}

int HttpModelFcgi::parseRecord(
    const std::string& data)
{
    fcgi_header_t header;
    fcgi_end_request_body_t endr;

    char *con_buf = nullptr, *err_buf = nullptr;
    unsigned int con_len = 0, err_len = 0;
    int padding_buf[8] = { 0 };

    unsigned char type;
    unsigned int request_id;
    unsigned int content_length;
    unsigned char padding_length;

    const char* begin = data.c_str();
    unsigned int i = 0;

    while (memcpy((void*)&header, (const void*)(begin + i), sizeof(header))) {

        i += sizeof(header);

        type = header.type;
        request_id = (unsigned int)(header.request_id_hi << 8) + (unsigned int)(header.request_id_lo);
        content_length = (unsigned int)(header.content_length_hi << 8) + (unsigned int)(header.content_length_lo);
        padding_length = header.padding_length;

        printf("type:%d,requestId:%d,contentLength:%d,paddingLength:%d\n",
            type, request_id, content_length, padding_length);

        if (request_id != requestId) {
            LOG(Debug) << "requestId not same,response-id(" << request_id << ")\n";
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
            memcpy(con_buf, (const void*)(begin + i), content_length);
            i += content_length;
            // if (ret == -1 || ret != content_length) {
            // printf("read fcgi_stdout record error\n");
            // return -1;
            // }

            printf("[Debug][fcgi-out]:len(%d),data(%.*s)\n", content_length, content_length, con_buf);
            // TODO: http parser to parse http response.

            if (padding_length > 0) {
                memcpy(padding_buf, (const void*)(begin + i), padding_length);
                i += padding_length;
                // if (ret == -1 || ret != padding_length) {
                // printf("read fcgi_stdout padding error\n");
                // return -1;
                // }
                printf("[Debug][fcgi-padding]:len(%d),data(%.*s)\n", padding_length, padding_length, (char*)padding_buf);
            }

        } else if (type == FCGI_STDERR) {

            err_len += content_length;

            if (err_buf != NULL) {
                err_buf = (char*)realloc((void*)err_buf, err_len);
            } else {
                err_buf = (char*)malloc(content_length);
            }

            // FIXME: store date begin last used index.
            memcpy(err_buf, (const void*)(begin + i), content_length);
            i += content_length;
            // if (ret == -1 || ret != content_length) {
            //     return -1;
            // }

            printf("[Debug][fcgi-error]:len(%d),data(%.*s)\n", content_length, content_length, (char*)err_buf);

            if (padding_length > 0) {
                memcpy(padding_buf, (const void*)(begin + i), padding_length);
                i += padding_length;
                // if (ret == -1 || ret != padding_length) {
                // printf("read fcgi_stderr padding error\n");
                // return -1;
                // }
                printf("[Debug][fcgi-padding]:len(%d),data(%.*s)\n", padding_length, padding_length, (char*)padding_buf);
            }

        } else if (type == FCGI_END_REQUEST) {

            memcpy(&endr, (const void*)(begin + i), sizeof(endr));
            i += sizeof(endr);

            // if (ret == -1 || ret != sizeof(endr)) {
            //     free(con_buf);
            //     free(err_buf);
            //     return -1;
            // }

            printf("[Debug]3zhenhuli end\n");
            free(con_buf);
            free(err_buf);
            return 0;
        }

        if (i == data.size()) {
            break;
        }
    }
    return 0;
}

int HttpModelFcgi::parseFcgiResponse(const std::string& data)
{

    if (parseRecord(data) < 0) {
        std::cout << "parse record error\n";
        return -1;
    }

    return 0;
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
