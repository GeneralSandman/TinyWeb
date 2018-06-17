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

#include "http.h"
#include "my_http_parser.h"

#include <string.h>
#include <stdio.h>

void printUrl(const Url *url)
{
    if (url->field_set & (1 << HTTP_UF_SCHEMA))
    {
        int off = url->fields[HTTP_UF_SCHEMA].offset;
        int len = url->fields[HTTP_UF_SCHEMA].len;
        printf("schema:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_HOST))
    {
        int off = url->fields[HTTP_UF_HOST].offset;
        int len = url->fields[HTTP_UF_HOST].len;
        printf("host:%.*s\n", len, (const char *)url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_PORT))
    {
        int off = url->fields[HTTP_UF_PORT].offset;
        int len = url->fields[HTTP_UF_PORT].len;
        printf("port:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_PATH))
    {
        int off = url->fields[HTTP_UF_PATH].offset;
        int len = url->fields[HTTP_UF_PATH].len;
        printf("path:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_QUERY))
    {
        int off = url->fields[HTTP_UF_QUERY].offset;
        int len = url->fields[HTTP_UF_QUERY].len;
        printf("query:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_FRAGMENT))
    {
        int off = url->fields[HTTP_UF_FRAGMENT].offset;
        int len = url->fields[HTTP_UF_FRAGMENT].len;
        printf("fragment:%.*s\n", len, url->data + off);
    }
    if (url->field_set & (1 << HTTP_UF_USERINFO))
    {
        int off = url->fields[HTTP_UF_USERINFO].offset;
        int len = url->fields[HTTP_UF_USERINFO].len;
        printf("userinfo:%.*s\n", len, url->data + off);
    }
}

#define checkOrGoError(con) \
    do                      \
    {                       \
        if (!con)           \
        {                   \
            goto error;     \
        }                   \
    } while (0);

void HttpParser::setType(enum httpParserType type)
{
    m_nType = type;
    m_nState = (type == HTTP_RESPONSE) ? s_resp_start
                                       : ((type == HTTP_REQUEST
                                               ? s_requ_start
                                               : s_start_resp_or_requ));
}

int HttpParser::invokeByName(const char *funName,
                             const char *data,
                             unsigned int len)
{
    std::cout << "invoke function by name:" << funName << std::endl;
    if (m_pSettings == nullptr)
        return -1;
    std::string fname(funName);
    HttpCallback fun = m_pSettings->getMethodByName(fname);
    if (fun == nullptr)
        return -1;

    return fun();
}

enum http_host_state HttpParser::parseHostChar(const char ch,
                                               enum http_host_state stat)
{
    switch (ch)
    {
    //invaild char in url
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

    switch (stat)
    {
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
        //not finished
        break;

    case s_http_host_v6_zone_start:
        if (isAlphaNum(ch) ||
            ch == '%' ||
            ch == '.' ||
            ch == '-' ||
            ch == '_' ||
            ch == '~')
            return s_http_host_v6_zone;
        break;

    case s_http_host_v6_zone:
        if (ch == ']')
            return s_http_host_v6_end;
        //FIXME:RFC 6874
        // else if (ch ==''||
        //          ch =='')
        //     return s_http_host_error;

        else if (isAlphaNum(ch) ||
            ch == '%' ||
            ch == '.' ||
            ch == '-' ||
            ch == '_' ||
            ch == '~')
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

int HttpParser::parseHost(const char *stream,
                          int &at,
                          int len,
                          Url *&result,
                          bool has_at_char)
{
    //The example of data: dissigil.cn.
    //You MUST guarentee data just a fragment of host in url.
    //TODO: todo comments

    assert(stream == result->data);
    assert(result->field_set & (1 << HTTP_UF_HOST));
    assert(len == result->fields[HTTP_UF_HOST].len);
    assert(at == result->fields[HTTP_UF_HOST].offset);

    char *begin = (char *)stream + at;
    enum http_host_state prestat = has_at_char
                                       ? s_http_userinfo_start
                                       : s_http_host_start;
    enum http_host_state stat;
    for (int i = 0; i < len; i++)
    {
        char ch = *(begin + i);
        // std::cout << ch << (unsigned int)prestat << std::endl;

        stat = parseHostChar(*(begin + i), prestat);

        switch (stat)
        {
        case s_http_host_error:
            return -1;
            break;

        case s_http_userinfo_start:
            //This value is Impossible!
            continue;
            break;

        case s_http_userinfo:
            if (prestat != stat)
            {
                result->fields[HTTP_UF_USERINFO].offset = at + i;
                result->fields[HTTP_UF_USERINFO].len = 1;
                result->field_set |= (1 << HTTP_UF_USERINFO);
            }
            else
                result->fields[HTTP_UF_USERINFO].len++;
            break;

        case s_http_host_start:
            //nothing
            break;

        case s_http_host_v6_start:
            // do nothing
            break;

        case s_http_host_v4:
            if (prestat != stat)
            {
                result->fields[HTTP_UF_HOST].offset = at + i;
                result->fields[HTTP_UF_HOST].len = 1;
            }
            else
                result->fields[HTTP_UF_HOST].len++;
            break;

        case s_http_host_v6:
            if (prestat != stat)
            {
                result->fields[HTTP_UF_HOST].offset = at + i;
                result->fields[HTTP_UF_HOST].len = 1;
            }
            else
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
            if (prestat != stat)
            {
                result->fields[HTTP_UF_PORT].offset = at + i;
                result->fields[HTTP_UF_PORT].len = 1;
                result->field_set |= (1 << HTTP_UF_PORT);
            }
            else
                result->fields[HTTP_UF_PORT].len++;
            break;
        }

        prestat = stat;
    }

    switch (stat)
    {
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
    switch (ch)
    {
    case '\r':
    case '\n':
    case '\t':
    case '\a':
    case '\f':
    case ' ': //invaild char in url
        return s_error;
        break;

    default:
        break;
    }

    switch (stat)
    {
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
        else if (isUserInfoChar(ch) ||
                 ch == '[' ||
                 ch == ']') //Ipv6 or userInfochar
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
        if (isUrlChar(ch))
        {
            return s_requ_query_string;
        }
        else if (ch == '?')
        {
            return s_requ_query_string;
        }
        else if (ch == '#')
        {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_query_string: //finished
        //FIXME:
        if (isUrlChar(ch))
        {
            return s_requ_query_string;
        }
        else if (ch == '?')
        {
            return s_requ_query_string;
        }
        else if (ch == '#')
        {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_fragment_start: //finished
                                //fragment:
                                //http://blog.httpwatch.com/2011/03/01/6-things-you-should-know-about-fragment-urls/
        return s_requ_fragment;

        break;

    case s_requ_fragment: //finished
        if (isUrlChar(ch))
        {
            return s_requ_fragment;
        }
        else if (ch == '?')
        {
            return s_requ_fragment;
        }
        else if (ch == '#')
        {
            return s_requ_fragment;
        }
        break;

    default:

        break;
    }

    return s_error;
}

int HttpParser::parseUrl(const char *stream,
                         int &at,
                         int len,
                         Url *&result)
{
    // std::cout << "function parseUrl\n";
    memset(result, sizeof(Url), 0);

    char *begin = (char *)stream;
    result->data = begin;
    enum state prestat = s_requ_url_begin;
    enum state stat;
    enum httpUrlField prefield = HTTP_UF_MAX;
    enum httpUrlField field;
    bool has_at_char = false; //TODO:return has_at_char

    for (int i = 0; i < len; i++)
    {
        char ch = *(begin + at + i);
        // std::cout << ch << (unsigned int)prestat << std::endl;
        stat = parseUrlChar(ch, prestat);
        // field = 0;
        if (ch == '@')
            has_at_char = true;

        switch (stat)
        {
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

        if (field == prefield)
        {
            result->fields[field].len++;
        }
        else
        {
            result->fields[field].offset = at + i;
            result->fields[field].len = 1;

            result->field_set |= (1 << field);
        }

        prefield = field;
        prestat = stat;
    }

    if (result->field_set & (1 << HTTP_UF_HOST))
    {
        int offset = result->fields[HTTP_UF_HOST].offset;
        int len = result->fields[HTTP_UF_HOST].len;
        std::string host(result->data + offset, len);
        std::cout << "--host:" << host << std::endl;

        if (-1 == parseHost(stream, offset, len, result, has_at_char))
            return -1;
    }
    else if (result->field_set & (1 << HTTP_UF_SCHEMA)) // http:///index.html is invaild
        return -1;

    if (result->field_set & (1 << HTTP_UF_PORT))
    {
        unsigned int offset = result->fields[HTTP_UF_PORT].offset;
        unsigned int len = result->fields[HTTP_UF_PORT].len;
        unsigned int port = 0;
        for (int i = 0; i < len; i++)
        {
            port *= 10;
            port += *(begin + offset + i) - '0';
            if (port > 65535)
                return -1;
        }
        // std::cout << "port:" << port << std::endl;
    }

    //FIXME:
    //TODO:
    //!!!

    return 0;
}

enum http_header_state HttpParser::parseHeaderChar(const char ch,
                                                   enum http_header_state stat)
{
    switch (stat)
    {
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
        else if (isAlpha(ch))
            return s_http_header_key_start;
        break;

    case s_http_header_key_start:
        if (isAlpha(ch) || ch == '-')
            return s_http_header_key;
        else if (ch == ':')
            return s_http_header_colon;
        break;

    case s_http_header_key:
        if (ch == ':')
            return s_http_header_colon;
        else if (isAlpha(ch) || ch == '-')
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

int HttpParser::parseHeader(const char *stream,
                            int &at,
                            int len)
{

    char *begin = (char *)stream;

    enum http_header_state prestat = s_http_header_start;
    enum http_header_state stat;

    unsigned int headers = 0;

    unsigned int keybegin = 0;
    unsigned int keylen = 0;

    unsigned int valuebegin = 0;
    unsigned int valuelen = 0;

    for (int i = 0; i < len; i++)
    {
        char ch = *(begin + at + i);
        stat = parseHeaderChar(ch, prestat);

        switch (stat)
        {
        case s_http_header_error:
            return -1;
            break;

        case s_http_header_start:
            //do nothing
            break;

        case s_http_header_key_start:
            keybegin = at + i;
            keylen = 1;
            break;

        case s_http_header_key:
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
            // std::cout << "key begin:" << keybegin
            //   << " key len:" << keylen
            //   << "->value begin:" << valuebegin
            //   << " value len:" << valuelen << std::endl;
            printf("%.*s->%.*s^\n", keylen, begin + keybegin,
                   valuelen, begin + valuebegin);
            headers++;
            keybegin = keylen = 0;
            valuebegin = valuelen = 0;
            stat = s_http_header_start;
            break;

        case s_http_headers_almost_done:
            break;

        case s_http_headers_done:
            std::cout << "header number:" << headers << std::endl;
            break;
        }

        prestat = stat;
    }
}

int HttpParser::execute(const char *stream,
                        int &at,
                        int len)
{
    std::cout << "function HttpParser::execute()\n";

    const char *begin = stream;

    const char *url_begin = nullptr;
    const char *status_phrase_begin = nullptr;
    const char *header_key_begin = nullptr;
    const char *header_value_begin = nullptr;
    const char *body_begin = nullptr;
    const char *method_begin = nullptr;

    unsigned int headers = 0;

    if (getErrno() != HPE_OK)
        return 0;

    if (len == 0)
    {
    }

    switch (m_nState)
    {
    // switch to some init state;
    case s_resp_start:

        break;

    case s_requ_start:

        break;

    case s_start_resp_or_requ:

        break;

    default:
        break;
    }

    for (int i = 0; i < len; i++)
    {
        char ch = *(begin + at + i);

        switch (m_nState)
        {

        case s_start_resp_or_requ:
            if (ch == 'H') //response
            {
                m_nState = s_resp_H;
                invokeByName("getMessage", nullptr, 0);
            }
            else
            {
                m_nState = s_requ_start;
                invokeByName("getMessage", nullptr, 0);
            }
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
            std::cout << "response http version:HTTP/" << m_nHttpVersionMajor << "."
                      << m_nHttpVersionMinor << std::endl;
            break;

        case s_resp_status_code_start:
            m_nStatusCode = 0;
            checkOrGoError(isNum(ch));
            // std::cout << *(begin + i) << std::endl;
            m_nStatusCode *= 10;
            m_nStatusCode += ch - '0';
            m_nState = s_resp_status_code;
            break;

        case s_resp_status_code:
            if (ch == ' ')
            {
                m_nState = s_resp_status_phrase_start;
                std::cout << "http status code:" << m_nStatusCode << std::endl;
            }
            else
            {
                checkOrGoError(isNum(ch));
                m_nStatusCode *= 10;
                m_nStatusCode += ch - '0';
            }
            break;

        case s_resp_status_phrase_start:
            checkOrGoError(isAlpha(ch));
            status_phrase_begin = begin + at + i;
            m_nState = s_resp_status_phrase;
            break;

        case s_resp_status_phrase:
            if (ch == '\r')
            {
                m_nState = s_resp_line_almost_done;
            }
            else if (ch == '\n')
            {
                std::string phrase(status_phrase_begin, begin + at + i);
                std::cout << "status phrase:" << phrase << std::endl;
                m_nState = s_resp_line_done;
            }
            else if (isAlpha(ch))
            {
                //FIXME:
            }
            break;

        case s_resp_line_almost_done:
            if (ch == '\r')
            {
                m_nState = s_resp_line_almost_done;
            }
            else if (ch == '\n')
            {
                std::string phrase(status_phrase_begin, begin + at + i);
                std::cout << "status phrase:" << phrase << std::endl;
                std::cout << "request line done:" << std::endl;
                m_nState = s_resp_line_done;
            }
            break;

        case s_resp_line_done:
            if (ch == '\r')
            {
                m_nState = s_headers_done;
                break;
            }
            else if (ch == '\n')
            {
                m_nState = s_headers_done;
                break;
            }

            checkOrGoError(isAlpha(ch));
            header_key_begin = begin + at + i;
            m_nState = s_header_key_start;
            break;

        case s_requ_start:
            m_nState = s_requ_method; //FIXME:
            method_begin = begin + at + i;
            break;

        case s_requ_method_start:
            // method_begin = begin + at + i;
            //do nothing???
            //TODO:
            if (ch == ' ')
            {
            }
            checkOrGoError(isAlpha(ch));

            break;

        case s_requ_method:
            if (ch == ' ')
            {
                std::string method(method_begin, begin + at + i);
                std::cout << "method:" << method << std::endl;
                m_nState = s_requ_url_begin;
            }
            break;

        case s_requ_url_begin:
            if (isUrlChar(ch))
            {
                url_begin = begin + at + i;
                m_nState = s_requ_url;
            }
            else if (ch == ' ')
            {
                m_nState = s_requ_url_begin;
            }
            break;

        case s_requ_url:
            if (isUrlChar(ch))
            {
                m_nState = s_requ_url;
            }
            else if (ch == ' ')
            {
                //FIXME:
                std::string url(url_begin, begin + at + i);
                std::cout << "url:" << url << std::endl;
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
            std::cout << "http version:HTTP/" << m_nHttpVersionMajor << "."
                      << m_nHttpVersionMinor << std::endl;
            break;

        case s_requ_line_almost_done:
            checkOrGoError('\n');
            m_nState = s_requ_line_done;
            break;

        case s_requ_line_done:
            if (ch == '\r')
            {
                m_nState = s_headers_done;
                break;
            }
            else if (ch == '\n')
            {
                m_nState = s_headers_done;
                break;
            }

            checkOrGoError(isAlpha(ch));
            header_key_begin = begin + at + i;
            m_nState = s_header_key_start;
            break;

        case s_header_key_start:
            m_nState = s_header_key;
            break;

        case s_header_key:
            if (ch == ':')
            {
                std::string key(header_key_begin, begin + at + i);
                std::cout << "key:" << key << std::endl;
                m_nState = s_header_value_start;
            }
            break;

        case s_header_value_start:
            if (ch == ' ')
            {
                // m_nState = s_header_value_start;
            }
            else if (ch == '\r' || ch == '\n')
            {
                m_nState = s_error;
            }
            else
            {
                // std::cout
                header_value_begin = begin + at + i;
                m_nState = s_header_value;
            }
            break;

        case s_header_value:
            if (ch == '\r')
            {
                std::string value(header_value_begin, begin + at + i);
                std::cout << "value:" << value << std::endl;
                headers++;
                m_nState = s_header_almost_done;
            }
            if (ch == '\n')
            {
                m_nState = s_error;
            }
            break;

        case s_header_almost_done:
            checkOrGoError((ch == '\n'));
            m_nState = s_header_done;
            break;

        case s_header_done:
            if (isAlpha(ch))
            {
                header_key_begin = begin + at + i;
                m_nState = s_header_key_start;
            }
            else if (ch == '\r')
            {
                m_nState = s_headers_almost_done;
            }
            else if (ch == '\n')
            {
                m_nState = s_headers_done;
            }
            break;

        case s_headers_almost_done:
            checkOrGoError((ch == '\n'));
            std::cout << "headers Number:" << headers << std::endl;
            m_nState = s_headers_done;
            break;

        case s_headers_done:
            break;
        }
    }

error:
    std::cout << "parser error\n";
    return -1;
}