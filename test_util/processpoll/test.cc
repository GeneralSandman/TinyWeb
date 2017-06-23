#include "processpoll.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <iostream>

using namespace std;
using namespace poll;
int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "argv error\n";
        exit(-1);
    }
    cout << "pid:" << getpid() << endl;
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listen_fd, 8);

    Poll * p=Poll::create(listen_fd);
    if(p){
        p->run();
        delete p;
    }

    close(listen_fd);

    return 0;
}