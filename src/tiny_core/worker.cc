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

#include <tiny_core/worker.h>
#include <tiny_base/log.h>

#include <unistd.h>

Worker::Worker(int num, const std::string &name)
    : m_nNumber(num),
      m_nName(name)
{
    LOG(Debug) << "class Worker constructor\n";
}

void Worker::work()
{
    LOG(Info) << "worker(" << m_nName << ")start to work\n";
    //wait client and handle some functions
    LOG(Info) << "worker(" << m_nName << ")sleep 3 seconds\n";
    sleep(3);
}

Worker::~Worker()
{
    LOG(Debug) << "class Worker destructor\n";
}

Master::Master(int num, const std::string &name)
    : Worker(num, name)
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

Slave::Slave(int num, const std::string &name)
    : Worker(num, name)
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
HttpListenWorker::HttpListenWorker(int num, std::string name)
    : Worker(num, name)
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