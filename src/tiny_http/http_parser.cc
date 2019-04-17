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

#include <tiny_base/log.h>
#include <tiny_http/http.h>
#include <tiny_http/http_parser.h>

#include <boost/bind.hpp>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

void printHttpHeaders(const HttpHeaders* headers)
{
    std::cout << "<++++++++++[Debug]-HttpHeaders Information++++++++++>" << std::endl;

    printf("valid_host:%d\n", headers->valid_host);
    printf("valid_referer:%d\n", headers->valid_referer);
    printf("connection_keep_alive:%d\n", headers->connection_keep_alive);
    printf("connection_close:%d\n", headers->connection_close);
    printf("chrome:%d\n", headers->chrome);
    printf("content_identify_length:%d\n", headers->content_identify_length);
    printf("content_identify_eof:%d\n", headers->content_identify_eof);
    printf("chunked:%d\n", headers->chunked);

    std::cout << "++++special header+++\n";
    if (headers->host != nullptr) {
        printStr(&(headers->host->key));
        printStr(&(headers->host->value));
    }
    if (headers->connection != nullptr) {
        printStr(&(headers->connection->key));
        printStr(&(headers->connection->value));
    }
    if (headers->if_modified_since != nullptr) {
        printStr(&(headers->if_modified_since->key));
        printStr(&(headers->if_modified_since->value));
    }
    if (headers->if_unmodified_since != nullptr) {
        printStr(&(headers->if_unmodified_since->key));
        printStr(&(headers->if_unmodified_since->value));
    }
    if (headers->user_agent != nullptr) {
        printStr(&(headers->user_agent->key));
        printStr(&(headers->user_agent->value));
    }
    if (headers->referer != nullptr) {
        printStr(&(headers->referer->key));
        printStr(&(headers->referer->value));
    }
    if (headers->content_length != nullptr) {
        printStr(&(headers->content_length->key));
        printStr(&(headers->content_length->value));
    }
    if (headers->content_type != nullptr) {
        printStr(&(headers->content_type->key));
        printStr(&(headers->content_type->value));
    }
    if (headers->transfer_encoding != nullptr) {
        printStr(&(headers->transfer_encoding->key));
        printStr(&(headers->transfer_encoding->value));
    }
    if (headers->accept_encoding != nullptr) {
        printStr(&(headers->accept_encoding->key));
        printStr(&(headers->accept_encoding->value));
    }
    if (headers->upgrade != nullptr) {
        printStr(&(headers->upgrade->key));
        printStr(&(headers->upgrade->value));
    }
    if (headers->expect != nullptr) {
        printStr(&(headers->expect->key));
        printStr(&(headers->expect->value));
    }
    if (headers->cookie != nullptr) {
        printStr(&(headers->cookie->key));
        printStr(&(headers->cookie->value));
    }
    if (headers->last_modified != nullptr) {
        printStr(&(headers->last_modified->key));
        printStr(&(headers->last_modified->value));
    }
    std::cout << "+++++generals+++++\n";
    for (auto t : headers->generals) {

        printStr(&(t->key));
        printStr(&(t->value));
    }
    std::cout << "<+++++++++++++++++++++++++++++++++++++++++++++++++++>" << std::endl;
}

