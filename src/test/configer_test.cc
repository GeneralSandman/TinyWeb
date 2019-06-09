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
#include <tiny_base/log.h>

#include <iostream>

using namespace std;

void test1()
{
    std::string file;
    std::cin >> file;
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "load config successfull\n";
    else
        std::cout << "load config failed\n";
}

void test2()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";

    std::string servername = "dissigil.cn";
    std::string proxyname = "proxy_test1";
    std::string cachename = "cache_test";
    BasicConfig basic = configer.getBasicConfig();
    FcgiConfig fcgi = configer.getFcgiConfig();
    ServerConfig server = configer.getServerConfig(servername);
    ProxyConfig proxy = configer.getProxyConfig(proxyname);
    CacheConfig cache = configer.getCacheConfig(cachename);
    LogConfig log = configer.getLogConfig();

    std::cout << "======basic\n";
    std::cout << basic.worker << std::endl;
    std::cout << basic.pid << std::endl;
    std::cout << basic.sendfile << std::endl;
    std::cout << basic.mimetype << std::endl;
    std::cout << basic.chunked << std::endl;
    std::cout << basic.gzip << std::endl;
    std::cout << basic.gzip_buffers_4k << std::endl;
    std::cout << basic.gzip_min_len << std::endl;

    std::cout << "======fcgi-basic\n";
    std::cout << fcgi.enable << std::endl;
    std::cout << fcgi.keep_connect << std::endl;
    std::cout << fcgi.connect_timeout << std::endl;
    std::cout << fcgi.send_timeout << std::endl;
    std::cout << fcgi.read_timeout << std::endl;

    std::cout << "======fcgi-addvance\n";
    for (auto t : server.fcgis) {
        std::cout << "pattern: size:(" << t.pattern.size() << "),data(" << t.pattern << ")\n";
        if (regex_match("http://127.0.0.1/test/dynamic_get.php", t.pattern)) {
            std::cout << "1---------\n";
        }
        if (regex_match("sdsdfsphp", t.pattern)) {
            std::cout << "2---------\n";
        }
        if (regex_match("sdsdfs.phps", t.pattern)) {
            std::cout << "3---------\n";
        }
        std::cout << "path:" << t.path << std::endl;

        std::cout << "indexpage:";
        for (auto i : t.indexpage) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        
        std::cout << "listen:" << t.listen << std::endl;
    }


    std::cout << "======server\n";
    std::cout << server.listen << std::endl;

    std::cout << "======proxy\n";
    std::cout << proxy.name << std::endl;
    std::cout << proxy.enable << std::endl;
    std::cout << proxy.keep_connect << std::endl;
    std::cout << proxy.connect_timeout << std::endl;
    std::cout << proxy.send_timeout << std::endl;
    std::cout << proxy.read_timeout << std::endl;
    std::cout << proxy.buffers_4k << std::endl;
    for (auto t : proxy.set_header) {
        std::cout << t.first << "->" << t.second << std::endl;
    }



    std::cout << "======cache\n";
    std::cout << cache.name << std::endl;
    std::cout << cache.server_address << std::endl;
    std::cout << cache.path << std::endl;
    for (auto t: cache.file_grade) {
        std::cout << t << ",";
    }
    std::cout << std::endl;
    std::cout << cache.space_max_size << std::endl;
    std::cout << cache.expires << std::endl;

    std::cout << "======log\n";
    std::cout << log.level << std::endl;
    std::cout << log.path << std::endl;
    std::cout << log.debugfile << std::endl;
    std::cout << log.fatalfile << std::endl;

    std::cout << "======mimetype\n";
    std::vector<std::string> filetypes;
    filetypes.push_back("asdf");
    filetypes.push_back("html");
    filetypes.push_back("htm");
    filetypes.push_back("shtml");
    filetypes.push_back("jpg");
    filetypes.push_back("jpeg");
    filetypes.push_back("js");
    filetypes.push_back("css");

    for (auto t : filetypes) {
        std::cout << t << " " << configer.getMimeType(t) << std::endl;
    }
}

