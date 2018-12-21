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

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

class BasicConfig
{
    public:
        int worker;
        std::string pid;
        bool sendfile;
        std::string mimetype;
        bool chunked;
        bool gzip;
        int gzip_level;
        int gzip_buffers_4k;
        int gzip_min_len;
        std::vector<int> gzip_http_version;
        std::vector<std::string> gzip_mime_type;
};

class FcgiConfig
{
    public:
        bool enable;
        unsigned int connect_timeout;
        unsigned int send_timeout;
        unsigned int read_timeout;
};

typedef struct errorpage
{
    unsigned int code;
    std::string path;
    std::string file;
}errorpage;

typedef struct fcgi_t
{
    std::string pattern;
    std::string path;
    std::vector<std::string> indexpage;
    std::string listen;
};

class ServerConfig
{
    public:
        int listen;
        std::string www;
        std::vector<std::string> servername;
        std::vector<std::string> indexpage;
        std::vector<errorpage> errorpages;
        std::vector<fcgi_t> fcgis;
};

class LogConfig
{
    public:
        std::string level; 
        std::string path;
        std::string debugfile;
        std::string infofile;
        std::string warnfile;
        std::string errorfile;
        std::string fatalfile;
};

void setConfigerFile(const std::string &file);
int loadConfig(bool debug);

class Configer
{
    private:
        static std::string m_nFile;
        BasicConfig basicConf;
        FcgiConfig fcgiConf;
        std::vector<ServerConfig> serverConf;
        LogConfig logConf;

        Configer();
        Configer(const Configer &c) {}

        bool haveServerName(const ServerConfig &conf, const std::string &servername)
        {
            auto it = find(std::begin(conf.servername), 
                    std::end(conf.servername), 
                    servername);
            return (it != std::end(conf.servername));
        }

    public:
        static Configer &getConfigerInstance()
        {
            static Configer ConfigerInstance;
            return ConfigerInstance;
        }
        void setConfigerFile(const std::string &file);
        int checkConfigerFile(const std::string &file);
        int loadConfig(bool debug = false);

        const BasicConfig & getBasicConfig();
        const ServerConfig & getServerConfig(const std::string &servername);
        const LogConfig & getLogConfig();
        ~Configer();
};

#endif //
