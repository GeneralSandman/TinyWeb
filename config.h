/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*
****************************************
*
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <iostream>

class Reader;

class Configer
{
  private:
    std::string m_nFile;
    Reader *m_pFileReader;
    std::map<std::string, std::string> m_nValue;
    bool m_fParseLine(const std::string &, std::string &, std::string &);

  public:
    Configer(const std::string &config_file);
    void init();
    bool loadConfig();
    std::string getValue(const std::string &);
    void test()
    {
        for (auto t : m_nValue)
        {
            std::cout << t.first << "=" << t.second << std::endl;
        }
    }
    ~Configer();
};

#endif //