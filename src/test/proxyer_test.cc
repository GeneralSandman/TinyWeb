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
#include <tiny_base/log.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/process.h>
#include <tiny_core/processpool.h>
#include <tiny_core/protocol.h>
#include <tiny_core/slave.cc>
#include <tiny_core/socket.h>
#include <tiny_core/timerid.h>
#include <tiny_http/http_parser.h>
#include <tiny_http/http_proxyer.h>

#include <iostream>
#include <string>

void initConfiger()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";
    std::cout << std::endl;
}

void test1()
{
    NetAddress clientAddress("0.0.0.0:9595");
    EventLoop* loop = new EventLoop();

    HttpProxyer* proxyer = new HttpProxyer(loop, clientAddress);

    TimerId timerid1 = loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:80");
    bool retry = false;
    bool keepconnect = false;

    int* test = new int;
    *test = 90090;
    auto connectCallback = [test]() {
        LOG(Debug) << "Testing connect source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    auto messageCallback = [test]() {
        LOG(Debug) << "Testing get message from source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    auto writeCompleteCallback = [test]() {
        LOG(Debug) << "Testing write completely source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    auto closeCallback = [test]() {
        LOG(Debug) << "Testing close connection with source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    proxy_callback_t* proxy_callback = new proxy_callback_t;
    proxy_callback->connectCallback = connectCallback; // send proxy request.
    proxy_callback->messageCallback = messageCallback; // parse proxy response.
    proxy_callback->writeCompleteCallback = writeCompleteCallback;
    proxy_callback->closeCallback = closeCallback;

    proxyer->start();

    TimerId timerid2 = loop->runAfter(3, boost::bind(&HttpProxyer::request, proxyer, serverAddress, proxy_callback));

    loop->loop();

    delete proxy_callback;
    delete proxyer;
    delete loop;
}

int main()
{
    initConfiger();
    headerMeaningInit();
    test1();
    // test2();
    // test3();
    return 0;
}
