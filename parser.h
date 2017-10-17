/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Parser---
*function:to parse the data have received and
*get line data or 
****************************************
*
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>

enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER
};

enum LINE_STATUS
{
    LINE_OK = 0,
    LINE_BAD,
    LINE_OPEN
};

enum HTTP_CODE
{
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
    FORBIDDEN_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
};

class Parser
{
  public:
    Parser(int connectfd);
    ~Parser();
    HTTP_CODE parse_content();
    int recv_data_continue();

  public:
    int m_nSockfd;

    char m_nBuf[4096];
    int m_nReadIndex;
    int m_nCheckIndex;
    CHECK_STATE m_nCheckStat;

    int m_nLineStart;

    std::string m_nMethod;
    std::string m_nUrl;
    std::string m_nHttpVersion;
    std::string m_nHost;

    LINE_STATUS m_fParseLine();
    HTTP_CODE m_fParseRequestLine();
    HTTP_CODE m_fParseHeader();
};

//-------HttpParser--------//

struct HttpRequestHeader
{
    std::string method;
    std::string url;
    std::string version;
};

struct HttpRequestContent
{
    std::string host;
    std::string connection;
    std::string user_agent;
    std::string accept;
    std::string cookie;
};

class HttpParser
{
  private:
    CHECK_STATE m_nCheckStat;

    bool m_fParseRequestHeader(const std::string &line, struct HttpRequestHeader *header);
    bool m_fParseRequestContent(const std::string &line, struct HttpRequestContent *content);

  public:
    HttpParser();
    bool parseRequestLine(const std::string &line,
                          struct HttpRequestHeader *header,
                          struct HttpRequestContent *content);

    ~HttpParser();
};

#endif