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

#include "../client.h"
#include "../netaddress.h"
#include "../eventloop.h"
#include "../factory.h"

using namespace std;

int main()
{
    EventLoop *loop = new EventLoop();
    Protocol *clientProtocol = new TestClientProtocol();
    Factory *clientFactory = new ClientFactory(loop, clientProtocol);
    Client *tcpClient = new Client(loop, clientFactory);

    NetAddress serveraddress("127.0.0.1:9999");
    NetAddress clientaddress("127.0.0.1:9595");
    bool retry = false;
    bool keepconnect = false;
    tcpClient->start();
    tcpClient->connect(clientaddress, serveraddress, retry, keepconnect);

    loop->loop();

    delete loop;
    delete clientProtocol;
    delete clientFactory;
    delete tcpClient;
    return 0;
}