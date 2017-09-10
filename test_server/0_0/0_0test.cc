#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

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

    int data_recv = 0;

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buf[]="hello world\n";

    while (1)
    {

        int connectfd = accept(listen_fd, &client_addr, &client_len);
        write(connectfd,buf,strlen(buf));
        close(connectfd);
    }

    close(listen_fd);

    return 0;
}