void printUrl(const Url* url)
{
    std::cout << "<++++++++++[Debug]-Url Information++++++++++>" << std::endl;

    if (url->field_set & (1 << HTTP_UF_SCHEMA)) {
        int off = url->fields[HTTP_UF_SCHEMA].offset;
        int len = url->fields[HTTP_UF_SCHEMA].len;
        printf("schema:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_HOST)) {
        int off = url->fields[HTTP_UF_HOST].offset;
        int len = url->fields[HTTP_UF_HOST].len;
        printf("host:%.*s\n", len, (const char*)url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_PORT)) {
        int off = url->fields[HTTP_UF_PORT].offset;
        int len = url->fields[HTTP_UF_PORT].len;
        printf("port:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_PATH)) {
        int off = url->fields[HTTP_UF_PATH].offset;
        int len = url->fields[HTTP_UF_PATH].len;
        printf("path:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_QUERY)) {
        int off = url->fields[HTTP_UF_QUERY].offset;
        int len = url->fields[HTTP_UF_QUERY].len;
        printf("query:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_FRAGMENT)) {
        int off = url->fields[HTTP_UF_FRAGMENT].offset;
        int len = url->fields[HTTP_UF_FRAGMENT].len;
        printf("fragment:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_USERINFO)) {
        int off = url->fields[HTTP_UF_USERINFO].offset;
        int len = url->fields[HTTP_UF_USERINFO].len;
        printf("userinfo:%.*s\n", len, url->data + off);
    }
    std::cout << "<+++++++++++++++++++++++++++++++++++++++++++>" << std::endl;
}

void printBody(const HttpBody* body)
{
    std::cout << "<++++++++++[Debug]-body Information+++++++++>" << std::endl;

    std::cout << "<+++++++++++++++++++++++++++++++++++++++++++>" << std::endl;
}

#define checkOrGoError(con) \
    do {                    \
        if (!con) {         \
            goto error;     \
        }                   \
    } while (0);

#define offsetof__(s, m) (size_t) & (((s*)0)->m)

headerCallback headers_in[] = {
    {
        .name = Str("host"),
        .offset = offsetof__(HttpHeaders, host),
        .fun = boost::bind(parseHostValue, _1, _2),
    },

    {
        .name = Str("connection"),
        .offset = offsetof__(HttpHeaders, connection),
        .fun = boost::bind(parseConnectionValue, _1, _2),
    },

    {
        .name = Str("content-length"),
        .offset = offsetof__(HttpHeaders, content_length),
        .fun = boost::bind(parseContentLength, _1, _2),
    },

    {
        .name = Str("if-modified-since"),
        .offset = offsetof__(HttpHeaders, if_modified_since),
        .fun = boost::bind(parseIfModifiedSince, _1, _2),
    },

    {
        .name = Str("if-unmodified-since"),
        .offset = offsetof__(HttpHeaders, if_unmodified_since),
        .fun = boost::bind(parseIfUnmodifiedSince, _1, _2),
    },

    {
        .name = Str("user-agent"),
        .offset = offsetof__(HttpHeaders, user_agent),
        .fun = boost::bind(parseUserAgent, _1, _2),
    },

    {
        .name = Str("referer"),
        .offset = offsetof__(HttpHeaders, referer),
        .fun = boost::bind(parseRefer, _1, _2),
    },

    {
        .name = Str("transfer-encoding"),
        .offset = offsetof__(HttpHeaders, transfer_encoding),
        .fun = boost::bind(parseTransferEncoding, _1, _2),
    },

    {
        .name = Str("cookie"),
        .offset = offsetof__(HttpHeaders, cookie),
        .fun = boost::bind(parseCookie, _1, _2),
    },

    {
        .name = Str("content-type"),
        .offset = offsetof__(HttpHeaders, content_type),
        .fun = boost::bind(parseContentType, _1, _2),
    },

    {
        .name = Str("accept-encoding"),
        .offset = offsetof__(HttpHeaders, accept_encoding),
        .fun = boost::bind(parseAcceptEncoding, _1, _2),
    },

    {
        .name = Str("upgrade"),
        .offset = offsetof__(HttpHeaders, upgrade),
        .fun = boost::bind(parseUpgrade, _1, _2),
    },

    {
        .name = Str("expect"),
        .offset = offsetof__(HttpHeaders, expect),
        .fun = boost::bind(parseExpect, _1, _2),
    },

    {
        .name = Str("last-modified"),
        .offset = offsetof__(HttpHeaders, last_modified),
        .fun = boost::bind(parseLastModified, _1, _2),
    },

    {
        .name = Str("x-powered-by"),
        .offset = offsetof__(HttpHeaders, x_powered_by),
        .fun = boost::bind(parseXPoweredBy, _1, _2),
    },

    {
        .name = Str("range"),
        .offset = offsetof__(HttpHeaders, range),
        .fun = boost::bind(parseRange, _1, _2),
    },

    {
        .name = Str("if-range"),
        .offset = offsetof__(HttpHeaders, if_range),
        .fun = boost::bind(parseIfRange, _1, _2),
    },

    {
        .name = Str("content-range"),
        .offset = offsetof__(HttpHeaders, content_range),
        .fun = boost::bind(parseContentRange, _1, _2),
    },
};

/*parse TinyWeb Cache file header*/
headerCallback headers_in_cache[] = {
    {
        .name = Str("TinyWeb-Cache-Date"),
        .offset = offsetof__(TinyWebCacheHeaders, tinyweb_cache_date),
        .fun = boost::bind(parseTinyWebCacheDate, _1, _2),
    },

    {
        .name = Str("TinyWeb-Cache-Location"),
        .offset = offsetof__(TinyWebCacheHeaders, tinyweb_cache_location),
        .fun = boost::bind(parseTinyWebCacheLocation, _1, _2),
    },

    {
        .name = Str("TinyWeb-Cache-Access"),
        .offset = offsetof__(TinyWebCacheHeaders, tinyweb_cache_access),
        .fun = boost::bind(parseTinyWebCacheAccess, _1, _2),
    },

    {
        .name = Str("TinyWeb-Cache-Body-Offset"),
        .offset = offsetof__(TinyWebCacheHeaders, tinyweb_cache_body_offset),
        .fun = boost::bind(parseTinyWebCacheBodyOffset, _1, _2),
    },
};

std::unordered_map<unsigned int, headerCallback> headerKeyHash;

void headerMeaningInit()
{
    int len = ARRAY_SIZE(headers_in);
    for (int i = 0; i < len; i++) {
        unsigned int hash = JSHash(headers_in[i].name.data, headers_in[i].name.len);
        headerKeyHash[hash] = headers_in[i];
        std::cout << "add special header callback(" << headers_in[i].name.data << ")\n";
    }
}

/*init TinyWeb Cache file header*/
void headerMeaningInit_cache()
{
    int len = ARRAY_SIZE(headers_in_cache);
    for (int i = 0; i < len; i++) {
        unsigned int hash = JSHash(headers_in_cache[i].name.data, headers_in_cache[i].name.len);
        headerKeyHash[hash] = headers_in_cache[i];
        std::cout << "add special(TinyWeb-Cache) header callback(" << headers_in_cache[i].name.data << ")\n";
    }
}

void HttpParser::setType(enum HttpContentType type)
{
    m_nType = type;

    switch (type) {

    case HTTP_TYPE_BOTH: {
        m_nState = s_start_resp_or_requ;
    } break;
    case HTTP_TYPE_REQUEST: {
        m_nState = s_requ_start;
    } break;
    case HTTP_TYPE_RESPONSE: {
        m_nState = s_resp_start;
    } break;
    case HTTP_TYPE_FCGI_RESPONSE: {
        m_nState = s_requ_line_done;
    } break;
    }
}

int HttpParser::invokeByName(const char* funName,
    const char* data,
    unsigned int len)
{
    if (nullptr == m_pSettings) {
        LOG(Warn) << "HttpParser have invalid HttpParserSettings\n";
        return -1;
    }

    std::string fname(funName);
    HttpCallback fun = m_pSettings->getMethodByName(fname);
    if (fun == nullptr) {
        LOG(Warn) << "HttpParser invoke invalid header-parse-callback\n";
        return -1;
    }

    return fun();
}

enum http_host_state HttpParser::parseHostChar(const char ch,
    enum http_host_state stat)
{
    switch (ch) {
    // Invalid char in url
    case '\r':
    case '\n':
    case '\t':
    case '\a':
    case '\f':
    case ' ':
        return s_http_host_error;
        break;

    default:
        break;
    }

    switch (stat) {
    case s_http_userinfo_start: //http://@hostname/ is vaild
    case s_http_userinfo:
        if (ch == '@')
            return s_http_host_start;
        else if (isUserInfoChar(ch))
            return s_http_userinfo;
        break;

    case s_http_host_start:
        if (ch == '[') //is IPv6
            return s_http_host_v6_start;
        else if (isHostChar(ch)) //is IPv4
            return s_http_host_v4;
        break;

    case s_http_host_v6_start:
        //not finished
        if (isIpv6Char(ch))
            return s_http_host_v6;
        else if (ch == ']') //http://[]/ is invaild
            return s_http_host_error;
        else if (ch == '%')
            return s_http_host_error; //http://[%eth]/ is invaild
        break;

    case s_http_host_v4:
        if (isHostChar(ch))
            return s_http_host_v4;
        else if (ch == ':')
            return s_http_host_port_start;
        break;

    case s_http_host_v6:
        if (ch == ']')
            return s_http_host_v6_end;
        else if (isIpv6Char(ch))
            return s_http_host_v6;
        else if (ch == '%')
            return s_http_host_v6_zone_start;
        break;

    case s_http_host_v6_end:
        if (ch == ':')
            return s_http_host_port_start;
        // FIXME:Not finished
        break;

    case s_http_host_v6_zone_start:
        if (isAlphaNum(ch) || ch == '%' || ch == '.' || ch == '-' || ch == '_' || ch == '~')
            return s_http_host_v6_zone;
        break;

    case s_http_host_v6_zone:
        if (ch == ']')
            return s_http_host_v6_end;
        // FIXME:RFC 6874
        // else if (ch ==''||
        //          ch =='')
        //     return s_http_host_error;

        else if (isAlphaNum(ch) || ch == '%' || ch == '.' || ch == '-' || ch == '_' || ch == '~')
            return s_http_host_v6_zone;
        break;

    case s_http_host_port_start:
    case s_http_host_port:
        if (isNum(ch))
            return s_http_host_port;
        break;

    default:
        break;
    }

    return s_http_host_error;
}

int HttpParser::parseHost(const char* stream,
    unsigned int at,
    unsigned int len,
    Url* result,
    bool has_at_char)
{
    //The example of data: dissigil.cn.
    //You MUST guarentee data just a fragment of host in url.
    //TODO: todo comments

    // assert(stream == result->data);
    // assert(result->field_set & (1 << HTTP_UF_HOST));
    // assert(len == result->fields[HTTP_UF_HOST].len);
    // assert(at == result->fields[HTTP_UF_HOST].offset);

    char* begin = (char*)stream + at;
    enum http_host_state prestat = has_at_char
        ? s_http_userinfo_start
        : s_http_host_start;
    enum http_host_state stat;
    for (unsigned int i = 0; i < len; i++) {
        char ch = *(begin + i);
        // std::cout << ch << (unsigned int)prestat << std::endl;

        stat = parseHostChar(ch, prestat);

        switch (stat) {
        case s_http_host_error:
            return -1;
            break;

        case s_http_userinfo_start:
            //This value is Impossible!
            continue;
            break;

        case s_http_userinfo:
            if (prestat != stat) {
                result->fields[HTTP_UF_USERINFO].offset = at + i;
                result->fields[HTTP_UF_USERINFO].len = 1;
                result->field_set |= (1 << HTTP_UF_USERINFO);
            } else
                result->fields[HTTP_UF_USERINFO].len++;
            break;

        case s_http_host_start:
            //nothing
            break;

        case s_http_host_v6_start:
            // do nothing
            break;

        case s_http_host_v4:
            if (prestat != stat) {
                result->fields[HTTP_UF_HOST].offset = at + i;
                result->fields[HTTP_UF_HOST].len = 1;
            } else
                result->fields[HTTP_UF_HOST].len++;
            break;

        case s_http_host_v6:
            if (prestat != stat) {
                result->fields[HTTP_UF_HOST].offset = at + i;
                result->fields[HTTP_UF_HOST].len = 1;
            } else
                result->fields[HTTP_UF_HOST].len++;
            break;

        case s_http_host_v6_end:
            //do nothing
            break;

        case s_http_host_v6_zone_start:
            result->fields[HTTP_UF_HOST].len++;
            break;

        case s_http_host_v6_zone:
            result->fields[HTTP_UF_HOST].len++;
            break;

        case s_http_host_port_start:
            //do nothing
            break;

        case s_http_host_port:
            if (prestat != stat) {
                result->fields[HTTP_UF_PORT].offset = at + i;
                result->fields[HTTP_UF_PORT].len = 1;
                result->field_set |= (1 << HTTP_UF_PORT);
            } else
                result->fields[HTTP_UF_PORT].len++;
            break;
        }

        prestat = stat;
    }

    switch (stat) {
    case s_http_host_error:
    case s_http_userinfo_start:
    case s_http_userinfo:
    case s_http_host_start:
    case s_http_host_v6_start:
    case s_http_host_v6:
    case s_http_host_v6_zone_start:
    case s_http_host_v6_zone:
    case s_http_host_port_start:
        return -1; //invaild

    case s_http_host_v4:
    case s_http_host_v6_end:
    case s_http_host_port:
        return 0; //vaild
        break;
    }

    return 0;
}

enum state HttpParser::parseUrlChar(const char ch,
    enum state stat)
{
    switch (ch) {
    case '\r':
    case '\n':
    case '\t':
    case '\a':
    case '\f':
    case ' ': //invalid char in url
        return s_error;
        break;

    default:
        break;
    }

    switch (stat) {
    case s_requ_url_begin:
        if (ch == '/')          //or ch=='*' :method CONNECT
            return s_requ_path; // /index.html
        else if (isAlpha(ch))
            return s_requ_schema;
        break;

    case s_requ_schema: //finished
        if (isAlpha(ch))
            return s_requ_schema;
        else if (ch == ':')
            return s_requ_schema_slash;
        break;

    case s_requ_schema_slash: //finished
        if (ch == '/')
            return s_requ_schema_slash_slash;
        break;

    case s_requ_schema_slash_slash: //finished
        if (ch == '/')
            return s_requ_server_start;
        break;

    case s_requ_server_start:

        if (ch == '/') //http:///
            return s_error;
        else if (ch == '@') //http://@hostname/ is vaild
            return s_requ_server;
        else if (ch == '?') //http://?queurystring/ is invaild
            return s_error;
        else if (ch == '#')
            return s_error;
        else if (ch == '[') //Ipv6 begin
            return s_requ_server;
        else if (ch == ']') //http://]:80/
            return s_error;
        else if (ch == ':') //http://:443
            return s_error;
        else if (isUserInfoChar(ch)) //FIXME:
            return s_requ_server;
        break;

    case s_requ_server: //finished

        if (ch == '/')
            return s_requ_path;
        else if (ch == '@')
            return s_requ_server_at;
        else if (ch == '?')
            return s_requ_query_string_start;
        else if (ch == '#')
            return s_requ_fragment_start;
        else if (isUserInfoChar(ch) || ch == '[' || ch == ']') //Ipv6 or userInfochar
            return s_requ_server;
        break;

    case s_requ_path: //finished
        if (isUrlChar(ch))
            return s_requ_path;
        else if (ch == '?')
            return s_requ_query_string_start;
        else if (ch == '#')
            return s_requ_fragment_start;
        break;

    case s_requ_server_at: //finished
        if (ch == '@')     //double '@' in url : invaild
            return s_error;
        else if (ch == '/') // host://a@/abc
            return s_error;
        // checkOrGoError(isUrlChar(ch));
        if (!isUrlChar(ch))
            return s_error;
        return s_requ_server;
        break;

    case s_requ_query_string_start: //finished
        if (isUrlChar(ch)) {
            return s_requ_query_string;
        } else if (ch == '?') {
            return s_requ_query_string;
        } else if (ch == '#') {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_query_string: //finished
        //FIXME:
        if (isUrlChar(ch)) {
            return s_requ_query_string;
        } else if (ch == '?') {
            return s_requ_query_string;
        } else if (ch == '#') {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_fragment_start: //finished
        //fragment:
        //http://blog.httpwatch.com/2011/03/01/6-things-you-should-know-about-fragment-urls/
        return s_requ_fragment;

        break;

    case s_requ_fragment: //finished
        if (isUrlChar(ch)) {
            return s_requ_fragment;
        } else if (ch == '?') {
            return s_requ_fragment;
        } else if (ch == '#') {
            return s_requ_fragment;
        }
        break;

    default:

        break;
    }

    return s_error;
}

int HttpParser::parseUrl(const char* stream,
    unsigned int len,
    Url* result)
{
    memset(result, 0, sizeof(Url));

    char* begin = (char*)stream;
    result->data = begin;
    result->len = len;

    enum state prestat = s_requ_url_begin;
    enum state stat;

    enum httpUrlField prefield = HTTP_UF_MAX;
    enum httpUrlField field;

    bool has_at_char = false;

    for (unsigned int i = 0; i < len; i++) {
        char ch = *(begin + i);
        // std::cout << "[Debug]" << ch << (unsigned int)prestat << std::endl;
        stat = parseUrlChar(ch, prestat);
        // field = 0;
        if (ch == '@')
            has_at_char = true;

        switch (stat) {
        case s_error: //finished
            return -1;
            break;

        /* Skip delimeters */
        case s_requ_schema_slash: //finished
        case s_requ_schema_slash_slash:
        case s_requ_server_start:
        case s_requ_query_string_start:
        case s_requ_fragment_start:
            prestat = stat;
            continue;
            break;

        case s_requ_server_at: //finished
            has_at_char = true;
            break;

        case s_requ_schema: //finished
            field = HTTP_UF_SCHEMA;
            break;

        case s_requ_server: //finished
            field = HTTP_UF_HOST;
            break;

        case s_requ_path: //finished
            field = HTTP_UF_PATH;
            break;

        case s_requ_query_string: //finished
            field = HTTP_UF_QUERY;
            break;

        case s_requ_fragment: //finished
            field = HTTP_UF_FRAGMENT;
            break;

        default:
            break;
        }

        // std::cout << int(field) << std::endl;

        if (field == prefield) {
            result->fields[field].len++;
        } else {
            result->fields[field].offset = i;
            result->fields[field].len = 1;

            result->field_set |= (1 << field);
        }

        prefield = field;
        prestat = stat;
    }

    if (result->field_set & (1 << HTTP_UF_HOST)) {
        unsigned int offset = result->fields[HTTP_UF_HOST].offset;
        unsigned int len = result->fields[HTTP_UF_HOST].len;
        std::string host(begin + offset, len);
        // std::cout << "[parseUrl]::host:" << len << ":" << host << std::endl;
        if (-1 == parseHost(stream, offset, len, result, has_at_char))
            return -1;
    } else if (result->field_set & (1 << HTTP_UF_SCHEMA)) // http:///index.html is invaild
        return -1;

    if (result->field_set & (1 << HTTP_UF_PORT)) {
        unsigned int offset = result->fields[HTTP_UF_PORT].offset;
        unsigned int len = result->fields[HTTP_UF_PORT].len;
        unsigned int port = 0;
        for (unsigned int i = 0; i < len; i++) {
            port *= 10;
            port += *(begin + offset + i) - '0';
            if (port > 65535)
                return -1;
        }
        // std::cout << "port:" << port << std::endl;
    }

    return 0;
}

enum http_header_state HttpParser::parseHeaderChar(const char ch,
    enum http_header_state stat)
{
    switch (stat) {
    case s_http_header_error:
        //impossible
        return s_http_header_error;
        break;

    case s_http_header_start:
        if (ch == ' ')
            return s_http_header_start;
        else if (ch == '\r')
            return s_http_headers_almost_done;
        else if (ch == '\n')
            return s_http_headers_done;
        else if (ch == ':')
            return s_http_header_start;
        else if (isAlphaNum(ch))
            return s_http_header_key_start;
        break;

    case s_http_header_key_start:
        if (isAlphaNum(ch) || ch == '-')
            return s_http_header_key;
        else if (ch == ':')
            return s_http_header_colon;
        break;

    case s_http_header_key:
        if (ch == ':')
            return s_http_header_colon;
        else if (isAlphaNum(ch) || ch == '-')
            return s_http_header_key;
        break;

    case s_http_header_colon:
        if (ch == ' ')
            return s_http_header_space;
        break;

    case s_http_header_space:
        if (ch == ' ')
            return s_http_header_space;
        else if (ch == '\r')
            return s_http_header_error;
        else if (ch == '\n')
            return s_http_header_error;
        return s_http_header_value_start;
        break;

    case s_http_header_value_start:
        if (ch == '\r')
            return s_http_header_almost_done;
        else if (ch == '\n')
            return s_http_header_done;
        return s_http_header_value;
        break;

    case s_http_header_value:
        if (ch == '\r')
            return s_http_header_almost_done;
        else if (ch == '\n')
            return s_http_header_done;
        return s_http_header_value;
        break;

    case s_http_header_almost_done:
        if (ch == '\r')
            return s_http_header_almost_done;
        else if (ch == '\n')
            return s_http_header_done;
        break;

    case s_http_header_done:
        if (ch == '\r')
            return s_http_headers_almost_done;
        else if (ch == '\n')
            return s_http_headers_done;
        break;

    case s_http_headers_almost_done:
        if (ch == '\r')
            return s_http_headers_almost_done;
        else if (ch == '\n')
            return s_http_headers_done;
        return s_http_header_error;
        break;

    case s_http_headers_done:
        //impossible ????
        return s_http_header_start;
        break;
    }

    return s_http_header_error;
}

int HttpParser::parseHeader(const char* stream,
    unsigned int& at,
    unsigned int len,
    HttpHeader* result)
{

    char* begin = (char*)stream;

    enum http_header_state prestat = s_http_header_start;
    enum http_header_state stat;

    unsigned int keybegin = 0;
    unsigned int keylen = 0;

    unsigned int valuebegin = 0;
    unsigned int valuelen = 0;

    for (unsigned int i = 0; i < len; i++) {
        char ch = *(begin + at + i);
        stat = parseHeaderChar(ch, prestat);

        switch (stat) {
        case s_http_header_error:
            return -1;
            break;

        case s_http_header_start:
            //do nothing
            break;

        case s_http_header_key_start:
            if (isAlphaNum(ch) || ch == '-')
                ch = toLower(ch);
            else
                return -1;
            keybegin = at + i;
            keylen = 1;
            break;

        case s_http_header_key:
            if (isAlphaNum(ch) || ch == '-')
                ch = toLower(ch);
            else
                return -1;
            keylen++;
            break;

        case s_http_header_colon:
            //do nothing
            break;

        case s_http_header_space:
            //do nothing
            break;

        case s_http_header_value_start:
            valuebegin = at + i;
            valuelen = 1;
            break;

        case s_http_header_value:
            valuelen++;
            break;

        case s_http_header_almost_done:
            break;

        case s_http_header_done:
            result->keyHash = JSHash(begin + keybegin, keylen);
            result->key.data = begin + keybegin;
            result->key.len = keylen;
            result->value.data = begin + valuebegin;
            result->value.len = valuelen;

            // printf("[%u]%.*s->%.*s^\n", result->keyHash, keylen, begin + keybegin,
            //       valuelen, begin + valuebegin);

            at += i;
            return 0;
            stat = s_http_header_start;
            break;

        case s_http_headers_almost_done:
            break;

        case s_http_headers_done:
            at += i;
            return 1; //headers done
            break;
        }

        prestat = stat;
    }

    return 0;
}

int HttpParser::parseHeaders(const char* stream,
    unsigned int at,
    unsigned int len,
    HttpHeaders* result)
{
    int return_val = 0;
    unsigned int offset = at;

    while (1) {
        HttpHeader* header = new HttpHeader;
        return_val = parseHeader(stream, offset, len, header);
        if (return_val == -1) {
            std::cout << "parse header error\n";
            return -1;
            break;
        } else if (return_val == 0) {
            result->generals.push_back(header);
        } else if (return_val == 1) {
            //parse headers done
            break;
        }
        // std::cout << "[Debug]index after parseHeader:" << offset << std::endl;

        if (offset == len)
            break;
        offset++;
    }

    return 0;
}

int HttpParser::parseHeadersMeaning(HttpHeaders* headers)
{
    for (auto t : headers->generals) {
        auto p = headerKeyHash.find(t->keyHash);
        if (p == headerKeyHash.end()) {
            std::cout << "[parse headerMeaning callback]general header" << std::endl;
        } else {
            HttpHeader** tmp = (HttpHeader**)((char*)headers + p->second.offset);
            *tmp = t;
            p->second.fun(&(t->value), headers);
        }
    }
    return 0;
}

int HttpParser::semanticAnalysisHttp(HttpRequest* request)
{
    HttpContentType type = request->type;
    HttpHeaders* hs = request->headers;
    bool keep_alive = shouldKeepAlive(request);
    enum http_body_type body_type = t_http_body_type_init;
    unsigned short method = request->method;
    unsigned short http_version = request->http_version_major * 10 + request->http_version_minor;
    unsigned int status_code = request->statusCode;

    if (hs->content_identify_length && hs->chunked) {

        // Bad request.
        std::cout << "[Debug] headers have content-length and chunked simultaneously\n";
        return -1;
    } else if (hs->content_identify_length && !hs->chunked) {

        // Identify body by content_length.
        std::cout << "[Debug] identify body by length\n";
        body_type = t_http_body_end_by_length;
    } else if (!hs->content_identify_length && hs->chunked) {

        // Identify body by chunked.
        std::cout << "[Debug] identify body by chunked\n";
        body_type = t_http_body_chunked;
    } else {

        // Identify body by eof.
        // Client can detect the close of connection.
        std::cout << "[Debug] identify body by eof\n";
        body_type = t_http_body_end_by_eof;

        /* See RFC 2616 section 4.4 */
        if (status_code / 100 == 1 || /* 1xx e.g. Continue */
            status_code == 204 ||     /* No Content */
            status_code == 304        /* Not Modified */
        ) {                           /* response to a HEAD request */
            // No body message.
            body_type = t_http_body_skip;
        }
    }

    if (http_version <= 9) {
        printf("Skip body message HTTP%d\n", http_version);
        body_type = t_http_body_skip;
    }

    if (!hs->valid_host && http_version >= 11) {
        // Bad request.
        printf("Must have host field in HTTP%d\n", http_version);
        return -1;
    }

    if (method == HTTP_METHOD_HEAD) {
        body_type = t_http_body_skip;
    }

    if (method == HTTP_METHOD_PUT && !hs->valid_content_length) {
        //printf("Must have content in method PUT\n");
        // Bad request.
    }

    if (type == HTTP_TYPE_REQUEST &&  /* http-request */
        method != HTTP_METHOD_GET &&  /* GET */
        method != HTTP_METHOD_POST && /* POST */
        method != HTTP_METHOD_HEAD && /* HEAD */
        method != HTTP_METHOD_PUT     /* PUT */
    ) {
        printf("sorry, tinyweb can't support this http-method now\n");
    }

    // if (request->method == HTTP_METHOD_TRACE) {
    //printf("Client request with method TRACE\n");
    // }

    //if (hs->has_upgrade && hs->connection_upgrade)
    // {
    //Upgrade: WebSocket
    //Connection: Upgrade
    //search google
    // }

    // if (request->method == HTTP_METHOD_CONNECT && http_version <= 10) {
    //https://blog.csdn.net/kobejayandy/article/details/24606521
    // }

    request->body_type = body_type;
    return 0;
}

int HttpParser::semanticAnalysisFcgiResponse(HttpRequest* request)
{
    HttpContentType type = request->type;
    HttpHeaders* hs = request->headers;
    bool keep_alive = shouldKeepAlive(request);
    enum http_body_type body_type = t_http_body_type_init;
    unsigned short method = request->method;
    unsigned short http_version = request->http_version_major * 10 + request->http_version_minor;
    unsigned int status_code = request->statusCode;

    // TODO: need to judge the meaning of fcgi response.
    LOG(Debug) << "analysis fcgi response\n";

    body_type = t_http_body_end_by_eof;

    request->body_type = body_type;
    return 0;
}

int HttpParser::parseBody(const char* stream,
    unsigned int at,
    unsigned int len,
    enum http_body_type body_type,
    unsigned int content_length_n)
{
    if (!len) {
        return 0;
    }
    const char* begin = stream;

    enum http_body_state stat;

    switch (body_type) {
    case t_http_body_type_init:
        // TODO:
        break;

    case t_http_body_end_by_length:
        stat = s_http_body_identify_by_length;
        break;

    case t_http_body_end_by_eof:
        stat = s_http_body_identify_by_eof;
        break;

    case t_http_body_chunked:
        stat = s_http_body_chunk_size;
        break;

    case t_http_body_skip:
        // TODO:
        break;
    }

    unsigned long long chunk_size = 0;

    for (unsigned int i = 0; i < len; i++) {
        char ch = *(begin + at + i);

        // printf("parseBody:%c-%d\n", ch, int(stat));

        switch (stat) {
        case s_http_body_error:
            return -1;
            break;

        case s_http_body_identify_by_length: {
            unsigned int to_read = MIN(content_length_n,
                len - i);
            printf("len:%d,content:%.*s\n",to_read, to_read, begin + at + i);
            i += to_read;
            return 0;
        } break;

        case s_http_body_identify_by_eof:
            printf("content:%.*s\n", len - i, begin + at + i);
            i = len;
            break;

        case s_http_body_chunk_size_start: {
            short int tmp = getHex(ch);
            if (tmp == -1) {
                std::cout << "chunk size invalid\n";
                return -1;
            }
            chunk_size *= 16;
            chunk_size += tmp;
            stat = s_http_body_chunk_size;
        } break;

        case s_http_body_chunk_size: {
            if (ch == CR) {
                stat = s_http_body_chunk_size_almost_done;
                break;
            } else if (ch == ' ') {
                stat = s_http_body_chunk_size;
                break;
            }

            short int tmp = getHex(ch);
            if (tmp == -1) {
                std::cout << "chunk size invalid\n";
                return -1;
            }
            chunk_size *= 16;
            chunk_size += tmp;
        } break;

        case s_http_body_chunk_size_almost_done:
            checkOrGoError((ch == LF));
            std::cout << "chunk size:" << chunk_size << std::endl;
            if (chunk_size != 0)
                stat = s_http_body_chunk_data;
            else {
                //is trailing;
            }
            stat = s_http_body_chunk_data;
            break;

        case s_http_body_chunk_size_done:
            //TODO:finish
            break;

        case s_http_body_chunk_data: {
            unsigned long long to_read = MIN(chunk_size,
                len - i);
            printf("data-len:%d,chunk-data:%.*s\n", to_read, to_read, begin + at + i);
            i += to_read - 1;
            stat = s_http_body_chunk_data_almost_done;
        }

        break;

        case s_http_body_chunk_data_almost_done:
            checkOrGoError((ch == CR));
            stat = s_http_body_chunk_data_done;
            break;

        case s_http_body_chunk_data_done:
            checkOrGoError((ch == LF));
            if (chunk_size == 0) {
                std::cout << "It is traling chunk\n";
                stat = s_http_body_chunks_done;
            } else {
                stat = s_http_body_chunk_size_start;
                chunk_size = 0;
            }
            // stat = s_http_body_chunk;
            break;

        case s_http_body_chunks_done:
            return 0;
            break;
        }
    }

    return 0;

error:
    return -1;
}

int HttpParser::execute(const char* stream,
    unsigned int at,
    unsigned int len,
    HttpRequest* request)
{
    char* begin = (char*)stream + at;

    int return_val = 0;
    bool break_for = false;
    unsigned int content_length = 0;

    // enum http_method method;
    enum http_body_type body_type = t_http_body_type_init;

    unsigned int url_begin = 0;
    unsigned int url_len = 0;

    unsigned int status_phrase_begin = 0;
    unsigned int status_phrase_len = 0;

    unsigned int headers_begin = 0;
    unsigned int headers_len = 0;

    unsigned int body_begin = 0;
    //unsigned int body_len = 0;

    unsigned int method_begin = 0;
    unsigned int method_len = 0;

    //unsigned int headerNum = 0;

    if (getErrno() != HPE_OK)
        return 0;

    if (len == 0) {
        return 0;
    }

    switch (m_nState) {
    case s_resp_start:
        request->type = HTTP_TYPE_RESPONSE;
        break;

    case s_requ_start:
        request->type = HTTP_TYPE_REQUEST;
        break;

    case s_start_resp_or_requ:
        request->type = HTTP_TYPE_BOTH;
        break;

    case s_requ_line_done:
        request->type = HTTP_TYPE_FCGI_RESPONSE;
        break;

    default:
        break;
    }

    for (unsigned int i = 0; i < len; i++) {
        char ch = *(begin + i);
        // LOG(Debug) << "char:" << ch << ",stat:" << m_nState << std::endl;

        switch (m_nState) {

        case s_start_resp_or_requ:
            // need to fix FIXME:
            if (ch == 'H') //is http response
            {
                m_nState = s_resp_H;
                // invokeByName("getResponseMessage", nullptr, 0);TODO:
            } else {
                m_nState = s_requ_start;
                // invokeByName("getRequestsMessage", nullptr, 0);TODO:
            }
            // invokeByName("getMessage", nullptr, 0);TODO:
            break;

        case s_resp_start: //not finished
            checkOrGoError((ch == 'H'));
            m_nState = s_resp_H;
            break;

        case s_resp_H:
            checkOrGoError((ch == 'T'));
            m_nState = s_resp_HT;
            break;

        case s_resp_HT:
            checkOrGoError((ch == 'T'));
            m_nState = s_resp_HTT;
            break;

        case s_resp_HTT:
            checkOrGoError((ch == 'P'));
            m_nState = s_resp_HTTP;
            break;

        case s_resp_HTTP:
            checkOrGoError((ch == '/'));
            m_nState = s_resp_HTTP_slash;
            break;

        case s_resp_HTTP_slash:
            checkOrGoError(isNum(ch));
            m_nState = s_resp_version_major;
            m_nHttpVersionMajor = ch - '0';
            break;

        case s_resp_version_major:
            checkOrGoError((ch == '.'));
            m_nState = s_resp_version_dot;
            break;

        case s_resp_version_dot:
            checkOrGoError(isNum(ch));
            m_nState = s_resp_version_minor;
            m_nHttpVersionMinor = ch - '0';
            break;

        case s_resp_version_minor:
            checkOrGoError((ch == ' '));
            m_nState = s_resp_status_code_start;
            break;

        case s_resp_status_code_start:
            m_nStatusCode = 0;
            checkOrGoError(isNum(ch));
            m_nStatusCode *= 10;
            m_nStatusCode += ch - '0';
            m_nState = s_resp_status_code;
            break;

        case s_resp_status_code:
            if (ch == ' ') {
                m_nState = s_resp_status_phrase_start;
            } else {
                checkOrGoError(isNum(ch));
                m_nStatusCode *= 10;
                m_nStatusCode += ch - '0';
            }
            break;

        case s_resp_status_phrase_start:
            checkOrGoError(isAlpha(ch));
            status_phrase_begin = i;
            status_phrase_len = 1;
            m_nState = s_resp_status_phrase;
            break;

        case s_resp_status_phrase:
            if (ch == '\r') {
                m_nState = s_resp_line_almost_done;
            } else if (ch == '\n') {
                m_nState = s_resp_line_done;
            } else if (isAlphaNum(ch) || ch == ' ' || ch == '-') {
                status_phrase_len++;
            }
            break;

        case s_resp_line_almost_done:
            checkOrGoError((ch == '\n'));
            {
                //invokeByName("", nullptr, 0);TODO:
                m_nState = s_resp_line_done;
            }
            break;

        case s_resp_line_done:
            if (ch == '\r') {
                m_nState = s_headers_almost_done;
                break;
            } else if (ch == '\n') {
                m_nState = s_headers_done;
                break;
            }

            checkOrGoError(isAlphaNum(ch));
            headers_begin = i;
            m_nState = s_header_start;
            break;

        case s_requ_start:
            m_nState = s_requ_method;
            method_begin = i;
            method_len = 1;
            break;

        case s_requ_method_start:
            if (ch == ' ') {
            }
            checkOrGoError((isUpper(ch) || ch == '_'));
            method_len++;
            break;

        case s_requ_method:
            if (ch == ' ') {
                m_nState = s_requ_url_begin;
            } else if (isUpper(ch) || ch == '_') {
                method_len++;
            }
            break;

        case s_requ_url_begin:
            if (isUrlChar(ch)) {
                url_begin = i;
                url_len = 1;
                m_nState = s_requ_url;
            } else if (ch == ' ') {
                m_nState = s_requ_url_begin;
            }
            break;

        case s_requ_url:
            if (isUrlChar(ch)) {
                m_nState = s_requ_url;
                url_len++;
            } else if (ch == ' ') {
                m_nState = s_requ_HTTP_start;
            }
            break;

        case s_requ_schema:
        case s_requ_schema_slash:
        case s_requ_schema_slash_slash:
        case s_requ_server_start:
        case s_requ_server:
        case s_requ_path:
        case s_requ_query_string_start:
        case s_requ_query_string:
        case s_requ_fragment_start:
            //do nothing
            break;

        case s_requ_HTTP_start:
            checkOrGoError((ch == 'H'));
            m_nState = s_requ_H;
            break;

        case s_requ_H:
            checkOrGoError((ch == 'T'));
            m_nState = s_requ_HT;
            break;

        case s_requ_HT:
            checkOrGoError((ch == 'T'));
            m_nState = s_requ_HTT;
            break;

        case s_requ_HTT:
            checkOrGoError((ch == 'P'));
            m_nState = s_requ_HTTP;
            break;

        case s_requ_HTTP:
            checkOrGoError((ch == '/'));
            m_nState = s_requ_HTTP_slash;
            break;

        case s_requ_HTTP_slash:
            checkOrGoError(isNum(ch));
            m_nState = s_requ_version_major;
            m_nHttpVersionMajor = ch - '0';
            break;

        case s_requ_version_major:
            checkOrGoError((ch == '.'));
            m_nState = s_requ_version_dot;
            break;

        case s_requ_version_dot:
            checkOrGoError(isNum(ch));
            m_nState = s_requ_version_minor;
            m_nHttpVersionMinor = ch - '0';
            break;

        case s_requ_version_minor:
            checkOrGoError((ch == '\r'));
            m_nState = s_requ_line_almost_done;
            break;

        case s_requ_line_almost_done:
            checkOrGoError('\n');
            m_nState = s_requ_line_done;
            break;

        case s_requ_line_done:
            if (ch == '\r') {
                m_nState = s_headers_almost_done;
                break;
            } else if (ch == '\n') {
                m_nState = s_headers_done;
                break;
            }

            checkOrGoError(isAlpha(ch));
            m_nState = s_header_start;
            headers_begin = i;
            headers_len = 1;
            break;

        case s_header_start:
            headers_len++;
            m_nState = s_header;
            break;

        case s_header:
            headers_len++;
            if (ch == '\r')
                m_nState = s_header_almost_done;
            else if (ch == '\n')
                m_nState = s_header_done;
            else if (isAlphaNum(ch) || ch == ':')
                m_nState = s_header;
            break;

        case s_header_almost_done:
            checkOrGoError((ch == '\n'));
            headers_len++;
            m_nState = s_header_done;
            break;

        case s_header_done:
            if (isAlphaNum(ch) || ch == ':')
                m_nState = s_header_start;
            else if (ch == '\r')
                m_nState = s_headers_almost_done;
            else if (ch == '\n')
                m_nState = s_headers_done;
            headers_len++;
            break;

        case s_headers_almost_done:
            checkOrGoError((ch == '\n'));
            m_nState = s_headers_done;
            headers_len++;
            break;

        case s_headers_done:
            //TODO: do something by headers meaning
            //std::cout << "[Debug] body begin\n";
            body_begin = i;
            m_nState = s_body_start;
            break_for = true;
            break;

        case s_body_start:
            break;

        case s_body:
            break;

        case s_body_done:
            break;

        case s_chunk:
            break;
        }

        if (break_for)
            break;
    }

    //parse http content done
    //parse headers,content,

    if (request->type == HTTP_TYPE_REQUEST) {
        request->method = getMethod(begin + method_begin, method_len);
        request->method_s = std::string(begin + method_begin, method_len);

        request->url = new Url;
        return_val = parseUrl(begin + url_begin,
            url_len,
            request->url);
        if (return_val == -1)
            goto error;
        //change goto error command
    } else if (request->type == HTTP_TYPE_RESPONSE) {
        //HTTP_TYPE_RESPONSE
        request->statusCode = m_nStatusCode;
        request->statusPhrase = std::string(status_phrase_begin, status_phrase_len);
    }

    request->http_version_major = m_nHttpVersionMajor;
    request->http_version_minor = m_nHttpVersionMinor;

    //parse headers information.
    request->headers = new HttpHeaders;
    httpHeadersInit(request->headers);
    request->headers->data = (char*)begin;
    request->headers->offset = headers_begin;
    request->headers->len = headers_len;

    return_val = parseHeaders(begin,
        headers_begin,
        headers_len,
        request->headers);

    if (return_val == -1)
        goto error;

    return_val = parseHeadersMeaning(request->headers);
    if (return_val == -1)
        goto error;

    //switch body type : chunk or end-by-eof or end-by-length
    if (request->type == HTTP_TYPE_FCGI_RESPONSE) {
        return_val = semanticAnalysisFcgiResponse(request);
    } else {
        return_val = semanticAnalysisHttp(request);
    }
    if (return_val == -1)
        goto error;

    // FIXME: to handle the body.
    // have bug.
    if (body_begin == 0 || len - body_begin == 0) {
        return 0;
    }

    body_type = request->body_type;
    // TODO: update the parseBody function.
    return_val = parseBody(begin, body_begin, len - body_begin, body_type, request->headers->content_length_n);
    if (return_val == -1)
        goto error;

    return 0;

error:
    // std::cout << "parser error\n";
    LOG(Debug) << "http request content is invalid\n";
    return -1;
};
