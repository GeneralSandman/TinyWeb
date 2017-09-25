#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv)
{

    const char *ip = "127.0.0.1";
    int port = 3333;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int connect_fd = connect(listen_fd, (const struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (connect_fd < 0)
    {
        cout << "connect error";
        exit(-1);
    }
    char buf[] = "information\n";
    write(connect_fd, (const void *)buf, strlen(buf));

    close(listen_fd);
    close(connect_fd);

    return 0;
}