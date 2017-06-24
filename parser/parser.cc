#include "parser.h"
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>

namespace parser
{
Parser::Parser(int connectfd) : m_nSockfd(connectfd),
                                m_nReadIndex(0),
                                m_nCheckIndex(0),
                                m_nCheckStat(CHECK_STATE_REQUESTLINE),
                                m_nLineStart(0)
{
    memset(m_nBuf, '\0', sizeof(m_nBuf));
}

Parser::~Parser()
{
}
parser::HTTP_CODE Parser::parse_content()
{

    parser::LINE_STATUS linestatus = LINE_OK;
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
    }

    return res;
}

parser::LINE_STATUS Parser::m_fParseLine()
{
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

parser::HTTP_CODE Parser::m_fParseRequestLine()
{
    


    m_nCheckStat = CHECK_STATE_HEADER; //change the statue
    return NO_REQUEST;
}
parser::HTTP_CODE Parser::m_fParseHeader()
{
    char *linestart = m_nBuf + m_nLineStart;
    m_nLineStart = m_nCheckIndex; //start the new line
    if (*linestart == '\0')
    {
        return GET_REQUEST;
    }
    else if (strncasecmp(linestart, "Host:", 5) == 0)
    {
    }
    else if (strncasecmp(linestart, "Referer:", 8) == 0)
    {
    }
    else
    {
    }

    return NO_REQUEST; //not a complete request
}
}