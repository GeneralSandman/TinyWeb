/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class Connection---
 *
 ****************************************
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <tiny_base/buffer.h>
#include <tiny_base/memorypool.h> 
#include <tiny_core/callback.h>
#include <tiny_core/channel.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/time.h>

#include <boost/function.hpp>

class Connection;
class Socket;
class EventLoop;

enum Connection_State {
    Connecting,
    Connected,
    DisConnecting,
    DisConnected
};

class Connection {
private:
    EventLoop* m_pEventLoop;
    Connection_State m_nState;
    Socket* m_pConnectSocket; //have difference between ListenSocket and connectSocket
    Channel* m_pChannel;      //handle connect fd

    ConnectionCallback m_nConnectCallback;
    MessageCallback m_nMessageCallback;
    WriteCompleteCallback m_nWriteCompleteCallback;
    CloseCallback m_nCloseCallback;

    Buffer m_nInputBuffer;
    Buffer m_nOutputBuffer;

    NetAddress m_nLocalAddress;
    NetAddress m_nPeerAddress;

    void m_fHandleRead(Time arrive);
    void m_fHandleWrite();
    void m_fHandleClose();
    void m_fHandleError();

    void m_fShutdownWrite();

public:
    Connection(EventLoop*, int, const NetAddress&, const NetAddress&);

    void send(const std::string& message);
    void send(buffer_t* buffer);
    void send(const chain_t* chain);

    void sendfile(int inputFd, off_t* offset, size_t count);

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

    void shutdownWrite() { m_fShutdownWrite(); }
    void establishConnection();
    void destoryConnection();

    NetAddress getLocalAddress()
    {
        return m_nLocalAddress;
    }
    NetAddress getPeerAddress()
    {
        return m_nPeerAddress;
    }

    ~Connection();
};

#endif // !CONNECTION_H
