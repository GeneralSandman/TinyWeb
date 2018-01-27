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

#include "http.h"

#include <iostream>
#include <string>
#include <vector>

enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER
};

enum ParseHttpResponseStatus
{
    Status_ParseLine = 0,
    Status_ParseHeader,
    Status_ParseBody
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

class HttpParser
{
  private:
    std::string m_nLastOpenLine;

    ParseHttpResponseStatus m_nCheckStat;
    void m_fGetRequestLines(const std::string &s,
                            std::vector<std::string> &res,
                            std::string &restOpenLine);
    bool m_fParseRequestLine(const std::string &line,
                             struct HttpRequestLine &res);
    bool m_fParseRequestHeader(const std::string &line,
                               struct HttpRequestHeader &res);
    bool m_fParseRequestBody(const std::string &line,
                             struct HttpRequestEntiyBody &res);

  public:
    HttpParser();
    bool parseRequest(const std::string &data,
                      struct HttpRequest &request);
    ~HttpParser();
};

#endif