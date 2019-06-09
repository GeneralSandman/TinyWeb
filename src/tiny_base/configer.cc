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

#include <tiny_base/api.h>
#include <tiny_base/configer.h>
#include <tiny_base/exception.h>
#include <tiny_base/log.h>

#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

using namespace boost::property_tree;

typedef boost::property_tree::ptree ptree;
typedef boost::property_tree::ptree::const_iterator piterator;

//default configuration file.
std::string Configer::m_nFile = "./TinyWeb.conf";

void setConfigerFile(const std::string& file)
{
    Configer::getConfigerInstance().setConfigerFile(file);
}

int loadConfig(bool debug)
{
    return Configer::getConfigerInstance().loadConfig(debug);
}

Configer::Configer()
{
    // LOG(Debug) << "class Configer constructor\n";
}

void Configer::setConfigerFile(const std::string& file)
{
    m_nFile = file;
}

int Configer::checkConfigerFile(const std::string& file)
{
    cacheConf.clear();
    serverConf.clear();
    mimeTypes.clear();

    boost::property_tree::ptree root;
    boost::property_tree::ptree items;

    try {
        boost::property_tree::read_json<boost::property_tree::ptree>(file, root);
    } catch (const std::exception& ex) {
        LOG(Debug) << "configeure-file("<<file<<") have invalid json formate\n";
        return -1;
    } catch (...) {
        LOG(Debug) << "configeure-file("<<file<<") parse error\n";
    }

    bool debug = false;
    items = debug ? root.get_child("develop") : root.get_child("product");
    boost::property_tree::ptree basic = items.get_child("basic");
    boost::property_tree::ptree fcgi = items.get_child("fcgi");
    boost::property_tree::ptree cache = items.get_child("cache");
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
    for (auto it = gzip_http_version.begin(); it != gzip_http_version.end(); it++) {
        std::string str = it->second.get_value<std::string>();
        int vers = 0;
        for (auto t : str) {
            if ('0' <= t && t <= '9') {
                vers *= 10;
                vers += int(t - '0');
            }
        }
        basicConf.gzip_http_version.push_back(vers);
    }

    ptree gzip_mime_type = basic.get_child("gzip_mime_type");
    for (auto it = gzip_mime_type.begin(); it != gzip_mime_type.end(); it++) {
        std::string mime_type = it->second.get_value<std::string>();
        basicConf.gzip_mime_type.push_back(mime_type);
    }

    // fcgi-config
    fcgiConf.enable = fcgi.get<bool>("enable", false);
    fcgiConf.keep_connect = fcgi.get<bool>("keep_connect", false);
    fcgiConf.connect_timeout = fcgi.get<unsigned int>("connect_timeout", 1000);
    fcgiConf.send_timeout = fcgi.get<unsigned int>("send_timeout", 1000);
    fcgiConf.read_timeout = fcgi.get<unsigned int>("read_timeout", 1000);

    // cache-config
    for (piterator it = cache.begin(); it != cache.end(); ++it) {
        CacheConfig cache;
        cache.name = it->second.get<std::string>("name", "");
        cache.server_address = it->second.get<std::string>("server_address", "");
        cache.path = it->second.get<std::string>("path", "");

        ptree file_grade_ptree = it->second.get_child("file_grade");
        for (piterator a = file_grade_ptree.begin(); a != file_grade_ptree.end(); a++) {
            cache.file_grade.push_back(a->second.get_value<int>());
        }

        cache.space_max_size = it->second.get<unsigned long long>("space_max_size", 0);
        cache.expires = it->second.get<unsigned long long>("expires", 0);

        cacheConf.push_back(cache);
    }

    // server-config
    for (piterator it = server.begin(); it != server.end(); ++it) {
        ServerConfig tmp;
        
        tmp.listen = it->second.get<int>("listen", 80);
        tmp.www = it->second.get<std::string>("www", "");

        ptree servername = it->second.get_child("servername");
        for (piterator a = servername.begin(); a != servername.end(); a++) {
            tmp.servername.push_back(a->second.get_value<std::string>());
        }

        ptree indexpage = it->second.get_child("indexpage");
        for (piterator a = indexpage.begin(); a != indexpage.end(); a++) {
            tmp.indexpage.push_back(a->second.get_value<std::string>());
        }

        ptree errorpage_ptree = it->second.get_child("errorpage");
        for (piterator a = errorpage_ptree.begin(); a != errorpage_ptree.end(); a++) {
            errorpage page;
            page.path = a->second.get<std::string>("path", "");
            page.file = a->second.get<std::string>("file", "");

            ptree code = a->second.get_child("code");
            for (auto b = code.begin(); b != code.end(); b++) {
                page.code = b->second.get_value<int>();
                tmp.errorpages.push_back(page);
            }
        }

        ptree fcgi_ptree = it->second.get_child("fcgi");
        for (piterator a = fcgi_ptree.begin(); a != fcgi_ptree.end(); a++) {
            fcgi_t f;
            f.pattern = a->second.get<std::string>("pattern", "");
            f.path = a->second.get<std::string>("path", "");
            f.listen = a->second.get<std::string>("listen", "");

            ptree indexpage = a->second.get_child("indexpage");
            for (auto b = indexpage.begin(); b != indexpage.end(); b++) {
                std::string page = b->second.get_value<std::string>();
                f.indexpage.push_back(page);
            }
            tmp.fcgis.push_back(f);
        }

        serverConf.push_back(tmp);
    }

    // log-config
    logConf.level = log.get<std::string>("level", "Info");
    logConf.path = log.get<std::string>("path", "");
    logConf.debugfile = log.get<std::string>("debugfile", "");
    logConf.infofile = log.get<std::string>("infofile", "");
    logConf.warnfile = log.get<std::string>("warnfile", "");
    logConf.errorfile = log.get<std::string>("errorfile", "");
    logConf.fatalfile = log.get<std::string>("fatalfile", "");

    // mimetype-config
    ptree roott;
    std::string file_type;
    std::string mime_type;
    boost::property_tree::read_json<boost::property_tree::ptree>(basicConf.mimetype, roott);
    for (piterator t = roott.begin(); t != roott.end(); t++) {
        mime_type = t->first;

        for (auto i = t->second.begin(); i != t->second.end(); i++) {
            file_type = i->second.get_value<std::string>();
            mimeTypes[file_type] = mime_type;
        }
    }

    return 0;
}

