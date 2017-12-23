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
    Client *tcpClient = new Client(loop, factory);
    tcpClient->start();

    string serverip = "192.168.1.1:80";
    NetAddress serveraddress(serverip);

    bool retry = false;
    
    for (int i = 0; i < 10; i++)
    {
        tcpClient->connect(serveraddress, retry, 9898 + i);
    }

    loop->loop();

    delete loop;
    delete tcpClient;

    return 0;
}