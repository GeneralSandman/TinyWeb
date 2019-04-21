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

HttpBuilder::HttpBuilder(MemoryPool* pool)
    : m_pPool(pool)
    , m_nChunkModel(pool)
    , m_nGzipModel(pool)
    , m_nWriteTailChunk(false)
{
    m_nGzipModel.init();
    LOG(Debug) << "class HttpBuilder constructor\n";
}

void HttpBuilder::buildResponse(const HttpRequest* req, bool valid_requ, HttpResponse* response)
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
        LOG(Debug) << "file(" << f << ") exit\n";
    } else {
        LOG(Debug) << "file(" << f << ") not exit\n";
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
        // TODO: change this value by gzip.
        headers->content_length_n = file->info.st_size;
    }

    sdsdestory(&file_path);
}

void HttpBuilder::lineToStr(const HttpResponseLine* line, sdstr* line_str)
{
    unsigned int major = line->http_version_major;
    unsigned int minor = line->http_version_minor;
    const char* code = httpStatusCode(line->status);
    const char* phrase = httpStatusStr(line->status);

    sdscatsprintf(line_str, "HTTP/%d.%d %s %s\r\n", major, minor, code, phrase);
}

void HttpBuilder::headersToStr(HttpResponseHeaders* headers, sdstr* res)
{
    sdstr tmp;
    sdsnewempty(&tmp, 256); // more efficient

    if (headers->connection_keep_alive) {
        sdscat(&tmp, "Connection: keep-alive\r\n");
    } else if (headers->connection_close) {
        sdscat(&tmp, "Connection: close\r\n");
    }

    if (headers->valid_content_length) {
        // sdscatsprintf(&tmp, "Content-Length: %u\r\n", headers->content_length_n);
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

    // sdscat(&tmp, "Content-Encoding: gzip\r\n");
    sdscat(&tmp, "Transfer-Encoding: chunked\r\n");

    sdscatsds(res, &tmp);
    sdscat(res, "\r\n");

    sdsdestory(&tmp);
}

void HttpBuilder::bodyToStr(const HttpFile* file, sdstr* body_str)
{
}

chain_t* HttpBuilder::bodyToChain(HttpFile* file,
    chain_t* chain,
    enum content_encoding_type cont,
    enum transport_encoding_type trans)
{
    // TODO:
    chain_t* origin = chain;
    chain_t* result = nullptr;
    void* chunklen_address = nullptr;
    unsigned int before_chunked = 0;
    unsigned int after_chunked = 0;

    if (cont == content_gzip_t) {
        chain = m_nGzipModel.getOutputChain();
    }

    clearData(chain);
    clearData(origin);

    // --------

    if (trans == transport_no_t) {
        LOG(Debug) << "transport-encoding: no" << std::endl;

    } else if (trans == transport_content_length_t) {
        LOG(Debug) << "transport-encoding: content-length" << std::endl;

        // set value of header::content_length_n

    } else if (trans == transport_chunked_t) {
        LOG(Debug) << "transport-encoding: chunked" << std::endl;

        chunklen_address = m_nChunkModel.chunked_begin(chain);
        before_chunked = countAllDataSize(chain);
    }

    // -----------

    if (cont == content_no_t) {
        LOG(Debug) << "content-encoding: no" << std::endl;
        if (!file->noMoreData()) {
            file->getData(chain);
        }

        result = chain;

    } else if (cont == content_gzip_t) {
        LOG(Debug) << "content-encoding: gzip" << std::endl;

        if (!file->noMoreData()) {
            chain_t* output = nullptr;

            LOG(Debug) << "all buffer-size:" << countAllBufferSize(origin)
                       << ",all data-size:" << countAllDataSize(origin) << std::endl;

            file->getData(origin);

            bool endData = file->noMoreData();
            LOG(Debug) << "zhenhuli:" << endData << std::endl;
            gzip_status res = m_nGzipModel.compress(origin, output, endData);
            if (res == gzip_error) {
                LOG(Debug) << "compress error\n";
            }

            result = output;
        } else {
            result = chain;
        }

    } else if (cont == content_deflate_t) {
        LOG(Debug) << "content-encoding: deflate" << std::endl;
    }

    // ------------

    if (trans == transport_no_t) {
        LOG(Debug) << "transport-encoding: no" << std::endl;

    } else if (trans == transport_content_length_t) {
        LOG(Debug) << "transport-encoding: content-length" << std::endl;

        // set value of header::content_length_n

    } else if (trans == transport_chunked_t) {
        LOG(Debug) << "transport-encoding: chunked" << std::endl;

        after_chunked = countAllDataSize(result);
        m_nChunkModel.chunked_end(result, chunklen_address, after_chunked - before_chunked);

        if (after_chunked - before_chunked == 0) {
            LOG(Debug) << "tail chunk\n";
            if (content_gzip_t == cont) {
                if (file->noMoreData()) {
                    LOG(Debug) << "empty chain && file noMoreData, is tail chunk\n";
                    m_nWriteTailChunk = true;
                } else {
                    LOG(Debug) << "empty chain between compress, clearData && don't send\n";
                    // result = nullptr;
                    clearData(result);
                }
            } else {
                m_nWriteTailChunk = true;
            }
        }
    }

    return result;
    // -----------
}

bool HttpBuilder::noMoreBody(HttpFile* file,
    chain_t* chain,
    enum content_encoding_type cont,
    enum transport_encoding_type trans)
{
    bool res = false;
    if (trans == transport_chunked_t) {
        res = file->noMoreData() && m_nWriteTailChunk;
        LOG(Debug) << "chunked, judge by file->noMoreData() && writeTailChunk,value(" << res << ")\n";
    } else {
        res = file->noMoreData();
        LOG(Debug) << "no-chunked, judge by file->noMoreData(),value(" << res << ")\n";
    }

    return res;
}

void HttpBuilder::response(const HttpRequest* req, std::string& data)
{
    // No use.
}

/* request */
void HttpBuilder::buildRequest(const HttpRequest* req_client, HttpRequest* req_server)
{
    if (nullptr == req_client || nullptr == req_server) {
        return;
    }

    req_server->type = HTTP_TYPE_REQUEST;
    req_server->method = req_client->method;
    req_server->http_version_major = req_client->http_version_major;
    req_server->http_version_minor = req_client->http_version_minor;

    req_server->method_s = std::string(httpMethodStr((enum http_method)(req_server->method)));

    req_server->headerNum = 0;
    req_server->url = req_client->url;
    // req_server->headers = nullptr;
    req_server->body = nullptr;

    if (nullptr != req_server->headers) {
        HttpHeaders *headers = req_server->headers;
        headers->connection_keep_alive = 1;
        headers->chrome = 1;
    }
}

void HttpBuilder::lineToStr(const HttpRequest* req, sdstr* line_str)
{
    if (nullptr == req || nullptr == line_str) {
        return;
    }

    char* url_begin = req->url->data;
    unsigned int url_len = req->url->len;

    sdscatsprintf(line_str, "%s %.*s HTTP/%d.%d\r\n",
        req->method_s.c_str(),
        url_len, url_begin,
        req->http_version_major, req->http_version_minor);
}

void HttpBuilder::requestHeadersToStr(const HttpHeaders* headers, sdstr* res)
{
    if (nullptr == headers || nullptr == res) {
        return;
    }

    sdstr tmp;
    sdsnewempty(&tmp, 256);

    if (headers->connection_keep_alive) {
        sdscat(&tmp, "Connection: keep-alive\r\n");
    } else if (headers->connection_close) {
        sdscat(&tmp, "Connection: close\r\n");
    }

    if (headers->chrome) {

        sdscat(&tmp, "User-Agent: Mozilla/5.0 "
                     "(compatible; MSIE 9.0; "
                     "Windows NT 6.1; WOW64; Trident/5.0)"
                     "\r\n");

    } else if (headers->firefox) {

        sdscat(&tmp, "User-Agent: Mozilla/5.0 "
                     "(Windows NT 6.2; Win64; x64; rv:16.0.1) "
                     "Gecko/20121011 Firefox/16.0.1"
                     "\r\n");

    } else if (headers->ie) {

        sdscat(&tmp, "User-Agent: Mozilla/5.0 "
                     "(Windows; U; MSIE 9.0; Windows NT 9.0; en-US);"
                     "\r\n");

    } else if (headers->safari) {

        sdscat(&tmp, "User-Agent: Mozilla/5.0 "
                     "(iPad; CPU OS 6_0 like Mac OS X) "
                     "AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 "
                     "Mobile/10A5355d Safari/8536.25"
                     "\r\n");

    } else if (headers->opera) {

        sdscat(&tmp, "User-Agent: Opera/9.80 "
                     "(X11; Linux i686; U; ru) "
                     "Presto/2.8.131 Version/11.11"
                     "\r\n");

    } else if (headers->unknow) {
    }

    sdscatsds(res, &tmp);
    sdscat(res, "\r\n");

    sdsdestory(&tmp);
}

void HttpBuilder::bodyToStr2(const HttpFile* file, sdstr* body_str)
{
}

HttpBuilder::~HttpBuilder()
{
    LOG(Debug) << "class HttpBuilder destructor\n";
}
