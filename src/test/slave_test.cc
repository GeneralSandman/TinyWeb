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
#include <tiny_http/http_parser.h> 


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

typedef std::pair<NetAddress, Socket*> NetSocketPair;

void test1()
{
    EventLoop *loop = new EventLoop();

    int i = 0;
    std::string name = "slave";
    Slave slave(loop,i,name);

    Socket *socket1;
    NetAddress address1("172.17.0.2:9090");

    // Create server1
    socket1 = new Socket(createNoBlockSocket());
    socket1->bindAddress(address1);
    NetSocketPair pair1(address1, socket1);
    slave.createListenServer(pair1);

    slave.work();

    delete socket1;
    delete loop;
}

void test2()
{
    EventLoop *loop = new EventLoop();

    int i = 0;
    std::string name = "slave";
    Slave slave(loop,i,name);

    Socket *socket1,*socket2;
    NetAddress address1("172.17.0.2:9090");
    NetAddress address2("172.17.0.2:9091");

    // Create server1
    socket1 = new Socket(createNoBlockSocket());
    socket1->bindAddress(address1);
    NetSocketPair pair1(address1, socket1);
    slave.createListenServer(pair1);
    // Create server2
    socket2 = new Socket(createNoBlockSocket());
    socket2->bindAddress(address2);
    NetSocketPair pair2(address2, socket2);
    slave.createListenServer(pair2);


    slave.work();

    delete socket1;
    delete socket2;
    delete loop;
}

void test3()
{
    EventLoop *loop = new EventLoop();

    int i = 0;
    std::string name = "slave";
    Slave slave(loop,i,name);

    Socket *socket1,*socket2,*socket3;
    NetAddress address1("172.17.0.2:9090");
    NetAddress address2("172.17.0.2:9091");
    NetAddress address3("172.17.0.2:9092");

    // Create server1
    socket1 = new Socket(createNoBlockSocket());
    socket1->bindAddress(address1);
    NetSocketPair pair1(address1, socket1);
    slave.createListenServer(pair1);

    // Create server2
    socket2 = new Socket(createNoBlockSocket());
    socket2->bindAddress(address2);
    NetSocketPair pair2(address2, socket2);
    slave.createListenServer(pair2);
    // Create server3
    socket3 = new Socket(createNoBlockSocket());
    socket3->bindAddress(address3);
    NetSocketPair pair3(address3, socket3);
    slave.createListenServer(pair3);


    slave.work();

    delete socket1;
    delete socket2;
    delete socket3;
    delete loop;
}

int main()
{
    initConfiger();
    headerMeaningInit();
    // test1();
    // test2();
    test3();
    return 0;
}
