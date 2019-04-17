/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class HttpParser---
 *
 ****************************************
 *
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <tiny_base/log.h>
#include <tiny_http/http.h>
#include <tiny_http/http_time.h>
#include <tiny_http/str_t.h>
#include <tiny_struct/sdstr_t.h>

#include <boost/function.hpp>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <stdio.h>

#define CR '\r'
#define LF '\n'

#define isNum(c) (('0' <= (c) && (c) <= '9'))
#define isAlpha(c) (('a' <= (c) && (c) <= 'z') || ('A' <= (c) && (c) <= 'Z'))
#define isAlphaNum(c) (isNum(c) || isAlpha(c))

#define isLower(c) (('a' <= (c) && (c) <= 'z'))
#define isUpper(c) (('A' <= (c) && (c) <= 'Z'))

#define toLower(c) (isUpper(c) ? (unsigned char)((c) | 0x20) : (c))
#define toUpper(c) (isLower(c) ? (unsigned char)((c)&0xdf) : (c))

#define isHexChar(c) (isNum(c) || ('a' <= toLower(c) && toLower(c) <= 'f'))

#define isMarkChar(c) ((c) == '-' || (c) == '_' || (c) == '.' || (c) == '!' || (c) == '~' || (c) == '*' || (c) == '\'' || (c) == '(' || (c) == ')')
#define isUserInfoChar(c) (isAlphaNum(c) || isMarkChar(c) || (c) == '%' || (c) == ';' || (c) == ':' || (c) == '&' || (c) == '=' || (c) == '+' || (c) == '$' || (c) == ',')

#define isHostChar(c) (isAlphaNum(c) || (c) == '.' || (c) == '-' || (c) == '_')

#define isIpv4Char(c) (isNum(c) || (c) == '.')
#define isIpv6Char(c) (isHexChar(c) || (c) == ':' || (c) == '.')

#define isMimeTypeChar(c) (isAlphaNum(c) || (c) == '/' || (c) == '.' || (c) == '*' || (c) == '-' || (c) == '_' || (c) == '+')

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// It is only used by http method:method only have lower , upper or '-' char.
#define getLetterHashNoCase(c) ((isUpper(c) || isLower(c)) ? (toUpper(c) - 'A') : 26)
// It is only used by http method;
#define getHash(hash, c) ((unsigned long long)((hash)*27 + getLetterHashNoCase(c)))

inline short int getHex(char c)
{
    if ('0' <= c && c <= '9')
        return (c - '0');
    else if ('A' <= c && c <= 'F')
        return (c - 'A' + 10);
    else if ('a' <= c && c <= 'f')
        return (c - 'a' + 10);
    return -1;
}

inline int strncasecmp__(const char* s1, const char* s2, int len)
{
    char c1, c2;
    while (len--) {
        c1 = toLower(*s1);
        c2 = toLower(*s2);
        s1++;
        s2++;
        if (c1 == c2) {
            if (c1)
                continue;
            return 0;
        }
        return c1 - c2;
    }
    return 0;
}

// Status of parse HTTP.
enum state {
    s_error = 1,

    s_start_resp_or_requ,

    s_resp_start,
    s_resp_H,
    s_resp_HT,
    s_resp_HTT,
    s_resp_HTTP,
    s_resp_HTTP_slash,
    s_resp_version_major,
    s_resp_version_dot,
    s_resp_version_minor,
    s_resp_status_code_start,
    s_resp_status_code,
    s_resp_status_phrase_start,
    s_resp_status_phrase,
    s_resp_line_almost_done,
    s_resp_line_done,

    s_requ_start,
    s_requ_method_start,
    s_requ_method,
    s_requ_url_begin,
    s_requ_url, // add
    s_requ_schema,
    s_requ_schema_slash,
    s_requ_schema_slash_slash,
    s_requ_server_start,
    s_requ_server,
    s_requ_path,
    s_requ_server_at,
    s_requ_query_string_start,
    s_requ_query_string,
    s_requ_fragment_start,
    s_requ_fragment,

