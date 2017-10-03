#include "reader.h"
#include "api.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

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
    m_nFileStream.open(file, std::ios_base::in);
    if (m_nFileStream.fail())
        handle_error("fail not exists");
    m_nLine = 0;
    m_nCurrLine = 0;

    m_fCountLine();
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
}
