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
#include <tiny_base/buffer.h>
#include <tiny_core/client.h>
#include <tiny_core/clientpool.h>
#include <tiny_core/connection.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/timerid.h>
#include <tiny_http/http_protocol.h>

using namespace std;

class TestProtocol : public Protocol {
private:
    ClientPool* m_pClientPool;
public:
    TestProtocol(ClientPool* pool)
        : Protocol()
        , m_pClientPool(pool)
    {
        LOG(Debug) << "class TestProtocol constructor\n";
    }

    void yield()
    {
        LOG(Debug) << "class TestProtocol give up control to connection\n";

        m_pClientPool->closeProtocol(this);
    }

    virtual void connectionMade()
    {
        LOG(Info) << "TestProtocol connection made\n";

        LOG(Info) << "TestProtocol send message\n";
        std::string message = "zhenhuli";
        sendMessage(message);
    }

    virtual void dataReceived(const std::string& data)
    {
        LOG(Info) << "TestProtocol data received:"
            << data << std::endl;

        // Give up control of this connection.
        yield();
    }

    virtual void writeCompletely()
    {
        LOG(Info) << "TestProtocol write completely\n";
    }

    virtual void connectionLost()
    {
        LOG(Info) << "TestProtocol connection lost\n";
    }

    virtual ~TestProtocol()
    {
        LOG(Debug) << "class TestProtocol destructor\n";
    }
};

void test1()
{
    NetAddress clientAddress("0.0.0.0:9595");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestProtocol(clientPool);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9000");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->connect(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));


    loop->loop();

    // delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
}

void test2()
{
    NetAddress clientAddress("0.0.0.0:9595");

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestProtocol(clientPool);

    loop->runAfter(15, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.4:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->connect(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));
    // loop->runAfter(2, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));


    loop->loop();

    // delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
}

void test3()
{
    NetAddress clientAddress(9595);

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestProtocol(clientPool);

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress1("172.17.0.3:9000");
    NetAddress serverAddress2("172.17.0.4:9090");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->connect(clientAddress, serverAddress1, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));
    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress1, protocol));


    loop->loop();

    // delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
}

int main()
{
    // test();
    // test1();
    test2();
    // test3();
}
