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
#include <tiny_base/configer.h> 
#include <tiny_core/slave.cc> 
#include <tiny_core/protocol.h> 
#include <tiny_core/factory.h> 
#include <tiny_core/eventloop.h>
#include <tiny_core/process.h>
#include <tiny_core/processpool.h>
#include <tiny_core/socket.h>
#include <tiny_core/netaddress.h>


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

int main()
{
    initConfiger();
    EventLoop *loop = new EventLoop();

    int i = 0;
    std::string name = "slave";
    int listen;
    Slave slave(loop,i,name);

    Socket *socket = new Socket(createNoBlockSocket());
    NetAddress address("172.17.0.2:9090");
    socket->bindAddress(address);
    listen = socket->getFd();

    slave.createListenServer(listen);
    slave.work();

    delete socket;
    delete loop;
    return 0;
}
