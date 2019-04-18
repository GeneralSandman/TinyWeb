/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class Connector---
 *This class has same function like Accepter.
 *It is used by client to connect server.
 *It will handler more errors.
 * reconnect server
 * server refuse to connect
 * 
 ****************************************
 *
 */

#ifndef CONNECTOR_T
#define CONNECTOR_T

#include <tiny_core/channel.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/socket.h>
#include <tiny_core/timerid.h>

class EventLoop;

// Distinguish connectionCallback in file callback.h
typedef boost::function<void(int, const NetAddress&, const NetAddress&)> newConnectionCallback;
const int MaxRetryDelayMs = 30 * 1000;
const int InitRetryDelayMs = 500;

class Connector {
private:
    enum States {
        Disconnected,
        Connecting,
        Connected
    };

    EventLoop* m_pEventLoop;
    // Socket *m_pConnectSocket;
    int m_nSockfd; // fd inside of Socket obj
    // Because this Socket obj have same fd with
    // Socket in Connection,they will close same fd tigger
    // close error.
    Channel* m_pConnectChannel;
    newConnectionCallback m_nNewConnectionCallback;

    NetAddress m_nHostAddress;
    NetAddress m_nServerAddress;
    States m_nState;

    bool m_nRetry;
    bool m_nKeepConnect;
    int m_nRetryTime;
    bool m_nConnect; // stop will disable that

    TimerId m_nRetryTimer;

    void m_fConnect();
    void m_fRetry();

    void m_fHandleWrite();
    void m_fHandleError();

    void m_fRemoveInvaildConnectChannel();
    void m_fRemoveInvaildConnectSocket();

    void m_fEstablishConnection();
    void m_fRetryConnect();

public:
    Connector(EventLoop*, const NetAddress&, const NetAddress&, bool retry, bool keepconnect);
    int getSocketFd() { return m_nSockfd; }
    void setConnectionCallback(newConnectionCallback c) { m_nNewConnectionCallback = c; }
    bool isRetry() { return m_nRetry; }
    bool isKeepConnect() { return m_nKeepConnect; }
    void start();
    void restart(); //invoke when the connection close after
    //establish, defferent from retry()
    void stop();
    ~Connector();
};

#endif // !CONNECTOR_T