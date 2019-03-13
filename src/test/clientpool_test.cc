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
public:
    TestProtocol()
        : Protocol()
    {
        LOG(Debug) << "class TestProtocol constructor\n";
    }

    virtual void connectionMade()
    {
        LOG(Info) << "TestProtocol connection made\n";

        std::string message = "zhenhuli";
        sendMessage(message);
    }
    virtual void dataReceived(const std::string& data)
    {
        LOG(Info) << "TestProtocol data received:"
            << data << std::endl;
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
    NetAddress clientAddress(9595);

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestProtocol();

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9000");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->connect(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));


    loop->loop();

    //delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
    //basic test successfully.
}

void test2()
{
    NetAddress clientAddress(9595);

    EventLoop* loop = new EventLoop();
    ClientPool* clientPool = new ClientPool(loop, clientAddress);
    Protocol* protocol = new TestProtocol();

    loop->runAfter(10, std::bind(&EventLoop::quit, loop));

    NetAddress serverAddress("172.17.0.3:9000");
    bool retry = false;
    bool keepconnect = false;

    clientPool->start();
    clientPool->connect(clientAddress, serverAddress, retry, keepconnect);

    loop->runAfter(3, boost::bind(&ClientPool::doTask, clientPool, clientAddress, serverAddress, protocol));


    loop->loop();

    //delete obj in right order.
    delete protocol;
    delete clientPool;
    delete loop;
    //basic test successfully.
}

int main()
{
    // test();
    test1();
    // test2();
    // test3();
}