    s_requ_HTTP_start,
    s_requ_H,
    s_requ_HT,
    s_requ_HTT,
    s_requ_HTTP,
    s_requ_HTTP_slash,
    s_requ_version_major,
    s_requ_version_dot,
    s_requ_version_minor,
    s_requ_line_almost_done,
    s_requ_line_done,

    // parse header statue
    s_header_start,
    s_header,
    s_header_almost_done,
    s_header_done,
    s_headers_almost_done,
    s_headers_done,

    s_body_start,
    s_body,
    s_body_done,

    s_chunk

};

// Status of parse host.
enum http_host_state {
    s_http_host_error = 1,
    s_http_userinfo_start,
    s_http_userinfo,
    s_http_host_start,
    s_http_host_v6_start,
    s_http_host_v4,
    s_http_host_v6,
    s_http_host_v6_end,
    s_http_host_v6_zone_start,
    s_http_host_v6_zone,
    s_http_host_port_start,
    s_http_host_port
};

// Status of parse header.
enum http_header_state {
    s_http_header_error = 1,

    s_http_header_start,
    s_http_header_key_start,
    s_http_header_key,
    s_http_header_colon,
    s_http_header_space,
    s_http_header_value_start,
    s_http_header_value,
    s_http_header_almost_done,
    s_http_header_done,
    s_http_headers_almost_done,
    s_http_headers_done,
};

// Types of body.
enum http_body_type {
    t_http_body_type_init = 1,
    t_http_body_end_by_length,
    t_http_body_end_by_eof,
    t_http_body_chunked,
    t_http_body_skip,
};

// Status of parse body.
enum http_body_state {
    s_http_body_error = 1,

    s_http_body_identify_by_length,
    s_http_body_identify_by_eof,

    s_http_body_chunk_size_start,
    s_http_body_chunk_size,
    s_http_body_chunk_size_almost_done,
    s_http_body_chunk_size_done,

    s_http_body_chunk_data,
    s_http_body_chunk_data_almost_done,
    s_http_body_chunk_data_done,
    s_http_body_chunks_done
};

enum httpUrlField {
    HTTP_UF_SCHEMA = 0,
    HTTP_UF_HOST = 1,
    HTTP_UF_PORT = 2,
    HTTP_UF_PATH = 3,
    HTTP_UF_QUERY = 4,
    HTTP_UF_FRAGMENT = 5,
    HTTP_UF_USERINFO = 6,
    HTTP_UF_MAX = 7
};

typedef struct Url {
    char* data;
    unsigned int len;

    unsigned int port : 16;
    unsigned int field_set : 16;

    struct
    {
        unsigned int offset : 16;
        unsigned int len : 16;
    } fields[HTTP_UF_MAX];

} Url;

inline void urlInit(Url* url)
{
    memset(url, 0, sizeof(Url));
    url->data = nullptr;
}

enum httpHeaderField {
    HTTP_HF_SCHEMA = 0,
    HTTP_HF_HOST = 1,
    HTTP_HF_KEEP_CONNECTION = 2,
    HTTP_HF_CONTENT_LENGTH = 3,
    HTTP_HF_UPGRADE = 4,
    HTTP_HF_TRANSFER_ENCODING_CHUNKED = 5,
    HTTP_HF_USERINFO = 6,
    HTTP_HF_MAX = 7
};

typedef struct HttpHeader {
    unsigned int keyHash;
    // TODO: update to sdstr_t
    Str key;
    Str value;
} HttpHeader;

inline void httpHeaderInit(HttpHeader* header)
{
    if (header == nullptr) {
        return;
    }
    header->keyHash = 0;
    setStrNull(&(header->key));
    setStrNull(&(header->value));
}

