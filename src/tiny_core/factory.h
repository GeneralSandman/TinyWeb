/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class Factory---
 *Main function is produces protocols by connection.
 *The lifetime of Connection is not control by it.
 ****************************************
 *
 */

#ifndef FACTORY_H
#define FACTORY_H

#include <tiny_core/protocol.h>
#include <tiny_core/connection.h>
#include <tiny_core/callback.h>

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <boost/bind.hpp>

class EventLoop;

/*-------------Factory------------*/
class Factory
{
    private:
        EventLoop *m_pLoop;
        int m_nNumProts;
        //int m_nActiveProts;
        const Protocol *m_pProtocol;
        const std::string m_nProtocolName;
        std::map<Connection *, Protocol *> m_nProtocols;

    public:
        Factory(EventLoop *m_pLoop, Protocol *prot);

        ConnectionCallback connectCallback()
        {
            //It is used by Server to set event callback;
            return ConnectionCallback(boost::bind(&Factory::createConnection,
                        this, _1));
        }

        MessageCallback getMessageCallback()
        {
            //It is used by Server to set event callback;
            return MessageCallback(boost::bind(&Factory::getMessage,
                        this, _1, _2, _3));
        }

        WriteCompleteCallback writeCompleteCallback()
        {
            //It is used by Server to set event callback;
            return WriteCompleteCallback(boost::bind(&Factory::writeComplete, this,_1));
        }

        CloseCallback closeConnectionCallback()
        {
            //It is used by Server to set event callback;
            return CloseCallback(boost::bind(&Factory::lostConnection,
                        this, _1));
        }

        void createConnection(Connection *newCon);
        void getMessage(Connection *con,
                Buffer *buf,
                Time time);
        void writeComplete(Connection *con);
        void lostConnection(Connection *con);

        void closeProtocol(Protocol *);                   //used by protocol
        void closeProtocolAfter(Protocol *, int seconds); //used by protocol

        virtual void buildProtocol(Protocol *newProt);
        virtual ~Factory();
};

/*-------------ServerFactory------------*/

class ServerFactory : public Factory
{
    private:
    public:
        ServerFactory(EventLoop *, Protocol *);
        virtual void buildProtocol(Protocol *newProt);
        virtual ~ServerFactory();
};

/*-------------ClientFactory------------*/

class ClientFactory : public Factory
{
    private:
    public:
        ClientFactory(EventLoop *, Protocol *);
        virtual void buildProtocol(Protocol *newProt);
        virtual ~ClientFactory();
};

#endif // FACTORY_H