void test3()
{ //
    std::string a = "../TinyWeb.conf";
    setConfigerFile(a);
    if (loadConfig(true))
        std::cout << "load config successfull\n";
    else
        std::cout << "load config failed\n";

    // initLogger(debugfile,
    //            infofile,
    //            warnfile,
    //            errorfile,
    //            fatalfile,
    //            convertStringToLoglevel(loglevel)); //error used
    // //FIXME:
    LOG(Debug) << endl;
    LOG(Info) << endl;
    LOG(Warn) << endl;
    LOG(Error) << endl;
    LOG(Fatal) << endl;

    setDebugLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setInfoLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setWarnLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setErrorLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setFatalLogFile("/home/li/TinyWeb/log/TinyWeb.log");

    LOG(Debug) << "test configer_test" << endl;
    LOG(Info) << "test configer_test" << endl;
    LOG(Warn) << "test configer_test" << endl;
    LOG(Error) << "test configer_test" << endl;
    LOG(Fatal) << "test configer_test" << endl;
}

void test4()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();

    if (0 == configer.checkConfigerFile(file)) {
        std::cout << "file valid" << std::endl;
        configer.setConfigerFile(file);
    } else {
        std::cout << "file invalid" << std::endl;
        return;
    }

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";

    std::string servername = "dissigil.cn";
    std::string cachename = "cache_test";
    BasicConfig basic = configer.getBasicConfig();
    FcgiConfig fcgi = configer.getFcgiConfig();
    ServerConfig server = configer.getServerConfig(servername);
    CacheConfig cache = configer.getCacheConfig(cachename);
    LogConfig log = configer.getLogConfig();

    std::cout << "======basic\n";
    std::cout << basic.worker << std::endl;
    std::cout << basic.pid << std::endl;
    std::cout << basic.sendfile << std::endl;
    std::cout << basic.mimetype << std::endl;
    std::cout << basic.chunked << std::endl;
    std::cout << basic.gzip << std::endl;
    std::cout << basic.gzip_buffers_4k << std::endl;
    std::cout << basic.gzip_min_len << std::endl;

    std::cout << "======fcgi\n";
    std::cout << fcgi.enable << std::endl;
    std::cout << fcgi.keep_connect << std::endl;
    std::cout << fcgi.connect_timeout << std::endl;
    std::cout << fcgi.send_timeout << std::endl;
    std::cout << fcgi.read_timeout << std::endl;

    std::cout << "======server\n";
    std::cout << server.listen << std::endl;
    std::cout << "fcgi=";
    for (auto fcgi:server.fcgis) {
        std::cout << fcgi.pattern << " ";
    }
    std::cout << std::endl;

    std::cout << "======cache\n";
    std::cout << cache.name << std::endl;
    std::cout << cache.server_address << std::endl;
    std::cout << cache.path << std::endl;
    std::cout << cache.space_max_size << std::endl;
    std::cout << cache.expires << std::endl;

    std::cout << "======log\n";
    std::cout << log.level << std::endl;
    std::cout << log.path << std::endl;
    std::cout << log.debugfile << std::endl;
    std::cout << log.fatalfile << std::endl;

    std::cout << "======mimetype\n";
    std::vector<std::string> filetypes;
    filetypes.push_back("asdf");
    filetypes.push_back("html");
    filetypes.push_back("htm");
    filetypes.push_back("shtml");
    filetypes.push_back("jpg");
    filetypes.push_back("jpeg");
    filetypes.push_back("js");
    filetypes.push_back("css");

    for (auto t : filetypes) {
        std::cout << t << " " << configer.getMimeType(t) << std::endl;
    }
}

int main()
{
    // test1();
    test2();
    // test3();
    // test4();
    return 0;
}
