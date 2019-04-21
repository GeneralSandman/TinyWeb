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
#include <tiny_base/log.h>
#include <tiny_core/client.h>
#include <tiny_core/protocol.h>
#include <tiny_core/clientpool.h>
#include <tiny_core/connection.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/timerid.h>
#include <tiny_http/http_protocol.h>

using namespace std;


void test1()
{
    NetAddress clientAddress("0.0.0.0:9595");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestClientPoolProtocol();

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->addClient(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));

    loop->loop();

    delete protocol;
    delete clientPool;
    delete loop;
}

void test2()
{
    NetAddress clientAddress("0.0.0.0:9595");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestClientPoolProtocol();

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->addClient(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));

    loop->loop();

    delete protocol;
    delete clientPool;
    delete loop;
}

void test3()
{
    NetAddress clientAddress("0.0.0.0:9595");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);

    Protocol* protocol1 = new TestClientPoolProtocol();
    Protocol* protocol2 = new TestClientPoolProtocol();
    Protocol* protocol3 = new TestClientPoolProtocol();

    loop->runAfter(15, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->addClient(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol1));
    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol2));
    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol3));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));

    loop->loop();

    // delete obj in right order.
    delete clientPool;
    delete protocol1;
    delete protocol2;
    delete protocol3;
    delete loop;
}

void test4()
{
    NetAddress clientAddress("0.0.0.0");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestClientPoolProtocol();

    loop->runAfter(15, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.4:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->addClient(clientAddress, serverAddress, retry, keepconnect, 2);

    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(4, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(5, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(6, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(7, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(8, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(9, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(10, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(11, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));

    loop->loop();

    // delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
}

void test5()
{
    NetAddress clientAddress(9595);

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestClientPoolProtocol();

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress1("172.17.0.3:9090");
    NetAddress serverAddress2("172.17.0.4:9090");
    NetAddress serverAddress3("172.17.0.5:9090");
    
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->addClient(clientAddress, serverAddress1, retry, keepconnect);
    clientPool->addClient(clientAddress, serverAddress2, retry, keepconnect);
    clientPool->addClient(clientAddress, serverAddress3, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress2, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress2, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress2, protocol));

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress3, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress3, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress3, protocol));

    loop->loop();

    delete protocol;
    delete clientPool;
    delete loop;
}

int main()
{
    // test1();
    // test2();
    test3();
    // test4();
    // test5();
}

int main1() {
    std::map<std::string, std::string> con;

    NetAddress address1("172.17.0.3:9090");
    NetAddress address2("172.17.0.4:9090");
    NetAddress address3("172.17.0.5:9090");

    con[address1.getIpPort()] = "a";
    con[address2.getIpPort()] = "b";
    con[address3.getIpPort()] = "c";

    std::cout << "zhenhuli" << con[address1.getIpPort()] << std::endl;
    std::cout << "zhenhuli" << con[address2.getIpPort()] << std::endl;
    std::cout << "zhenhuli" << con[address3.getIpPort()] << std::endl;


    return 0;
}
