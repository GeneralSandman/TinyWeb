/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---Configer Class---
 *
 ****************************************
 *
 */

#ifndef CONFIGER_H
#define CONFIGER_H

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class BasicConfig {
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

class FcgiConfig {
public:
    bool enable;
    bool keep_connect;
    unsigned int connect_timeout;
    unsigned int send_timeout;
    unsigned int read_timeout;
};

typedef struct errorpage {
    unsigned int code;
    std::string path;
    std::string file;
} errorpage;

typedef struct fcgi_t {
    std::string pattern;
    std::string path;
    std::vector<std::string> indexpage;
    std::string listen;
} fcgi_t;

class ProxyConfig {
public:
    std::string name;
    std::string server_address;
    bool enable;
    bool keep_connect;
    unsigned int connect_timeout;
    unsigned int send_timeout;
    unsigned int read_timeout;
    unsigned int buffers_4k;
    std::vector<std::pair<std::string, std::string>> set_header;
};

class CacheConfig {
public:
    std::string name;
    std::string server_address;
    std::string path;
    std::vector<int> file_grade;
    unsigned long long space_max_size;
    unsigned long long expires;
};

class ServerConfig {
public:
    int listen;
    std::string www;
    std::vector<std::string> servername;
    std::vector<std::string> indexpage;
    std::vector<errorpage> errorpages;
    std::vector<fcgi_t> fcgis;
};

class LogConfig {
public:
    std::string level;
    std::string path;
    std::string debugfile;
    std::string infofile;
    std::string warnfile;
    std::string errorfile;
    std::string fatalfile;
};

void setConfigerFile(const std::string& file);
int loadConfig(bool debug);

class Configer {
private:
    static std::string m_nFile;
    BasicConfig basicConf;
    FcgiConfig fcgiConf;
    std::vector<ProxyConfig> proxyConf;
    std::vector<CacheConfig> cacheConf;
    std::vector<ServerConfig> serverConf;
    LogConfig logConf;
    std::unordered_map<std::string, std::string> mimeTypes;

    Configer();
    Configer(const Configer& c) {}

    bool haveProxyName(const ProxyConfig& conf, const std::string& proxyname)
    {
        return (conf.name == proxyname);
    }

    bool haveCacheName(const CacheConfig& conf, const std::string& cachename)
    {
        return (conf.name == cachename);
        // auto it = find(std::begin(conf.name),
        // std::end(conf.name),
        // cachename);
        // return (it != std::end(conf.name));
    }

    bool haveServerName(const ServerConfig& conf, const std::string& servername)
    {
        auto it = find(std::begin(conf.servername),
            std::end(conf.servername),
            servername);
        return (it != std::end(conf.servername));
    }

public:
    static Configer& getConfigerInstance()
    {
        static Configer ConfigerInstance;
        return ConfigerInstance;
    }

    void setConfigerFile(const std::string& file);
    int checkConfigerFile(const std::string& file);
    int loadConfig(bool debug = false);

    const BasicConfig& getBasicConfig();
    const FcgiConfig& getFcgiConfig();
    const ProxyConfig& getProxyConfig(const std::string& proxyname);
    const std::vector<ProxyConfig>& getProxyConfig();
    const CacheConfig& getCacheConfig(const std::string& cachename);
    const ServerConfig& getServerConfig(const std::string& servername);
    const std::vector<ServerConfig>& getServerConfig();
    const LogConfig& getLogConfig();

    std::string getMimeType(const std::string& file_type);

    ~Configer();
};

#endif //
