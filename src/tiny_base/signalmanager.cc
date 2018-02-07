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

#include <tiny_base/signalmanager.h>
#include <tiny_base/log.h>
// #include <tiny_struct/string_t.h>
#include <string>
#include <tiny_base/api.h>

Signal::Signal(int num, const std::string &name, const std::string &meaning, SignalCallback callback)
    : m_nNumber(num),
      m_nName(name),
      m_nMeaning(meaning),
      m_nCallback(callback)
{
    // LOG(Debug) << "class Signal constructor\n";
}

Signal::~Signal()
{
    // LOG(Debug) << "class Signal destructor\n";
}

SignalManager::SignalManager()
    : m_nNum(0)
{
    LOG(Debug) << "class SignalManager constructor\n";
}

void SignalManager::addSignal(const Signal &s)
{
    add_signal(s.m_nNumber, s.m_nCallback);
    m_nNum++;
}

void SignalManager::updateSignal(const Signal &s)
{
    add_signal(s.m_nNumber, s.m_nCallback);
}

void SignalManager::deleteSignal(const Signal &s)
{
    remove_signal(s.m_nNumber);
    //we need fill remove_signal
    m_nNum--;
}

SignalManager::~SignalManager()
{
    LOG(Debug) << "class SignalManager destructor\n";
}