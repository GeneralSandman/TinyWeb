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

#include <tiny_core/eventloop.h>
#include <tiny_core/worker.h>
#include <tiny_base/log.h>

#include <unistd.h>

Worker::Worker(EventLoop *loop, int num, const std::string &name)
    : m_pEventLoop(loop),
      m_nNumber(num),
      m_nName(name)
{
    LOG(Debug) << "class Worker constructor\n";
}

void Worker::work()
{
    m_pEventLoop->loop();
}

Worker::~Worker()
{
    LOG(Debug) << "class Worker destructor\n";
}

Master::Master(EventLoop *loop, int num, const std::string &name)
    : Worker(loop, num, name)
{
    LOG(Debug) << "class Master constuctor\n";
}

void Master::work()
{
    
}

Master::~Master()
{
    LOG(Debug) << "class Master destuctor\n";
}

Slave::Slave(EventLoop *loop, int num, const std::string &name)
    : Worker(loop, num, name)
{
    LOG(Debug) << "class Slave constuctor\n";
}

void Slave::work()
{
}

Slave::~Slave()
{
    LOG(Debug) << "class Slave destructor\n";
}

/////////////
HttpListenWorker::HttpListenWorker(EventLoop *loop, int num, std::string name)
    : Worker(loop, num, name)
{
    LOG(Debug) << "class HttpListenWorker constructor\n";
}

void HttpListenWorker::work()
{
    LOG(Info) << "HttpListenWorker(" << m_nName << ")start to work\n";
}

HttpListenWorker::~HttpListenWorker()
{
    LOG(Debug) << "class HttpListenWorker destructor\n";
}