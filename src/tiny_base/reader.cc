/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include <tiny_base/reader.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

void Reader::m_fInit(void)
{
    m_nFileStream.open(m_nFile, std::ios_base::in);
    if (m_nFileStream.fail())
        handle_error("fail not exists");
    m_nLine = 0;
    m_nCurrLine = 0;
    m_fCountLine();
}

void Reader::m_fCountLine(void)
{
    std::string tmp;
    while (getline(m_nFileStream, tmp, '\n'))
    {
        m_nLine++;
    }
    m_nFileStream.clear();
    m_nFileStream.seekg(0, std::ios::beg);
}

Reader::Reader(const std::string &file)
{
    m_nFile = file;
    m_fInit();
    // LOG(Debug) << "class Reader constructor\n";
}

void Reader::setFile(const std::string &f)
{
    m_nFile = f;
    m_nFileStream.close();
    m_fInit();
}

std::string Reader::readLine(void)
{
    std::string result;
    getline(m_nFileStream, result, '\n');
    result += '\n';
    m_nCurrLine++;
    return result;
}

void Reader::resetToHead(void)
{
    m_nFileStream.clear();
    m_nFileStream.seekg(0, std::ios_base::beg);
    m_nCurrLine = 0;
}

Reader::~Reader()
{
    m_nFileStream.close();

    // LOG(Debug) << "class Reader destructor\n";
}
