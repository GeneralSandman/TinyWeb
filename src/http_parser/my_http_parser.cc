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

enum state
{
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
    s_resp_line_done,

    s_requ_start,
    s_requ_method_start,
    s_requ_method,
    s_requ_url_begin,
    s_requ_schema,
    s_requ_schema_slash,
    s_requ_schema_slash_slash,
    s_requ_server_start,
    s_requ_server,
    s_requ_path,
    s_requ_query_string_start,
    s_requ_query_string,
    s_requ_fragment_start,
    s_requ_HTTP_start,
    s_requ_H,
    s_requ_HT,
    s_requ_HTT,
    s_requ_HTTP,
    s_requ_HTTP_slash,
    s_requ_version_major,
    s_requ_version_dot,
    s_requ_version_minor,
    s_resp_line_almost_done,
    s_requ_line_done,

    //heaser statue
    // s_header_start,
    s_header_key_start,
    s_header_key,
    s_header_value_start,
    s_header_value,
    s_header_almost_done,
    s_header_done,
    s_headers_done,

    s_body_start,
    s_body,
    s_body_done,

    s_chunk

};

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

int HttpParser::parseUrl(const std::string &stream, int &at, int len, Url *result)
{
    std::cout << "function parseUrl\n";
    memset(result, sizeof(Url), 0);
    
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
