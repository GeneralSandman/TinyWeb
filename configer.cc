/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "configer.h"
#include "reader.h"
#include "api.h"
#include "log.h"

#include <algorithm>
#include <string>
#include <vector>

std::string Configer::m_nFile = "/TinyWeb.conf";
Reader Configer::m_nFileReader("/TinyWeb.conf");

void setConfigerFile(const std::string &file)
{
    Configer::getConfigerInstance().setConfigerFile(file);
}
bool loadConfig()
{
    return Configer::getConfigerInstance().loadConfig();
}
std::string getConfigValue(const std::string &key)
{
    return Configer::getConfigerInstance().getConfigValue(key);
}

void Configer::m_fInit()
{
    m_nValue["listen"] = "80";
    m_nValue["processpoll"] = "8";
    m_nValue["docs"] = "";
    m_nValue["hostname"] = "";
    m_nValue["loglevel"] = "Info";
    m_nValue["logpath"] = "";
    m_nValue["debugfile"] = "";
    m_nValue["infofile"] = "";
    m_nValue["warnfile"] = "";
    m_nValue["errorfile"] = "";
    m_nValue["fatalfile"] = "";
}

bool Configer::m_fParseLine(std::string &s,
                            std::string &key, std::string &value)
{
    eraseAllSpace(s);
    if (s[s.size() - 1] == '\n')
        s.erase(s.end() - 1);

    if (s.empty())
    {
        return true;
    }
    else if (s[0] == '#')
    {
        return true;
    }

    std::vector<std::string> argv;
    splitString(s, "=", argv);
    if (argv.size() > 2 || argv.size() == 0)
        return false;

    key.resize(argv[0].size());
    transform(argv[0].begin(), argv[0].end(), key.begin(), tolower);

    if (argv.size() == 2)
    {
        value.resize(argv[1].size());
        transform(argv[1].begin(), argv[1].end(), value.begin(), tolower);
    }

    return true;
}

Configer::Configer()
{
    m_fInit();
    LOG(Debug) << "class Configer constructor\n";
}

void Configer::setConfigerFile(const std::string &file)
{
    m_nFile = file;
    m_nFileReader.setFile(file);
}

bool Configer::loadConfig()
{
    std::map<std::string, std::string> value_tmp;

    while (m_nFileReader.haveMore())
    {
        std::string s, key, value;
        s = m_nFileReader.readLine();

        if (m_fParseLine(s, key, value))
        {
            if (key != "" && m_nValue.find(key) != m_nValue.end())
            {
                value_tmp[key] = value;
            }
        }
        else
        {
            return false;
        }
    }

    for (auto t : value_tmp)
        if (t.second != "")
            m_nValue[t.first] = t.second;
    return true;
    //successful
}

std::string Configer::getConfigValue(const std::string &key)
{
    if (m_nValue.find(key) != m_nValue.end())
        return m_nValue[key];
    return "";
}

Configer::~Configer()
{
    LOG(Debug) << "class Configer destructor\n";
}