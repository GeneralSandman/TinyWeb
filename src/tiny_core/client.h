/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---Client class---
 *This class have same function like Server
 ****************************************
 *
 */

#ifndef CLIENT_T
#define CLIENT_T

#include <tiny_core/callback.h>
#include <tiny_core/netaddress.h>

#include <map>
#include <set>

class EventLoop;
class Connector;
class Connection;
class Factory;

class Client {
private:
    bool m_nStarted;
    unsigned long long m_nConNum;

    EventLoop* m_pEventLoop;
    Factory* m_pFactory;
    //pair <hostAddress,peerAddress>
    typedef std::pair<NetAddress, NetAddress> AddressCouple;
    typedef std::pair<Connector*, Connection*> ConnectorCouple;
    std::map<AddressCouple, ConnectorCouple> m_nConnections;

    ConnectionCallback m_nConnectCallback;
    MessageCallback m_nMessageCallback;
    WriteCompleteCallback m_nWriteCompleteCallback;
    CloseCallback m_nCloseCallback;

    void m_fNewConnectionCallback(int, const NetAddress&,
        const NetAddress&);

    void m_fHandleClose(Connection*);

public:
    Client(EventLoop*, Factory*);
    //setXXXCallback():is a public function
    //but it always used by itself to set callback functions
    //which is provied by factory.
    void setConnectCallback(ConnectionCallback c)
    {
        m_nConnectCallback = c;
    }
    void setMessageCallback(MessageCallback c)
    {
        m_nMessageCallback = c;
    }
    void setWriteCompleteCallback(WriteCompleteCallback c)
    {
        m_nWriteCompleteCallback = c;
    }
    void setCloseCallback(CloseCallback c)
    {
        m_nCloseCallback = c;
    }
    void start();
    void connect(const NetAddress& hostAddress,
        const NetAddress& peeraddress,
        bool retry,
        bool keepconnect);
    void disconnect(const NetAddress& hostaddress,
        const NetAddress& peeraddress);
    void disconnectAll();
    void stop();
    ~Client();
};

#endif // !CLIENT_T
