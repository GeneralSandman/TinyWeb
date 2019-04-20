/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class Protocol---
 *This class is will used by class Server
 *The only thing we should do is to defind a new drive-Protocol
 *This class will is be used to 
 ****************************************
 *
 *
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <tiny_base/api.h>
#include <tiny_base/buffer.h>
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_core/callback.h>
#include <tiny_core/connection.h>
#include <tiny_core/time.h>

#include <boost/bind.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>

class Connection;
class Buffer;
class Factory;
class ClientPool;
class Protocol;

typedef Protocol* (*createProtocol)(void);

class Reflect {
private:
    static std::map<std::string, createProtocol> m_nProts;
    Reflect()
    {
        std::cout << "class Reflect constructor\n";
    }

public:
    static Reflect& getReflectInstance()
    {
        static Reflect r;
        return r;
    }
    Protocol* getProtocolByName(const std::string& name)
    {
        auto p = m_nProts.find(name);
        if (p == m_nProts.end())
            return nullptr;
        else
            return p->second();
    }
    void regist(const std::string& name, createProtocol f)
    {
        m_nProts[name] = f;
    }
    ~Reflect()
    {
        std::cout << "class Reflect destructor\n";
    }
};

class RegistAction {
public:
    RegistAction(const std::string& name, createProtocol f)
    {
        Reflect::getReflectInstance().regist(name, f);
    }
};

#define RegistProtocol(ProtocolName)      \
    ProtocolName* creator##ProtocolName() \
    {                                     \
        return new ProtocolName;          \
    }                                     \
    RegistAction regist##ProtocolName(#ProtocolName, (createProtocol)creator##ProtocolName)

std::string getName(Protocol* p);
Protocol* getInstanceByPointer(const Protocol* p);

/*-------Protocol------------*/

class Protocol {
public:
    Factory* m_pFactory;
    int m_nNumber;
    Connection* m_pConnection;

public:
    Protocol();
    void makeConnection();
    void getMessage(const std::string&);
    void writeComplete();
    void loseConnection();

    void sendMessage(const std::string&);
    void sendBuffer(buffer_t* buffer);
    void sendChain(const chain_t* chain);
    void closeProtocol();
    void closeProtocolAfter(int seconds);

    virtual void connectionMade();
    virtual void dataReceived(const std::string&);
    virtual void writeCompletely();
    virtual void connectionLost();
    virtual ~Protocol();

    friend class Factory;
};

/*-------ClientPoolProtocol------------*/

class ClientPoolProtocol : public Protocol {
private:
    ClientPool* m_pClientPool;

public:
    ClientPoolProtocol();
    void setClientPool(ClientPool* pool);
    void yield();

    virtual ~ClientPoolProtocol();
};


/*-------TestClientPoolProtocol------------*/

class TestClientPoolProtocol : public ClientPoolProtocol {
public:
    TestClientPoolProtocol();

    void connectionMade();
    void dataReceived(const std::string& data);
    void writeCompletely();
    void connectionLost();

    virtual ~TestClientPoolProtocol();
};

#endif // !PROTOCOL_H