int Configer::loadConfig(bool debug)
{
    cacheConf.clear();
    serverConf.clear();
    mimeTypes.clear();

    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>(m_nFile, root);

    items = debug ? root.get_child("develop") : root.get_child("product");
    boost::property_tree::ptree basic = items.get_child("basic");
    boost::property_tree::ptree fcgi = items.get_child("fcgi");
    boost::property_tree::ptree proxy = items.get_child("proxy");
    boost::property_tree::ptree cache = items.get_child("cache");
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
    for (auto it = gzip_http_version.begin(); it != gzip_http_version.end(); it++) {
        std::string str = it->second.get_value<std::string>();
        int vers = 0;
        for (auto t : str) {
            if ('0' <= t && t <= '9') {
                vers *= 10;
                vers += int(t - '0');
            }
        }
        basicConf.gzip_http_version.push_back(vers);
    }

    ptree gzip_mime_type = basic.get_child("gzip_mime_type");
    for (auto it = gzip_mime_type.begin(); it != gzip_mime_type.end(); it++) {
        std::string mime_type = it->second.get_value<std::string>();
        basicConf.gzip_mime_type.push_back(mime_type);
    }

    // fcgi-config
    fcgiConf.enable = fcgi.get<bool>("enable", false);
    fcgiConf.keep_connect = fcgi.get<bool>("keep_connect", false);
    fcgiConf.connect_timeout = fcgi.get<unsigned int>("connect_timeout", 1000);
    fcgiConf.send_timeout = fcgi.get<unsigned int>("send_timeout", 1000);
    fcgiConf.read_timeout = fcgi.get<unsigned int>("read_timeout", 1000);

    // proxy-config
    for (piterator it = proxy.begin(); it != proxy.end(); ++it) {
        ProxyConfig proxy_tmp;
        proxy_tmp.name = it->second.get<std::string>("name", "");
        proxy_tmp.server_address = it->second.get<std::string>("server_address", "");
        proxy_tmp.enable = it->second.get<bool>("enable", false);
        proxy_tmp.keep_connect = it->second.get<bool>("keep_connect", false);
        proxy_tmp.connect_timeout = it->second.get<unsigned int>("connect_timeout", 0);
        proxy_tmp.send_timeout = it->second.get<unsigned int>("send_timeout", 0);
        proxy_tmp.read_timeout = it->second.get<unsigned int>("read_timeout", 0);
        proxy_tmp.buffers_4k = it->second.get<unsigned int>("buffers_4k", 0);

        ptree set_header_tree = it->second.get_child("set_header");
        for (piterator a = set_header_tree.begin(); a != set_header_tree.end(); a++) {
            std::pair<std::string, std::string> header_pair;
            header_pair.first = a->second.get<std::string>("header", "");
            header_pair.second = a->second.get<std::string>("value", "");

            proxy_tmp.set_header.push_back(header_pair);
        }

        proxyConf.push_back(proxy_tmp);
    }

    // cache-config
    for (piterator it = cache.begin(); it != cache.end(); ++it) {
        CacheConfig cache;
        cache.name = it->second.get<std::string>("name", "");
        cache.server_address = it->second.get<std::string>("server_address", "");
        cache.path = it->second.get<std::string>("path", "");

        ptree file_grade_ptree = it->second.get_child("file_grade");
        for (piterator a = file_grade_ptree.begin(); a != file_grade_ptree.end(); a++) {
            cache.file_grade.push_back(a->second.get_value<int>());
        }

        cache.space_max_size = it->second.get<unsigned long long>("space_max_size", 0);
        cache.expires = it->second.get<unsigned long long>("expires", 0);

        cacheConf.push_back(cache);
    }

    // server-config
    for (piterator it = server.begin(); it != server.end(); ++it) {
        ServerConfig tmp;

        tmp.listen = it->second.get<int>("listen", 80);
        tmp.www = it->second.get<std::string>("www", "");

        ptree servername = it->second.get_child("servername");
        for (piterator a = servername.begin(); a != servername.end(); a++) {
            tmp.servername.push_back(a->second.get_value<std::string>());
        }

        ptree indexpage = it->second.get_child("indexpage");
        for (piterator a = indexpage.begin(); a != indexpage.end(); a++) {
            tmp.indexpage.push_back(a->second.get_value<std::string>());
        }

        ptree errorpage_ptree = it->second.get_child("errorpage");
        for (piterator a = errorpage_ptree.begin(); a != errorpage_ptree.end(); a++) {
            errorpage page;
            page.path = a->second.get<std::string>("path", "");
            page.file = a->second.get<std::string>("file", "");

            ptree code = a->second.get_child("code");
            for (auto b = code.begin(); b != code.end(); b++) {
                page.code = b->second.get_value<int>();
                tmp.errorpages.push_back(page);
            }
        }

        ptree fcgi_ptree = it->second.get_child("fcgi");
        for (piterator a = fcgi_ptree.begin(); a != fcgi_ptree.end(); a++) {
            fcgi_t f;
            f.pattern = a->second.get<std::string>("pattern", "");
            f.path = a->second.get<std::string>("path", "");
            f.listen = a->second.get<std::string>("listen", "");

            ptree indexpage = a->second.get_child("indexpage");
            for (auto b = indexpage.begin(); b != indexpage.end(); b++) {
                std::string page = b->second.get_value<std::string>();
                f.indexpage.push_back(page);
            }
            tmp.fcgis.push_back(f);
        }

        serverConf.push_back(tmp);
    }

    // log-config
    logConf.level = log.get<std::string>("level", "Info");
    logConf.path = log.get<std::string>("path", "");
    logConf.debugfile = log.get<std::string>("debugfile", "");
    logConf.infofile = log.get<std::string>("infofile", "");
    logConf.warnfile = log.get<std::string>("warnfile", "");
    logConf.errorfile = log.get<std::string>("errorfile", "");
    logConf.fatalfile = log.get<std::string>("fatalfile", "");

    // mimetype-config
    ptree roott;
    std::string file_type;
    std::string mime_type;
    boost::property_tree::read_json<boost::property_tree::ptree>(basicConf.mimetype, roott);
    for (piterator t = roott.begin(); t != roott.end(); t++) {
        mime_type = t->first;

        for (auto i = t->second.begin(); i != t->second.end(); i++) {
            file_type = i->second.get_value<std::string>();
            mimeTypes[file_type] = mime_type;
        }
    }

    return 0;
}

