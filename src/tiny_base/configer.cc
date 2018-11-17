/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/configer.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <algorithm>
#include <string>
#include <vector>

//default configuration file.
std::string Configer::m_nFile = "./TinyWeb.conf";
Reader Configer::m_nFileReader("./TinyWeb.conf");

void setConfigerFile(const std::string &file)
{
    Configer::getConfigerInstance().setConfigerFile(file);
}

bool loadConfig()
{
    return Configer::getConfigerInstance().loadConfig();
}

Configer::Configer()
{
    // LOG(Debug) << "class Configer constructor\n";
}

void Configer::setConfigerFile(const std::string &file)
{
    m_nFile = file;
}

bool Configer::loadConfig()
{
}

Configer::~Configer()
{
    // LOG(Debug) << "class Configer destructor\n";
}
