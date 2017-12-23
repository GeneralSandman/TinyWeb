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

#include "../connector.h"
#include "../netaddress.h"
#include "../eventloop.h"

using namespace std;

int main()
{

    EventLoop *loop = new EventLoop();
    string serverip = "192.168.1.1:80";
    NetAddress serveraddress(serverip);

    bool retry = false;

    Connector *connector = new Connector(loop, serveraddress, retry, 9899);

    loop->loop();

    delete loop;
    delete connector;

    return 0;
}