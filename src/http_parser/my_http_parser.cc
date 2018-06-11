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

#include "my_http_parser.h"

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
    s_requ_line_done,

    s_header_start,
    s_header_key_start,
    s_header_key,
    s_header_value_start,
    s_header_value,
    s_header_done,

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
            checkOrGoError(ch == 'H');
            m_nState = s_resp_H;
            break;

        case s_resp_H:
            checkOrGoError(ch == 'T');
            m_nState = s_resp_HT;
            break;

        case s_resp_HT:
            checkOrGoError(ch == 'T');

            m_nState = s_resp_HTT;
            break;
        case s_resp_HTT:
            checkOrGoError(ch == 'P');

            m_nState = s_resp_HTTP;
            break;

        case s_resp_HTTP:
            checkOrGoError(ch == '/');
            m_nState = s_resp_HTTP_slash;
            break;

        case s_resp_HTTP_slash:
            if (!('0' <= ch && ch <= '9'))
            {
                goto error;
            }
            m_nState = s_resp_version_major;
            m_nHttpVersionMajor = ch - '0';
            break;

        case s_resp_version_major:
            checkOrGoError(ch == '.');
            m_nState = s_resp_version_dot;
            break;

        case s_resp_version_dot:
            if (!('0' <= ch && ch <= '9'))
            {
                goto error;
            }
            m_nState = s_resp_version_minor;
            m_nHttpVersionMinor = ch - '0';
            break;

        case s_resp_version_minor:

            break;

        case s_resp_status_code_start:

            break;

        case s_resp_status_code:

            break;

        case s_resp_status_phrase_start:

            break;

        case s_resp_status_phrase:

            break;

        case s_resp_line_done:

            break;
        }
    }

error:
    std::cout << "parser error\n";
}
