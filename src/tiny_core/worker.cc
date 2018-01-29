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

Worker::Worker(int num, std::string name)
    : m_nNumber(num),
      m_nName(name)
{
    LOG(Debug) << "class Worker constructor\n";
}

Worker::Worker(const Worker &worker)
{
    m_nNumber = worker.m_nNumber;
    m_nName = worker.m_nName;
    LOG(Debug) << "class Worker constructor\n";
}

Worker &Worker::operator=(const Worker &worker)
{
    if (this != &worker)
    {
        m_nNumber = worker.m_nNumber;
        m_nName = worker.m_nName;
    }
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