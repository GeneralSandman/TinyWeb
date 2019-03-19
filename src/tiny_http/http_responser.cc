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

#include <TinyWebConfig.h>
#include <tiny_base/configer.h>
#include <tiny_base/file.h>
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http.h>
#include <tiny_http/http_model_file.h>
#include <tiny_http/http_parser.h>
#include <tiny_http/http_responser.h>
#include <tiny_struct/sdstr_t.h>

#include <iostream>
#include <string>

void specialResponseBody(enum http_status s, std::string& res)
{
    static std::string param1 = "<html>\r\n"
                                "<head><title>";

    static std::string param2 = "</title></head>\r\n"
                                "<center><h1>";

    static std::string param3 = "</h1></center>\r\n"
                                "<hr><center>" TINYWEB_VERSION "</center>\r\n"
                                "</body>\r\n"
                                "</html>\r\n";

    std::string code(httpStatusCode(s));
    std::string phrase(httpStatusStr(s));
    std::string tmp = code + " " + phrase;

    if (code == "<invalid>") {
        res = "<invalid>";
    } else {
        res = param1 + tmp + param2 + tmp + param3;
    }

    LOG(Debug) << "response special body\n";
}

HttpResponser::HttpResponser()
{
    LOG(Debug) << "class HttpResponser constructor\n";
}

void HttpResponser::buildResponse(const HttpRequest* req, bool valid_requ, HttpResponse* response)
{
    Url* url = req->url;
    sdstr file_path;
    HttpFile* file;
    int file_type;
    int return_val;

    sdsnewempty(&file_path);
    file = &response->file;

    // Get server-configer.
    std::string server_name = "dissigil.cn";
    Configer& configer = Configer::getConfigerInstance();
    ServerConfig server = configer.getServerConfig(server_name);

    sdsncat(&file_path, server.www.c_str(), server.www.size());

    // Get request info and init response.
    if (url->field_set & (1 << HTTP_UF_PATH)) {
        unsigned int off = url->fields[HTTP_UF_PATH].offset;
        unsigned int len = url->fields[HTTP_UF_PATH].len;
        sdsncat(&file_path, url->data + off, len);
    }

    // Init HttpFile struct.
    std::string f(file_path.data, file_path.len);
    file_type = isRegularFile(f);
    if (0 == file_type) {
        // std::cout << "file:\n";
        return_val = file->setFile(f);
    } else if (1 == file_type) {
        // std::cout << "path:\n";
        return_val = file->setPathWithDefault(f, server.indexpage);
        // TODO:if hasn't default index page in , go to sepical file
    } else if (-1 == file_type) {
        // std::cout << "sepical file:\n";
    }

    if (!return_val) {
        std::cout << "++file exit" << std::endl;
    } else {
        std::cout << "--file not exit" << std::endl;
    }

    HttpResponseLine* line = &(response->line);
    line->http_version_major = req->http_version_major;
    line->http_version_minor = req->http_version_minor;
    line->status = HTTP_STATUS_OK;

    HttpResponseHeaders* headers = &(response->headers);
    headers->connection_keep_alive = 0;
    headers->chunked = 0;
    headers->server = 1;

    headers->file_type = file->mime_type;

    if (!return_val) {
        headers->valid_content_length = 1;
    }

    if (headers->valid_content_length) {
        headers->content_length_n = file->info.st_size;
    }

    sdsdestory(&file_path);
}

void HttpResponser::lineToStr(const HttpResponseLine* line, sdstr* line_str)
{
    unsigned int major = line->http_version_major;
    unsigned int minor = line->http_version_minor;
    const char* code = httpStatusCode(line->status);
    const char* phrase = httpStatusStr(line->status);

    sdscatsprintf(line_str, "HTTP/%d.%d %s %s\r\n", major, minor, code, phrase);
}

void HttpResponser::headersToStr(HttpResponseHeaders* headers, sdstr* res)
{
    sdstr tmp;
    sdsnewempty(&tmp, 256); // more efficient

    if (headers->connection_keep_alive) {
        sdscat(&tmp, "Connection: keep-alive\r\n");
    } else if (headers->connection_close) {
        sdscat(&tmp, "Connection: close\r\n");
    }

    if (headers->valid_content_length) {
        sdscatsprintf(&tmp, "Content-Length: %u\r\n", headers->content_length_n);
    }

    if (headers->chunked) {
        sdscat(&tmp, "Transfer-Encoding: chunked\r\n");
    }

    if (headers->server) {
        char* version = TINYWEB_VERSION;
        sdscatsprintf(&tmp, "Server: %s\r\n", version);
    }

    if (headers->file_type.size()) {
        sdscatsprintf(&tmp, "Content-Type: %s\r\n", headers->file_type.c_str());
    }

    sdscatsds(res, &tmp);
    sdscat(res, "\r\n");

    sdsdestory(&tmp);
}

void HttpResponser::bodyToStr(const HttpFile* file, sdstr* body_str)
{
}

void HttpResponser::bodyToChain(HttpFile* file, chain_t* chain)
{
    file->getData(chain);
}

void HttpResponser::bodyToChain(HttpFile* file,
    chain_t* chain,
    enum content_encoding_type cont,
    enum transport_encoding_type trans)
{
    // TODO:
    if (cont == content_gzip_t) {
        LOG(Debug) << "content-encoding: gzip" << std::endl;

    } else if (cont == content_deflate_t) {
        LOG(Debug) << "content-encoding: deflate" << std::endl;

    }


    if (trans == transport_content_length_t) {
        LOG(Debug) << "transport-encoding: content-length" << std::endl;

        // set value of header::content_length_n

    } else if (trans == transport_chunked_t) {
        LOG(Debug) << "transport-encoding: chunked" << std::endl;


    }
}

void HttpResponser::response(const HttpRequest* req, std::string& data)
{
}

HttpResponser::~HttpResponser()
{
    LOG(Debug) << "class HttpResponser destructor\n";
}
