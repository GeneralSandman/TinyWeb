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

int HttpParser::execute(const std::string &stream, int &at)
{
    std::cout << "function HttpParser::execute()\n";

    const char *begin = stream.c_str() + at;
    const char *url_begin = nullptr;
    const char *status_phrase_begin = nullptr;
    const char *header_key_begin = nullptr;
    const char *header_value_begin = nullptr;
    const char *body_begin = nullptr;
}

void HttpParser::setType(enum httpParserType type)
{
    m_nType = type;
}