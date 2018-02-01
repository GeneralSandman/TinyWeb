/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include <tiny_core/connector.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/eventloop.h>

#include <boost/bind.hpp>

using namespace std;

EventLoop *loop = nullptr;

void connectionCallback(int sockfd, const NetAddress &localaddress,
                        const NetAddress &serveraddress)
{
    std::cout << "establish connection[" << localaddress.getIpPort()
              << "---" << serveraddress.getIpPort() << "]\n";
    std::cout << "close after 10 seconds\n";
    loop->runAfter(10, std::bind(&EventLoop::quit, loop));
}

//in this file,we can't test keepconnect priority,
//because close callback must handle connection.
//client_test.cc test the arg keepconnect.

void test1()
{
    //no retry ,no keepconnect
    loop = new EventLoop();
    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress localaddress("127.0.0.1:9555");
    bool retry = false;
    bool keepconnect = false;

    Connector *connector = new Connector(loop, localaddress, serveraddress,
                                         retry, keepconnect);
    connector->setConnectionCallback(boost::bind(connectionCallback, _1, _2, _3));
    connector->start();
    loop->loop();

    //we must delete the obj in right order.
    //can't delete loop firstly.
    delete connector;
    delete loop;
}

void test2()
{
    //retry , no keepconnect
    loop = new EventLoop();
    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress localaddress("127.0.0.1:9555");
    bool retry = true;
    bool keepconnect = false;

    Connector *connector = new Connector(loop, localaddress, serveraddress,
                                         retry, keepconnect);
    connector->setConnectionCallback(boost::bind(connectionCallback, _1, _2, _3));
    connector->start();
    loop->loop();

    delete connector;
    delete loop;
}

void test3()
{
    EventLoop *loop = new EventLoop();
    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress localaddress("127.0.0.1:9555");
    bool retry = false;
    bool keepconnect = false;

    Connector *connector = new Connector(loop, localaddress, serveraddress,
                                         retry, keepconnect);
    connector->setConnectionCallback(boost::bind(connectionCallback, _1, _2, _3));
    connector->start();

    loop->runAfter(5, boost::bind(&Connector::stop, connector));
    loop->runAfter(10, boost::bind(&EventLoop::quit, loop));

    loop->loop();
    delete loop;
    delete connector;
}

void test4()
{
    EventLoop *loop = new EventLoop();
    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress localaddress("127.0.0.1:9555");
    bool retry = false;
    bool keepconnect = false;

    Connector *connector = new Connector(loop, localaddress, serveraddress,
                                         retry, keepconnect);
    connector->setConnectionCallback(boost::bind(connectionCallback, _1, _2, _3));
    connector->start();

    loop->runAfter(5, boost::bind(&Connector::stop, connector));
    loop->runAfter(10, boost::bind(&EventLoop::quit, loop));

    loop->loop();
    delete connector;
    delete loop;
}

void test5()
{
    EventLoop *loop = new EventLoop();
    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress localaddress("127.0.0.1:9555");
    bool retry = false;
    bool keepconnect = false;

    Connector *connector = new Connector(loop, localaddress, serveraddress,
                                         retry, keepconnect);
    connector->setConnectionCallback(boost::bind(connectionCallback, _1, _2, _3));
    connector->start();

    loop->runAfter(5, boost::bind(&Connector::stop, connector));
    loop->runAfter(10, boost::bind(&EventLoop::quit, loop));

    loop->loop();
    delete connector;
    delete loop;
}

int main()
{
    // test1();
    test2();
    // test3();//can't use
    // test4();//can't use
    // test5();//can't use
    return 0;
}