const BasicConfig& Configer::getBasicConfig()
{
    return basicConf;
}

const FcgiConfig& Configer::getFcgiConfig()
{
    return fcgiConf;
}

const ProxyConfig& Configer::getProxyConfig(const std::string& proxyname)
{
    unsigned int index = 0;
    for (; index < proxyConf.size(); index++) {
        if (haveProxyName(proxyConf[index], proxyname))
            break;
    }

    if (index == proxyConf.size()) {
        std::cout << "not this proxyname(" << proxyname << ") config\n";
        index = 0;
    }
    return proxyConf[index];
}

const std::vector<ProxyConfig>& Configer::getProxyConfig()
{
    return proxyConf;
}

const CacheConfig& Configer::getCacheConfig(const std::string& cachename)
{
    unsigned int index = 0;
    for (; index < cacheConf.size(); index++) {
        if (haveCacheName(cacheConf[index], cachename))
            break;
    }

    if (index == cacheConf.size()) {
        std::cout << "not this cachename(" << cachename << ") config\n";
        index = 0;
    }
    return cacheConf[index];
}

const ServerConfig& Configer::getServerConfig(const std::string& servername)
{
    unsigned int index = 0;
    for (; index < serverConf.size(); index++) {
        if (haveServerName(serverConf[index], servername))
            break;
    }

    if (index == serverConf.size()) {
        std::cout << "not this servername(" << servername << ") config";
        index = 0;
    }
    return serverConf[index];
}

const std::vector<ServerConfig>& Configer::getServerConfig()
{
    return serverConf;
}

const LogConfig& Configer::getLogConfig()
{
    return logConf;
}

std::string Configer::getMimeType(const std::string& file_type)
{
    auto p = mimeTypes.find(file_type);
    if (p == mimeTypes.end()) {
        return "";
    } else {
        return mimeTypes[file_type];
    }
}

Configer::~Configer()
{
    // LOG(Debug) << "class Configer destructor\n";
}
