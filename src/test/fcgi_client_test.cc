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

#include <tiny_base/buffer.h>
#include <tiny_core/client.h>
#include <tiny_core/connection.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/timerid.h>
#include <tiny_http/http_protocol.h>
#include <tiny_http/http_model_fcgi.h>

using namespace std;

void test1()
{
    //no retry , no keepconnect
    EventLoop* loop = new EventLoop();
    Protocol* clientProtocol = new FcgiClientProtocol();
    Factory* clientFactory = new ClientFactory(loop, clientProtocol);
    Client* tcpClient = new Client(loop, clientFactory);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serveraddress("172.17.0.3:9000");
    NetAddress clientaddress(9595);
    bool retry = false;
    bool keepconnect = false;
    tcpClient->start();
    tcpClient->connect(clientaddress, serveraddress, retry, keepconnect);

    loop->loop();

    //delete obj in right order.
    delete tcpClient;
    delete clientFactory;
    delete clientProtocol;
    delete loop;
}

void test2()
{
    //no retry , no keepconnect
    EventLoop* loop = new EventLoop();
    Protocol* clientProtocol = new FcgiClientProtocol();
    Factory* clientFactory = new ClientFactory(loop, clientProtocol);
    Client* tcpClient = new Client(loop, clientFactory);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serveraddress("172.17.0.3:9000");
    NetAddress clientaddress(9595);
    bool retry = true;
    bool keepconnect = false;
    tcpClient->start();
    tcpClient->connect(clientaddress, serveraddress, retry, keepconnect);

    loop->loop();

    //delete obj in right order.
    delete tcpClient;
    delete clientFactory;
    delete clientProtocol;
    delete loop;
}

void test3()
{
    //no retry , no keepconnect
    EventLoop* loop = new EventLoop();
    Protocol* clientProtocol = new FcgiClientProtocol();
    Factory* clientFactory = new ClientFactory(loop, clientProtocol);
    Client* tcpClient = new Client(loop, clientFactory);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serveraddress("172.17.0.3:9000");
    NetAddress clientaddress(9595);
    bool retry = false;
    bool keepconnect = true;
    tcpClient->start();
    tcpClient->connect(clientaddress, serveraddress, retry, keepconnect);

    loop->loop();

    //delete obj in right order.
    delete tcpClient;
    delete clientFactory;
    delete clientProtocol;
    delete loop;
}

void test4()
{
    //no retry , no keepconnect
    EventLoop* loop = new EventLoop();
    Protocol* clientProtocol = new FcgiClientProtocol();
    Factory* clientFactory = new ClientFactory(loop, clientProtocol);
    Client* tcpClient = new Client(loop, clientFactory);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serveraddress("172.17.0.3:9000");
    NetAddress clientaddress(9595);
    bool retry = true;
    bool keepconnect = true;
    tcpClient->start();
    tcpClient->connect(clientaddress, serveraddress, retry, keepconnect);

    loop->loop();

    //delete obj in right order.
    delete tcpClient;
    delete clientFactory;
    delete clientProtocol;
    delete loop;
}

int main()
{
    int i;
    std::cout << "select test:";
    std::cin >> i;

    if (i == 1) {
        test1();
    } else if (i == 2) {
        test2();
    } else if (i == 3) {
        test3();
    } else if (i == 4) {
        test4();
    }
}
