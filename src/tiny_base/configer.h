/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---Configer Class---
 *We need to upgrade this class
 *in order to handle config file error
 ****************************************
 *
 */

#ifndef CONFIGER_H
#define CONFIGER_H

#include <map>
#include <string>
#include <list>
#include <iostream>

void setConfigerFile(const std::string &file);
bool loadConfig();

class Configer
{
    class Basic
    {
        int processpool;
        bool sendfile;
        std::string mimetype;
        bool gzip;
    };

    class Server
    {
        int listen;
        std::list<std::string> servername;
        std::string www;
        std::list<std::string> indexpage;
        std::list<std::string> errorpage;
    };

    class Log
    {
        std::string level; 
        std::string path;
        std::string debugfile;
        std::string infofile;
        std::string warnfile;
        std::string errorfile;
        std::string fatalfile;
    };

    private:
        static std::string m_nFile;
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
        ~Configer();
};

#endif //