typedef struct HttpHeaders {
    HttpHeader* host;
    HttpHeader* connection;
    HttpHeader* if_modified_since;
    HttpHeader* if_unmodified_since;
    HttpHeader* user_agent;
    HttpHeader* referer;

    HttpHeader* content_length;
    HttpHeader* content_type;
    HttpHeader* transfer_encoding;
    HttpHeader* accept_encoding;

    HttpHeader* upgrade;
    HttpHeader* expect;

    HttpHeader* cookie;
    HttpHeader* last_modified;

    HttpHeader* x_powered_by;

    HttpHeader* range;
    HttpHeader* if_range;
    HttpHeader* content_range;

    std::list<HttpHeader*> generals; //TODO:take place in list_t

    char* data;
    unsigned int offset;
    unsigned int len;

    unsigned int content_length_n;

    unsigned int valid_content_length : 1;
    unsigned int valid_host : 1;
    unsigned int valid_referer : 1;

    unsigned int connection_keep_alive : 1;
    unsigned int connection_close : 1;
    unsigned int connection_upgrade : 1;

    unsigned int chrome : 1;
    unsigned int firefox : 1;
    unsigned int ie : 1;
    unsigned int safari : 1;
    unsigned int opera : 1;
    unsigned int unknow : 1;

    unsigned int content_identify_length : 1;
    unsigned int content_identify_eof : 1;
    unsigned int chunked : 1;

    unsigned int has_upgrade : 1;

    unsigned int valid_if_modified_since : 1;
    unsigned int valid_if_unmodified_since : 1;
    unsigned int valid_last_modified : 1;

    time_t time_if_modified_since;
    time_t time_if_unmodified_since;
    time_t time_last_modified;

    //TODO:more information
} HttpHeaders;

typedef struct TinyWebCacheHeaders {

    HttpHeader* tinyweb_cache_date;
    HttpHeader* tinyweb_cache_location;
    HttpHeader* tinyweb_cache_access;
    HttpHeader* tinyweb_cache_body_offset;

}TinyWebCacheHeaders;

inline void httpHeadersInit(HttpHeaders* headers)
{
    if (headers == nullptr) {
        return;
    }
    // httpHeaderInit(headers->host);
    // httpHeaderInit(headers->connection);
    // httpHeaderInit(headers->if_modified_since);
    // httpHeaderInit(headers->if_unmodified_since);
    // httpHeaderInit(headers->user_agent);
    // httpHeaderInit(headers->referer);

    // httpHeaderInit(headers->content_length);
    // httpHeaderInit(headers->content_type);
    // httpHeaderInit(headers->transfer_encoding);
    // httpHeaderInit(headers->accept_encoding);

    // httpHeaderInit(headers->upgrade);
    // httpHeaderInit(headers->expect);

    // httpHeaderInit(headers->cookie);
    // httpHeaderInit(headers->last_modified);

    // httpHeaderInit(headers->x_powered_by);

    // httpHeaderInit(headers->range);
    // httpHeaderInit(headers->if_range);
    // httpHeaderInit(headers->content_range);

    headers->data = nullptr;
    headers->offset = 0;
    headers->len = 0;

    headers->content_length_n = 0;

    headers->valid_content_length = 0;
    headers->valid_host = 0;
    headers->valid_referer = 0;

    headers->connection_keep_alive = 0;
    headers->connection_close = 0;
    headers->connection_upgrade = 0;

    headers->chrome = 0;
    headers->firefox = 0;
    headers->ie = 0;
    headers->safari = 0;
    headers->opera = 0;
    headers->unknow = 0;

    headers->content_identify_length = 0;
    headers->content_identify_eof = 0;
    headers->chunked = 0;

    headers->has_upgrade = 0;

    headers->valid_if_modified_since = 0;
    headers->valid_if_unmodified_since = 0;
    headers->valid_last_modified = 0;

    headers->time_if_modified_since = 0;
    headers->time_if_unmodified_since = 0;
    headers->time_last_modified = 0;

}

void pushHeader(HttpHeaders* headers,
    HttpHeader* header,
    unsigned int key_hash);

typedef struct HttpBody {
    void* data;
    unsigned int offset;
    unsigned int len;
} HttpBody;

void printHttpHeaders(const HttpHeaders* headers);

void printUrl(const Url* url);

