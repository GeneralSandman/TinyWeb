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

#include "signalmanager.h"
#include "log.h"

Signal::Signal(int num, const std::string &name, const std::string& meaning, SignalCallback callback)
:m_nSignalNumber(num),
m_nName(name),
m_nMeaning(meaning),
m_nSignalCallback(callback)
{
    LOG(Debug) << "class Signal constructor\n";
}

Signal::~Signal()
{
    LOG(Debug) << "class Signal destructor\n";
}

SignalManager::SignalManager()
{
    LOG(Debug) << "class SignalManager constructor\n";
}

void SignalManager::addSignal() {}

void SignalManager::updateSignal() {}

void SignalManager::deleteSignal() {}

SignalManager::~SignalManager()
{
    LOG(Debug) << "class SignalManager destructor\n";
}