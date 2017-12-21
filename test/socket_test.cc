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

#include "../socket.h"
#include "../netaddress.h"
#include "../api.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void test_socket1()
{
    NetAddress host(1111);
    Socket server(createSocket());
    server.bindAddress(host);
    server.listen();
    cout << "local address:" << host.getIpPort() << endl;

    while (1)
    {
        NetAddress per;
        int con = server.accept(per);
        cout << "connection address:" << per.getIpPort() << endl;
        close(con);
    }
}

void test_socket2()
{
    NetAddress host("127.0.0.1", 2222);
    Socket server(createSocket());
    server.bindAddress(host);
    server.listen();
    cout << "local address:" << host.getIpPort() << endl;

    while (1)
    {
        NetAddress per;
        int con = server.accept(per);
        cout << "connection address:" << per.getIpPort() << endl;
        close(con);
    }
}

void test_socket3()
{
    NetAddress host("127.0.0.1:3333");
    Socket server(createSocket());
    server.bindAddress(host);
    server.listen();
    cout << "local address:" << host.getIpPort() << endl;

    while (1)
    {
        NetAddress per;
        int con = server.accept(per);
        cout << "connection address:" << per.getIpPort() << endl;
        close(con);
    }
}

void test_socket4()
{
    char ip[] = "127.0.0.1";
    int port = 4444;
    struct sockaddr_in s;
    bzero(&s, sizeof(s));
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = inet_addr((const char *)ip);
    s.sin_port = htons(port);

    NetAddress host(s);
    Socket server(createSocket());
    server.bindAddress(host);
    server.listen();
    cout << "local address:" << host.getIpPort() << endl;

    while (1)
    {
        NetAddress per;
        int con = server.accept(per);
        cout << "connection address:" << per.getIpPort() << endl;
        close(con);
    }
}

void test_socket5()
{
}
int main()
{
    int i;
    cin >> i;
    if (i == 1)
    {
        test_socket1();
    }
    else if (i == 2)
    {
        test_socket2();
    }
    else if (i == 3)
    {
        test_socket3();
    }
    else if (i == 4)
    {
        test_socket4();
    }
}