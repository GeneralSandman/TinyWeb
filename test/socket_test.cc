#include <iostream>
#include <vector>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../socket.h"
#include "../netaddress.h"
#include "../api.h"

using namespace std;

void test_netaddress()
{
    char ip[] = "127.0.0.1";
    int port = 5555;
    struct sockaddr_in s;
    bzero(&s, sizeof(s));
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = inet_addr((const char *)ip);
    s.sin_port = htons(port);

    vector<NetAddress> add;

    NetAddress host1;
    NetAddress host2(2222);
    NetAddress host3("127.0.0.1", 3333);
    NetAddress host4("127.0.0.1:4444");
    NetAddress host5(s);

    add.push_back(host1);
    add.push_back(host2);
    add.push_back(host3);
    add.push_back(host4);
    add.push_back(host5);

    for (int i = 0; i < add.size(); i++)
    {
        cout << "--------\n";
        cout << add[i].getIp() << endl;
        cout << add[i].getPort() << endl;
        cout << add[i].getIpPort() << endl;
        cout << "--------\n";
    }
}

int main()
{
    NetAddress host3("127.0.0.1", 3333);
    Socket server(createSocket());
    server.bindAddress(host3);
    server.listen();

    while (1)
    {
        NetAddress per;
        int con = server.accept(per);

        cout << per.getIp() << endl;
        cout << per.getPort() << endl;
        cout << per.getIpPort() << endl;

        close(con);
    }
}