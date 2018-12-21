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

#include <tiny_base/log.h>
#include <tiny_base/api.h>
#include <tiny_base/configer.h>

#include <algorithm>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

typedef boost::property_tree::ptree ptree;
typedef boost::property_tree::ptree::iterator piterator;

//default configuration file.
std::string Configer::m_nFile = "./TinyWeb.conf";

void setConfigerFile(const std::string &file)
{
    Configer::getConfigerInstance().setConfigerFile(file);
}

int loadConfig(bool debug)
{
    return Configer::getConfigerInstance().loadConfig(debug);
}

Configer::Configer()
{
    LOG(Debug) << "class Configer constructor\n";
}

void Configer::setConfigerFile(const std::string &file)
{
    m_nFile = file;
}

int Configer::checkConfigerFile(const std::string &file)
{
    //TODO:
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>(file, root);

    items = root.get_child("develop");
    boost::property_tree::ptree basic = items.get_child("basic");
    boost::property_tree::ptree server = items.get_child("server");
    boost::property_tree::ptree log = items.get_child("log");

    // basic-config
    std::cout << "<basic-config>" << std::endl;
    int worker = basic.get<int>("worker", 8);
    std::string pidfile = basic.get<std::string>("pid", "/var/run/TinyWeb.pid");
    bool sendfile = basic.get<bool>("sendfile", true);
    std::string mimetype = basic.get<std::string>("mimetype", "mime.types");
    bool chunked = basic.get<bool>("chunked", true);
    bool gzip = basic.get<bool>("gzip", true);
    int gzip_level = basic.get<int>("gzip_level", 3);
    int gzip_buffers_4k = basic.get<int>("gzip_buffers_4k", 4);
    int gzip_min_len = basic.get<int>("gzip_min_len", 1024);

    ptree gzip_http_version = basic.get_child("gzip_http_version");
    for (auto it = gzip_http_version.begin(); it != gzip_http_version.end(); it++)
    {
        std::cout << "gzip_http_version: " << it->second.get_value<std::string>() << std::endl;
    }
    ptree gzip_mime_type = basic.get_child("gzip_mime_type");
    for (auto it = gzip_mime_type.begin(); it != gzip_mime_type.end(); it++)
    {
        std::cout << "gzip_mime_type: " << it->second.get_value<std::string>() << std::endl;
    }

    // server-config
    std::cout << "<server-config>" << std::endl;
    for (piterator it = server.begin(); it != server.end(); ++it)
    {
        int listen = it->second.get<int>("listen", 80);
        ptree servername = it->second.get_child("servername");
        std::cout << "servername:";
        for (piterator a = servername.begin(); a != servername.end(); a++)
        {
            std::cout << a->second.get_value<std::string>() << " ";
        }
        std::cout << std::endl;

        ptree indexpage = it->second.get_child("indexpage");
        for (piterator a = indexpage.begin(); a != indexpage.end(); a++)
        {
            std::cout << a->second.get_value<std::string>() << " ";
        }

        ptree errorpage = it->second.get_child("errorpage");
        for (piterator a = errorpage.begin(); a != errorpage.end(); a++)
        {
            ptree code = a->second.get_child("code");
            std::cout << "code:";
            for (auto b = code.begin(); b != code.end(); b++)
            {
                std::cout << b->second.get_value<int>() << " ";
            }
            std::cout << std::endl;
            std::cout << "path:" << a->second.get<std::string>("path", "") << std::endl;
            std::cout << "file:" << a->second.get<std::string>("file", "") << std::endl;
        }
        std::string www = it->second.get<std::string>("www", "");
    }

    // log-config
    std::string level = log.get<std::string>("level", "Info");
    std::string path = log.get<std::string>("path", "");
    std::string debugfile = log.get<std::string>("debugfile", "");
    std::string infofile = log.get<std::string>("infofile", "");
    std::string warnfile = log.get<std::string>("warnfile", "");
    std::string errorfile = log.get<std::string>("errorfile", "");
    std::string fatalfile = log.get<std::string>("fatalfile", "");
    return 0;
}