void printBody(const HttpBody* body);

enum HttpContentType {
    HTTP_TYPE_BOTH,
    HTTP_TYPE_REQUEST,
    HTTP_TYPE_RESPONSE,
    HTTP_TYPE_FCGI_RESPONSE,
};

typedef struct HttpRequest {
    HttpContentType type;

    unsigned short method : 8;
    unsigned short http_version_major : 8;
    unsigned short http_version_minor : 8;

    unsigned int statusCode;
    std::string statusPhrase;

    std::string method_s;

    unsigned short headerNum;

    enum http_body_type body_type;

    Url* url;
    HttpHeaders* headers;
    HttpBody* body;

} HttpRequest;

inline bool endMessageByEof(const HttpRequest* request)
{
    HttpHeaders* hs = request->headers;
    unsigned int status_code = request->statusCode;
    enum http_body_type body_type = request->body_type;

    if (hs->valid_content_length || hs->chunked)
        return false; //Don't need eof

    /* See RFC 2616 section 4.4 */
    if (status_code / 100 == 1 ||        /* 1xx e.g. Continue */
        status_code == 204 ||            /* No Content */
        status_code == 304 ||            /* Not Modified */
        body_type == t_http_body_skip) { /* response to a HEAD request */
                                         // No body message.
        return false;
    }

    return true;
}

inline bool shouldKeepAlive(const HttpRequest* request)
{
    if (request->http_version_major >= 1 && request->http_version_minor >= 1) {
        //HTTP/1.1
        if (request->headers->connection_close)
            return false;
    } else {
        if (request->headers->connection_keep_alive)
            return true;
    }

    if (!endMessageByEof(request))
        return true;
}

typedef boost::function<int()> HttpCallback;
typedef boost::function<int()> HttpDataCallback;

class HttpParserSettings {
private:
    std::map<std::string, HttpCallback> m_nReflection;

    HttpCallback m_fGetMessage;
    HttpCallback m_fGetUrl;
    HttpCallback m_fGetStatus;
    HttpCallback m_fGetRequestLine;

    HttpCallback m_fGetHeader;
    HttpCallback m_fHeaderKey;
    HttpCallback m_fHeaderValue;

    HttpCallback m_fGetBody;
    HttpCallback m_fEndMessage;

    HttpCallback m_fGetChunk;
    HttpCallback m_fEndChunk;

public:
    HttpParserSettings()
    {
        // std::cout << "class HttpParserSettings constructor\n";
    }

    void setGetMessageCallback(const HttpCallback& c)
    {
        m_fGetMessage = c;
        m_nReflection["getMessage"] = c;
    }

    void setGetRequestLineCallback(const HttpCallback& c)
    {
        m_fGetRequestLine = c;
        m_nReflection["getRequestLine"] = c;
    }

    void setGetHeaderCallback(const HttpCallback& c)
    {
        m_fGetHeader = c;
        m_nReflection["getHeader"] = c;
    }

    void setGetBodyCallback(const HttpCallback& c)
    {
        m_fGetBody = c;
        m_nReflection["getBody"] = c;
    }

    void setGetEndMessageCallback(const HttpCallback& c)
    {
        m_fEndMessage = c;
        m_nReflection["endMessage"] = c;
    }

    HttpCallback getMethodByName(const std::string& fname)
    {
        //auto p = m_nReflection.find(fname);
        // if (p == m_nReflection.end())
        // 	return nullptr;
        // else
        return m_nReflection[fname];
        //FIXME:
    }

    ~HttpParserSettings()
    {
        // std::cout << "class HttpParserSettings destructor\n";
    }

    friend class HttpParser;
};

class HttpParser {
private:
    HttpParserSettings* m_pSettings;

    unsigned int m_nType : 2; //Http request or response
    unsigned int m_nFlags : 8;
    unsigned int m_nState;
    unsigned int m_nHeaderState;

    unsigned long long m_nRead;
    unsigned long long m_nContentLength;

