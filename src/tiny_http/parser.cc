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

#include <tiny_http/http.h>
#include <tiny_http/parser.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>

Parser::Parser(int connectfd) : m_nSockfd(connectfd),
                                m_nReadIndex(0),
                                m_nCheckIndex(0),
                                m_nCheckStat(CHECK_STATE_REQUESTLINE),
                                m_nLineStart(0)
{
    memset(m_nBuf, '\0', sizeof(m_nBuf));
    LOG(Debug) << "class Parser constructor\n";
}

Parser::~Parser()
{
    LOG(Debug) << "class Parser destructor\n";
}

HTTP_CODE Parser::parse_content()
{

    LINE_STATUS linestatus = LINE_OK;
    while ((linestatus = m_fParseLine()) == LINE_OK)
    {

        switch (m_nCheckStat)
        {
        case CHECK_STATE_REQUESTLINE:
        {
            HTTP_CODE res = m_fParseRequestLine();
            if (res == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (res == GET_REQUEST)
            {
                break;
            }
            else
            {
                break;
            }
        }

        case CHECK_STATE_HEADER:
        {
            HTTP_CODE res = m_fParseHeader();
            if (res == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (res == GET_REQUEST)
            {
                return GET_REQUEST;
            }
            else if (NO_REQUEST)
            {
                break;
            }
            else
            { //error
                // return res;
                //this is no idea
            }
        }

        default:
        {
        }
        }
    }
    if (linestatus == LINE_OPEN)
    {
        return NO_REQUEST;
        //contiune recv data;
    }
    else if (linestatus == LINE_BAD)
    {
        return BAD_REQUEST;
    }
}

int Parser::recv_data_continue()
{
    int res = read(m_nSockfd, m_nBuf + m_nReadIndex, 4096 - m_nReadIndex);
    if (res == 0)
    {
        std::cout << "client has close connection\n";
    }
    else if (res == -1)
    {
        std::cout << "reading failes\n";
    }
    else
    {
        m_nReadIndex += res;
        std::cout << "get data\n";
    }

    return res;
}

LINE_STATUS Parser::m_fParseLine()
{
    std::cout << m_nBuf << std::endl;
    // for (char *i = m_nBuf; *i != '\0'; i++)
    //     printf("%d ", *i);
    char check;
    for (; m_nCheckIndex < m_nReadIndex; m_nCheckIndex++)
    {
        check = m_nBuf[m_nCheckIndex];
        if (check == '\r')
        {
            if ((m_nCheckIndex + 1) == m_nReadIndex)
            {
                return LINE_OPEN;
            }
            else if (m_nBuf[m_nCheckIndex + 1] == '\n')
            {
                m_nBuf[m_nCheckIndex++] = '\0';
                m_nBuf[m_nCheckIndex++] = '\0';
                return LINE_OK;
            }
            else
            {
                return LINE_BAD;
            }
        }
        else if (check == '\n')
        {
            if (m_nCheckIndex > 1 && m_nBuf[--m_nCheckIndex] == '\r')
            {
                m_nBuf[m_nCheckIndex++] = '\0';
                m_nBuf[m_nCheckIndex] = '\0';
                return LINE_OK;
            }
            else
            {
                return LINE_BAD;
            }
        }
    }
    return LINE_OPEN;
}

HTTP_CODE Parser::m_fParseRequestLine()
{
    char *begin = m_nBuf;
    char *end = strpbrk(begin, " ");
    if (!end)
    {
        return BAD_REQUEST;
    }
    for (char *i = begin; i < end; i++)
    {
        m_nMethod += *i;
    }
    *(end++) = '\0';

    begin = end;
    end = nullptr;
    end = strpbrk(begin, " ");
    if (!end)
    {
        return BAD_REQUEST;
    }
    for (char *i = begin; i < end; i++)
    {
        m_nUrl += *i;
    }
    *(end++) = '\0';

    begin = end;

    for (char *i = begin; *i != '\0'; i++)
    {
        m_nHttpVersion += *i;
    }

    // std::cout << "method:" << m_nMethod << std::endl;
    // std::cout << "url:" << m_nUrl << std::endl;
    // std::cout << "version:" << m_nHttpVersion << std::endl;

    m_nCheckStat = CHECK_STATE_HEADER; //change the statue
    return NO_REQUEST;
}
HTTP_CODE Parser::m_fParseHeader()
{
    char *linestart = m_nBuf + m_nLineStart;
    m_nLineStart = m_nCheckIndex; //start the new line
    if (*linestart == '\0')
    {
        return GET_REQUEST;
    }
    else if (strncasecmp(linestart, "Host:", 5) == 0)
    {
        char *end = strpbrk(linestart, "\r");
        for (char *i = linestart + 5; i < end; i++)
        {
            m_nHost += *i;
        }
    }
    else if (strncasecmp(linestart, "Referer:", 8) == 0)
    {
    }
    else
    {
    }

    return NO_REQUEST; //not a complete request
}

//---------HttpParser---------/

void HttpParser::m_fGetRequestLines(const std::string &s, std::vector<std::string> &res, std::string &restOpenLine)
{
    const char crlf[] = "\r\n";

    auto search_begin = s.begin();
    auto line_end = s.end();

    while ((line_end = std::search(search_begin, s.end(), crlf, crlf + 2)) != s.end())
    {
        std::string tmp(search_begin, line_end);
        search_begin = line_end + 2;

        res.push_back(tmp);
        if (tmp == "") //FIXME:
            break;
    }

    //stroe restOpenLine
    if (s.end() > search_begin)
    {
        restOpenLine.resize(s.end() - search_begin);
        std::copy(search_begin, s.end(), restOpenLine.begin());
    }
}

bool HttpParser::m_fParseRequestLine(const std::string &line,
                                     struct HttpRequestLine &res)
{
    std::vector<std::string> vec;
    splitString(line, " ", vec);
    if (vec.size() != 3)
        return false;
    //FIXME:upgrade with regex
    // for (auto t : vec)
    // std::cout << t;
    // std::cout << std::endl;
    res.method = vec[0];
    res.url = vec[1];
    res.version = vec[2];

    // std::cout << res.method << std::endl;
    return true;
}

bool HttpParser::m_fParseRequestHeader(const std::string &line,
                                       struct HttpRequestHeader &res)
{
    std::vector<std::string> vec;
    splitString(line, ": ", vec);
    if (vec.size() != 2)
        return false;

    transform(vec[0].begin(), vec[0].end(), vec[0].begin(), tolower);
    transform(vec[1].begin(), vec[1].end(), vec[1].begin(), tolower);

    // for (auto t : vec)
    // std::cout << t;
    // std::cout << std::endl;

    if (vec[0] == "host")
    {
        res.host = vec[1];
    }
    else if (vec[0] == "connection")
    {
        res.connection = vec[1];
    }
    //leave another optionFIXME:

    return true;
}

bool HttpParser::m_fParseRequestBody(const std::string &line,
                                     struct HttpRequestEntiyBody &res)
{
    res.text += line;
    return true;
}

HttpParser::HttpParser()
    : m_nCheckStat(Status_ParseLine)
{
    LOG(Debug) << "class HttpParser constructor\n";
}

bool HttpParser::parseRequest(const std::string &data,
                              struct HttpRequest &request)
{
    // request=lines+empty_line+body
    std::vector<std::string> lines;
    // std::cout << "last open line:" << m_nLastOpenLine.size() << std::endl;
    m_fGetRequestLines(data, lines, m_nLastOpenLine);
    for (auto line : lines)
    {
        // LOG(Debug) << line << std::endl;
        // std::cout << "line:-" << line << "-\n";

        if (Status_ParseLine == m_nCheckStat) //line
        {
            // std::cout << "1\n";
            bool res = m_fParseRequestLine(line, request.line);
            if (res)
            {
                // std::cout << "12\n";
                // printHttpRequestLine(request.line);
                m_nCheckStat = Status_ParseHeader;
            }
            else
                return false;
        }
        else if (Status_ParseHeader == m_nCheckStat) //header
        {

            // std::cout << "2\n";

            if (line != "")
            {
                // std::cout << "22\n";
                bool res = m_fParseRequestHeader(line, request.header);
                if (!res)
                    return false;
            }
            else //Encounter an empth line between header and body
            {
                // std::cout << "23\n";
                // printHttpRequestLine(request.line);

                // if (request.line.method == "GET")
                // {
                //     std::cout << "232\n";
                //     m_nCheckStat = Status_ParseLine;
                //     // return true;
                // }
                m_nCheckStat = Status_ParseBody;
                // return false; //we have to response a httpresponse
            }
        }
    }

    if (Status_ParseBody == m_nCheckStat) //body
    {
        // std::cout << "3\n";

        bool res = m_fParseRequestBody(m_nLastOpenLine, request.body);
        if (res)
            m_nCheckStat = Status_ParseLine;
        if (!res)
            return false;
    }
}

HttpParser::~HttpParser()
{
    LOG(Debug) << "class HttpParser destructor\n";
}