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

enum http_host_state HttpParser::parseHostChar(const char ch, enum http_host_state stat)
{
    switch (ch)
    {
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
    case s_http_userinfo_start:
    case s_http_userinfo: //http://@hostname/ is vaild
        if (ch == '@')
            return s_http_host_start;
        else if (isUserInfoChar(ch))
            return s_http_userinfo;
        break;

    case s_http_host_start:
        if (ch == '[') //is IPv6
            return s_http_host_v6_start;
        if (isHostChar(ch)) //is IPv4
            return s_http_host;
        break;

    case s_http_host_v6_start:
        //not finished
        break;

    case s_http_host:
        if (isHostChar(ch))
            return s_http_host;
        break;

    case s_http_host_v6:
        //not finished
        break;

    case s_http_host_v6_end:
        //not finished
        break;

    case s_http_host_v6_zone_start:
        break;
    case s_http_host_v6_zone:
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
    case ' ':
        return s_error;
        break;

    default:
        break;
    }

    switch (stat)
    {
    case s_requ_url_begin:
        if (ch == '/') //or ch=='*' :method CONNECT
            return s_requ_path;
        if (isAlpha(ch))
            return s_requ_schema;
        break;

    case s_requ_schema:
        if (isAlpha(ch))
            return stat;
        if (ch == ':')
            return s_requ_schema_slash;
        break;

    case s_requ_schema_slash:
        if (ch == '/')
            return s_requ_schema_slash_slash;
        break;

    case s_requ_schema_slash_slash:
        if (ch == '/')
            return s_requ_server_start;
        break;

    case s_requ_server_start:
        break;

    case s_requ_server:

        if (ch == '/')
        {
            return s_requ_path;
        }
        else if (ch == '@')
        {
            return s_requ_server_at;
        }
        else if (ch == '?')
        {
            return s_requ_query_string_start;
        }
        else if (ch == '#')
        {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_path:
        if (IS_URL_CHAR(ch))
        {
            return stat;
        }
        else if (ch == '?')
        {
            return s_requ_query_string_start;
        }
        else if (ch == '#')
        {
            return s_requ_fragment_start;
        }
        break;

    case s_requ_server_at:
        if (ch == '@')
            return s_error;
        break;

    case s_requ_query_string_start:
        if (IS_URL_CHAR(ch))
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

    case s_requ_query_string:

        break;

    case s_requ_fragment_start:
        if (IS_URL_CHAR(ch))
        {
            return s_requ_fragment;
        }

        break;

    case s_requ_fragment:
        if (IS_URL_CHAR(ch))
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

    default:

        break;
    }

    return s_error;
}

int HttpParser::parserHost(const std::string &stream,
                           int &at,
                           int len,
                           Url *result)
{
    if (stream.empty() && result->data == nullptr)
        return 1;
}

int HttpParser::parseUrl(const std::string &stream,
                         int &at,
                         int len,
                         Url *result)
{
    std::cout << "function parseUrl\n";
    memset(result, sizeof(Url), 0);

    result->data = (char *)stream.c_str();
    enum state stat = s_requ_url_begin;
    enum httpUrlField oldfield = HTTP_UF_MAX, field;
    bool found_at = false;

    for (int i = 0; i < len; i++)
    {
        stat = parseUrlChar(*(stream.c_str() + at + i), stat);
        // field = 0;

        switch (stat)
        {
        case s_error:
            return -1;
            break;

        /* Skip delimeters */
        case s_requ_schema_slash:
        case s_requ_schema_slash_slash:
        case s_requ_server_start:
        case s_requ_query_string_start:
        case s_requ_fragment_start:
            continue;

        case s_requ_server_at:
            found_at = true;
            break;

        case s_requ_schema:
            field = HTTP_UF_SCHEMA;

        case s_requ_server:
            field = HTTP_UF_HOST;
            break;

        case s_requ_path:
            field = HTTP_UF_PATH;
            break;

        case s_requ_query_string:
            field = HTTP_UF_QUERY;
            break;

        case s_requ_fragment:
            field = HTTP_UF_FRAGMENT;
            break;

        default:
            break;
        }

        if (field == oldfield)
        {
            result->fields[field].len++;
            continue;
        }

        result->fields[field].offset = i;
        result->fields[field].len = 1;

        result->field_set |= (1 << field);
        oldfield = field;
    }

    if (result->field_set & (1 << HTTP_UF_HOST))
    {
        if (parseHost(result))
            return -1;
    }
}

int HttpParser::execute(const std::string &stream, int &at, int len)
{
    std::cout << "function HttpParser::execute()\n";

    const char *begin = stream.c_str() + at;
    const char *url_begin = nullptr;
    const char *status_phrase_begin = nullptr;
    const char *header_key_begin = nullptr;
    const char *header_value_begin = nullptr;
    const char *body_begin = nullptr;
    const char *method_begin = nullptr;

    if (getErrno() != HPE_OK)
        return 0;

    if (len == 0)
    {
    }

    switch (m_nState)
    {
        // switch to some init state;
    }

    for (int i = 0; i < len; i++)
    {
        char ch = *(begin + i);

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
            std::cout << "get response http version:HTTP/" << m_nHttpVersionMajor << "."
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
                std::cout << "get http status code:" << m_nStatusCode << std::endl;
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
            status_phrase_begin = begin + i;
            m_nState = s_resp_status_phrase;
            break;

        case s_resp_status_phrase:
            if (ch == '\r')
            {
                m_nState = s_resp_line_almost_done;
            }
            else if (ch == '\n')
            {
                std::string phrase(status_phrase_begin, begin + i);
                std::cout << "get status phrase:" << phrase << std::endl;
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
                std::string phrase(status_phrase_begin, begin + i);
                std::cout << "get status phrase:" << phrase << std::endl;
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
            header_key_begin = begin + i;
            m_nState = s_header_key_start;
            break;

        case s_requ_start:
            m_nState = s_requ_method; //FIXME:
            method_begin = begin + i;
            break;

        case s_requ_method_start:
            // method_begin = begin + i;
            if (ch == ' ')
            {
            }
            checkOrGoError(isAlpha(ch));

            break;

        case s_requ_method:
            if (ch == ' ')
            {
                std::string method(method_begin, begin + i);
                std::cout << "method:" << method << std::endl;
                m_nState = s_requ_url_begin;
            }
            checkOrGoError(isAlpha(ch));
            break;

        case s_requ_url_begin:
            // checkOrGoError(isUrlChar(ch));
            url_begin = begin + i;
            // m_nState=s_r
            break;

        case s_requ_schema:
            break;

        case s_requ_schema_slash:
            checkOrGoError((ch == '/'));
            m_nState = s_requ_schema_slash_slash;
            break;

        case s_requ_schema_slash_slash:
            checkOrGoError((ch == '/'));
            m_nState = s_requ_server_start;
            break;

        case s_requ_server_start:
            break;

        case s_requ_server:
            break;

        case s_requ_path:
            checkOrGoError(isAlphaNum(ch)); //FIXME:

            if (ch == '?')
            {
                m_nState = s_requ_query_string_start;
            }
            break;

        case s_requ_query_string_start:

            break;

        case s_requ_query_string:
            break;

        case s_requ_fragment_start:
            break;

        case s_requ_HTTP_start:
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
            // checkOrGoError((ch == ' '));
            if (ch == '\r')
            {
            }
            else if (ch == '\n')
            {
                m_nState = s_requ_line_done;
            }
            std::cout << "get response http version:HTTP/" << m_nHttpVersionMajor << "."
                      << m_nHttpVersionMinor << std::endl;
            break;

        // case s_resp_line_almost_done:
        //     break;
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
            header_key_begin = begin + i;
            m_nState = s_header_key_start;
            break;

        case s_header_key_start:
            m_nState = s_header_key;
            break;

        case s_header_key:
            if (ch == ':')
            {
                std::string key(header_key_begin, begin + i);
                std::cout << "get a header key:" << key << std::endl;
                m_nState = s_header_value_start;
            }
            break;

        case s_header_value_start:
            if (ch == ' ')
            {
            }
            else if (isAlpha(ch) || ch == '"')
            {
                header_value_begin = begin + i;
                m_nState = s_header_value;
            }
            break;

        case s_header_value:
            if (ch == '\r')
            {
            }
            if (ch == '\n')
            {
                std::string value(header_value_begin, begin + i);
                std::cout << "get a header value:" << value << std::endl;
                m_nState = s_header_done;
            }
            break;

        case s_header_almost_done:
            break;

        case s_header_done:
            if (isAlpha(ch))
            {
                header_key_begin = begin + i;
                m_nState = s_header_key_start;
            }
            else if (ch == '\r')
            {
            }
            else if (ch == '\n')
            {
                m_nState = s_headers_done;
            }
            break;

        case s_headers_done:
            break;
        }
    }

error:
    std::cout << "parser error\n";
    return -1;
}
