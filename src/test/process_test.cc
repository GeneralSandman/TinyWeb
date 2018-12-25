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

#include <tiny_core/process.h>
#include <tiny_core/socket.h> 

#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    Socket * listenSocket = new Socket(createNoBlockSocket());
    NetAddress listenAddress(9090);
    listenSocket->bindAddress(listenAddress);

    int listenFd = listenSocket->getFd();
    int socketPairFds[2] = { -1, -1 };
    int index = 1;
    Process* proc = new Process(std::to_string(index), index, socketPairFds);
    proc->setSignalHandlers();
    proc->createListenServer(listenFd);
    proc->start();

    delete proc;
    return 0;
}
