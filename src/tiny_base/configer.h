/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*We need to upgrade this class
*in order to handle config file error
****************************************
*
*/

#ifndef CONFIGER_H
#define CONFIGER_H

#include <tiny_base/reader.h>

#include <map>
#include <string>
#include <iostream>

void setConfigerFile(const std::string &file);
bool loadConfig();
std::string getConfigValue(const std::string &key);

class Configer
{
  private:
    static std::string m_nFile;
    static Reader m_nFileReader;
    std::map<std::string, std::string> m_nValue;

    void m_fInitDefaultKeyValue();
    bool m_fParseLine(std::string &, std::string &, std::string &);
    Configer();
    Configer(const Configer &c) //disable
    {
    }

  public:
    static Configer &getConfigerInstance()
    {
        static Configer ConfigerInstance;
        return ConfigerInstance;
    }

    void setConfigerFile(const std::string &file);
    bool loadConfig();
    std::string getConfigValue(const std::string &);
    void test()
    {
        for (auto t : m_nValue)
        {
            std::cout << "-" << t.first << "=" << t.second << "-" << std::endl;
        }
    }
    ~Configer();
};

#endif //