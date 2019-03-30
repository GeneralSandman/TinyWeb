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
#include <tiny_core/clientpool.h>
#include <tiny_core/connection.h>
#include <tiny_core/factory.h>
#include <tiny_core/protocol.h>

#include <vector>

std::string
getName(const Protocol* p)
{
    std::stringstream s;
    s << typeid(*p).name();
    std::string tmp(s.str().c_str());

    std::string res;
    for (unsigned int i = 0; i < tmp.size(); i++)
        if (!(tmp[i] >= 48 && tmp[i] <= 57))
            res += tmp[i];
    return res;
}

//----------Reflect api----------//
std::map<std::string, createProtocol> Reflect::m_nProts;

Protocol* getInstanceByPointer(const Protocol* p)
{
    return Reflect::getReflectInstance().getProtocolByName(getName(p));
}

//----------Protocol api----------//

Protocol::Protocol()
{
    // LOG(Debug) << "class Protocol constructor\n";
}

void Protocol::makeConnection()
{
    //It is used by Factory
    //user can't change this function.
    connectionMade();
}

void Protocol::getMessage(const std::string& data)
{
    //It is used by Factory
    //user can't change this function.
    dataReceived(data);
}

void Protocol::writeComplete()
{
    //It is used by Factory
    //user can't change this function.
    writeCompletely();
}

void Protocol::loseConnection()
{
    //It is used by Factory
    //user can't change this function.
    connectionLost();
}

void Protocol::sendMessage(const std::string& data)
{
    m_pConnection->send(data);
}

void Protocol::sendBuffer(buffer_t* buffer)
{
    m_pConnection->send(buffer);
}

void Protocol::sendChain(const chain_t* chain)
{
    m_pConnection->send(chain);
}

void Protocol::closeProtocol()
{
    m_pFactory->closeProtocol(this);
}

void Protocol::closeProtocolAfter(int seconds)
{
    m_pFactory->closeProtocolAfter(this, seconds);
}

void Protocol::connectionMade()
{
    //It can be override user.
}

void Protocol::dataReceived(const std::string& data)
{
    //It can be override by user.
}

void Protocol::writeCompletely()
{
    //It can be override by user.
}

void Protocol::connectionLost()
{
    //It can be override by user.
}

Protocol::~Protocol()
{
    // LOG(Debug) << "class Protocol destructor\n";
}

RegistProtocol(Protocol);

//----------end Protocol api----------//

//----------Protocol api----------//

ClientPoolProtocol::ClientPoolProtocol()
    : Protocol()
    , m_pClientPool(nullptr)
{
    LOG(Debug) << "class ClientPoolProtocol constructor\n";
}

void ClientPoolProtocol::setClientPool(ClientPool* pool)
{
    m_pClientPool = pool;
}

void ClientPoolProtocol::yield()
{
    LOG(Debug) << "class ClientPoolProtocol give up control to connection\n";

    if (nullptr != m_pClientPool) {
        m_pClientPool->closeProtocol(this);
    }
}

void ClientPoolProtocol::connectionMade()
{
    LOG(Info) << "ClientPoolProtocol connection made\n";

    LOG(Info) << "ClientPoolProtocol send message\n";
    std::string message = "zhenhuli";
    sendMessage(message);
}

void ClientPoolProtocol::dataReceived(const std::string& data)
{
    LOG(Info) << "ClientPoolProtocol data received:"
              << data << std::endl;

    // Give up control of this connection.
    yield();
}

void ClientPoolProtocol::writeCompletely()
{
    LOG(Info) << "ClientPoolProtocol write completely\n";
}

void ClientPoolProtocol::connectionLost()
{
    LOG(Info) << "ClientPoolProtocol connection lost\n";
}

ClientPoolProtocol::~ClientPoolProtocol()
{
    LOG(Debug) << "class ClientPoolProtocol destructor\n";
}

RegistProtocol(ClientPoolProtocol);

//----------end ClientPoolProtocol api----------//