    unsigned short m_nHttpVersionMajor;
    unsigned short m_nHttpVersionMinor;
    unsigned int m_nStatusCode : 16;
    unsigned int m_nMethod : 8;
    unsigned int m_nErrno : 7;
    unsigned int m_nIsUpgrade : 1;

    void* m_pData;

public:
    HttpParser(HttpParserSettings* set = nullptr)
        : m_pSettings(set)
        , m_nType(0)
        , //FIXME:
        m_nFlags(0)
        , m_nState(0)
        , m_nHeaderState(0)
        , m_nRead(0)
        , m_nContentLength(0)
        , m_nHttpVersionMajor(0)
        , m_nHttpVersionMinor(0)
        , m_nStatusCode(0)
        , m_nMethod(0)
        , m_nErrno(0)
        , m_nIsUpgrade(0)
        , m_pData(nullptr)
    {
        LOG(Debug) << "class HttpParser constructor\n";
    }

    void setType(enum HttpContentType type);

    enum http_errno getErrno()
    {
        return (enum http_errno)m_nErrno;
    }

    int invokeByName(const char* funName,
        const char* data,
        unsigned int len);

    //parse host
    enum http_host_state parseHostChar(const char ch,
        enum http_host_state s);
    int parseHost(const char* stream,
        unsigned int at,
        unsigned int len,
        Url* result,
        bool has_at_char);

    //parse url
    enum state parseUrlChar(const char ch,
        enum state s);
    int parseUrl(const char* stream,
        unsigned int len,
        Url* result);

    //parse header
    enum http_header_state parseHeaderChar(const char ch,
        enum http_header_state s);
    int parseHeader(const char* stream,
        unsigned int& at,
        unsigned int len,
        HttpHeader* result);
    int parseHeaders(const char* stream,
        unsigned int at,
        unsigned int len,
        HttpHeaders* result);

    int parseHeadersMeaning(HttpHeaders* headers);
    int semanticAnalysisHttp(HttpRequest* request);
    int semanticAnalysisFcgiResponse(HttpRequest* request);

    //parse body
    int parseBody(const char* stream,
        unsigned int at,
        unsigned int len,
        enum http_body_type body_type,
        unsigned int content_length_n);

    int execute(const char* stream,
        unsigned int at,
        unsigned int len,
        HttpRequest* request);

    ~HttpParser()
    {
        LOG(Debug) << "class HttpParser destructor\n";
    }
};

// Callbacks for parse header meaning.

typedef boost::function<int(const Str*, HttpHeaders* const)> headerFun;
typedef boost::function<int(const std::string&, HttpHeaders* const)> deheaderFun;

inline int parseHostValue(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]host\n";
    // TODO: No need to check the host value is valid or not???
    headers->valid_host = 1;
    return 0;
}

inline int builtHostValue(const std::string& s, HttpHeaders* const headers)
{

    return 0;
}

inline int parseConnectionValue(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]connetion\n";
    if (0 == strncasecmp__(s->data, "keep-alive", s->len)) {
        headers->connection_keep_alive = 1;
        // std::cout << "connection keep alive\n";
    } else if (0 == strncasecmp__(s->data, "close", s->len)) {
        headers->connection_close = 1;
        // std::cout << "connection close\n";
    } else if (0 == strncasecmp__(s->data, "upgrade", s->len)) {
        headers->connection_upgrade = 1;
        // std::cout << "connection upgrade\n";
    } else {
        // std::cout << "connection field is other value\n";
        return -1;
    }
    return 0;
}

inline int parseContentLength(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]content-length\n";
    unsigned int res = 0;
    char* p = s->data;
    for (unsigned int i = 0; i < s->len; i++) {
        char ch = *(p + i);
        if (!isNum(ch)) {
            std::cout << "Content-Length value invalid\n";
            return -1;
        }
        res *= 10;
        res += ch - '0';
    }

    std::cout << "content-length:" << res << std::endl;
    headers->content_length_n = res;
    headers->valid_content_length = 1;
    headers->content_identify_length = 1;

    //std::cout << "parse Content-Length:" << headers->content_length_n << std::endl;
    return 0;
}