int Configer::loadConfig(bool debug)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>(m_nFile, root);

    items = debug ? root.get_child("develop") : root.get_child("product");
    boost::property_tree::ptree basic = items.get_child("basic");
    boost::property_tree::ptree fcgi = items.get_child("fcgi");
    boost::property_tree::ptree server = items.get_child("server");
    boost::property_tree::ptree log = items.get_child("log");

    // basic-config
    basicConf.worker = basic.get<int>("worker", 8);
    basicConf.pid = basic.get<std::string>("pid", "/var/run/TinyWeb.pid__");
    basicConf.sendfile = basic.get<bool>("sendfile", false);
    basicConf.mimetype = basic.get<std::string>("mimetype", "mime.types");
    basicConf.chunked = basic.get<bool>("chunked", false);
    basicConf.gzip = basic.get<bool>("gzip", false);
    basicConf.gzip_level = basic.get<int>("gzip_level", 2);
    basicConf.gzip_buffers_4k = basic.get<int>("gzip_buffers_4k", 3);
    basicConf.gzip_min_len = basic.get<int>("gzip_min_len", 2048);

    ptree gzip_http_version = basic.get_child("gzip_http_version");
    for (auto it = gzip_http_version.begin(); it != gzip_http_version.end(); it++)
    {
        std::string str = it->second.get_value<std::string>();
        int vers = 0;
        for (auto t:str)
        {
            if ('0'<=t && t<='9')
            {
                vers *= 10;
                vers += int(t - '0');
            }
        }
        basicConf.gzip_http_version.push_back(vers);
    }

    ptree gzip_mime_type = basic.get_child("gzip_mime_type");
    for (auto it = gzip_mime_type.begin(); it != gzip_mime_type.end(); it++)
    {
        std::string mime_type = it->second.get_value<std::string>();
        basicConf.gzip_mime_type.push_back(mime_type);
    }

    // fcgi-config
    fcgiConf.enable = fcgi.get<bool>("enable", false);
    fcgiConf.connect_timeout = fcgi.get<unsigned int>("connect_timeout", 1000);
    fcgiConf.send_timeout = fcgi.get<unsigned int>("send_timeout", 1000);
    fcgiConf.read_timeout = fcgi.get<unsigned int>("read_timeout", 1000);
    

    // server-config
    for (piterator it = server.begin(); it != server.end(); ++it)
    {
        ServerConfig server;
        server.listen = it->second.get<int>("listen", 80);
        server.www = it->second.get<std::string>("www", "");

        ptree servername = it->second.get_child("servername");
        for (piterator a = servername.begin(); a != servername.end(); a++)
        {
            server.servername.push_back(a->second.get_value<std::string>());
        }

        ptree indexpage = it->second.get_child("indexpage");
        for (piterator a = indexpage.begin(); a != indexpage.end(); a++)
        {
            server.indexpage.push_back(a->second.get_value<std::string>());
        }

        ptree errorpage_ptree = it->second.get_child("errorpage");
        for (piterator a = errorpage_ptree.begin(); a != errorpage_ptree.end(); a++)
        {
            errorpage page;
            page.path = a->second.get<std::string>("path", "");
            page.file = a->second.get<std::string>("file", "");

            ptree code = a->second.get_child("code");
            for (auto b = code.begin(); b != code.end(); b++)
            {
                page.code = b->second.get_value<int>();
                server.errorpages.push_back(page);
            }
        }

        ptree fcgi_ptree = it->second.get_child("fcgi");
        for (piterator a = fcgi_ptree.begin(); a != fcgi_ptree.end(); a++)
        {
            fcgi_t f;
            f.pattern= a->second.get<std::string>("pattern", "");
            f.path = a->second.get<std::string>("path", "");
            f.listen = a->second.get<std::string>("listen", "");

            ptree indexpage = a->second.get_child("indexpage");
            for (auto b = indexpage.begin(); b != indexpage.end(); b++)
            {
                std::string page = b->second.get_value<std::string>();
                f.indexpage.push_back(page);
            }
            server.fcgis.push_back(f);
        }

        serverConf.push_back(server);
    }

    // log-config
    logConf.level = log.get<std::string>("level", "Info");
    logConf.path = log.get<std::string>("path", "");
    logConf.debugfile = log.get<std::string>("debugfile", "");
    logConf.infofile = log.get<std::string>("infofile", "");
    logConf.warnfile = log.get<std::string>("warnfile", "");
    logConf.errorfile = log.get<std::string>("errorfile", "");
    logConf.fatalfile = log.get<std::string>("fatalfile", "");
    return 0;
}

const BasicConfig & Configer::getBasicConfig()
{
    return basicConf;
}

const ServerConfig & Configer::getServerConfig(const std::string &servername)
{
    int index = 0;
    for (; index<serverConf.size(); index++)
    {
        if (haveServerName(serverConf[index], servername))
            break;
    }

    if (index == serverConf.size())
    {
        std::cout << "not this servername(" << servername <<") config";
        index = 0;
    }
    return serverConf[index];
}

const LogConfig & Configer::getLogConfig()
{
    return logConf;
}

Configer::~Configer()
{
    LOG(Debug) << "class Configer destructor\n";
}
