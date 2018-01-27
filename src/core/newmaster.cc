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

#include "processpool.h"
#include "newmaster.h"
#include "worker.h"
#include "log.h"

#include <string>

NewMaster::NewMaster()
    : m_pProcessPool(new ProcessPool())
{
    LOG(Debug) << "class NewMaster constructor\n";
}

void NewMaster::startWork()
{
}

void NewMaster::restartWork()
{
}

void NewMaster::stopAllWorkers()
{
}

void NewMaster::killAllWorkers()
{
}

Worker *NewMaster::createNewWorker()
{
    //invoked by ProcessPool
    int number = m_nWorkers.size();
    Worker *newWorker = new Worker(number, "worker" + std::to_string(number));
    m_nWorkers[number] = newWorker;
    return newWorker;
}

NewMaster::~NewMaster()
{
    LOG(Debug) << "class NewMaster destructor\n";
}