inline int parseIfModifiedSince(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]if-modified-since\n";
    int return_val;
    sdstr tmp;
    sdsnnew(&tmp, s->data, s->len);

    return_val = deformatHttpTime(&tmp, &(headers->time_if_modified_since));
    if (return_val == 0) {
        headers->valid_if_modified_since = 1;
        std::cout << "if-modified-since value is valid\n";
    }
    sdsdestory(&tmp);

    return 0;
}

inline int parseIfUnmodifiedSince(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]if-unmodified-since\n";
    // TODO: using http_time.h
    int return_val;
    sdstr tmp;
    sdsnnew(&tmp, s->data, s->len);

    return_val = deformatHttpTime(&tmp, &(headers->time_if_unmodified_since));
    if (return_val == 0) {
        headers->valid_if_unmodified_since = 1;
        std::cout << "if-unmodified-since value is valid\n";
    }
    sdsdestory(&tmp);

    return 0;
}

inline int parseUserAgent(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]user-agent:\n";
    // TODO:
    headers->chrome = 1;
    // std::cout << "client brower is chrome\n";
    return 0;
}

inline int parseRefer(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]refer:\n";
    headers->valid_referer = 1;
    return 0;
}

inline int parseTransferEncoding(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]transfer-encoding:\n";
    if (0 == strncasecmp__(s->data, "chunked", s->len)) {
        headers->chunked = 1;
        // std::cout << "chunked\n";
        return 0;
    } else {
        return -1;
    }
}

inline int parseCookie(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]cookie\n";
    // printf("get value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseContentType(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]content-type\n";

    return 0;
}

inline int parseAcceptEncoding(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]accept-encoding\n";

    return 0;
}

inline int parseUpgrade(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]upgrade\n";

    return 0;
}

inline int parseExpect(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]expect\n";

    return 0;
}

inline int parseLastModified(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]last-modified\n";
    int return_val;
    sdstr tmp;
    sdsnnew(&tmp, s->data, s->len);

    return_val = deformatHttpTime(&tmp, &(headers->time_last_modified));
    if (return_val == 0) {
        headers->valid_last_modified = 1;
        std::cout << "last-modified value is valid\n";
    }
    sdsdestory(&tmp);

    return 0;
}

inline int parseXPoweredBy(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]x-powered-by\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseRange(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]range\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseIfRange(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]if-range\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseContentRange(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback]content-range\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

/*parse TinyWeb Cache file header*/
inline int parseTinyWebCacheDate(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback] TinyWeb-Cache: cache-file date\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseTinyWebCacheLocation(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback] TinyWeb-Cache: cache-file location\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseTinyWebCacheAccess(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback] TinyWeb-Cache: cache-file access\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}

inline int parseTinyWebCacheBodyOffset(const Str* s, HttpHeaders* const headers)
{
    std::cout << "[parse headerMeaning callback] TinyWeb-Cache: cache-file body offset\n";
    printf("value:%.*s\n", s->len, s->data);
    return 0;
}
/*parse TinyWeb Cache file header*/

inline int parseValue(const Str* s, HttpHeaders* const headers)
{
    // std::cout << "[parse headerMeaning callback]get value:\n";
    // printf("get value:%.*s\n", s->len, s->data);
    return 0;
}

typedef struct headerCallback {
    Str name;
    size_t offset;
    headerFun fun;
} headerCallback;

//It is used by header key
inline unsigned int JSHash(const char* str, int len)
{
    unsigned int hash = 1315423911;
    // nearly a prime - 1315423911 = 3 * 438474637
    for (int i = 0; i < len; i++)
        hash ^= ((hash << 5) + (toLower(*(str + i)) + (hash >> 2)));
    return (hash & 0x7FFFFFFF);
};

#include <unordered_map>
extern headerCallback headers_in[];
extern std::unordered_map<unsigned int, headerCallback> headerKeyHash;

void headerMeaningInit();

#endif
