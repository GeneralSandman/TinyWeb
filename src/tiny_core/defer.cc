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

#include <tiny_core/defer.h>
#include <tiny_base/exception.h>

void defaultFun()
{
}

void defauleFunExce(Exception &e)
{
}

void Defer::m_fRunCallBacks()
{
    while (!m_nChains.empty())
    {
        Chain tmp = m_nChains.front();
        m_nChains.pop_front();
        callBack c = tmp.first;
        errorBack e = tmp.second;

        if (m_nNextChain)
        {
            try
            {
                c();
            }
            catch (Exception &e)
            {
                m_nPreException = e;
                m_nNextChain = false;
            }
            catch (...)
            {
                m_nNextChain = false;
            }
        }
        else
        {
            try
            {
                e(m_nPreException);
                //If no exception,set m_nNextChain as true;
                m_nNextChain = true;
            }
            catch (Exception &e)
            {
                m_nPreException = e;
                m_nNextChain = false;
            }
            catch (...)
            {
                m_nNextChain = false;
            }
        }
    }
}

Defer::Defer()
    : m_nNextChain(true),
      m_nPreException("noException"),
      m_nIsCalling(false),
      m_nCalled(false)
{
    LOG(Debug) << "class Defer constructor\n";
}

void Defer::addCallBack(callBack c)
{
    addCallBacks(c, defauleFunExce);
}

void Defer::addErrorBack(errorBack e)
{
    addCallBacks(defaultFun, e);
}

void Defer::addCallBacks(callBack c, errorBack e)
{
    m_nChains.push_back(Chain(c, e));
}

void Defer::callback()
{
    if (m_nIsCalling || m_nCalled)
        return;
    m_nNextChain = true;
    m_nIsCalling = true;
    m_nCalled = true;
    m_fRunCallBacks();
    m_nIsCalling = false;
}

void Defer::errorback(Exception &exc)
{
    if (m_nIsCalling || m_nCalled)
        return;
    m_nNextChain = false;
    m_nPreException = exc;
    m_nIsCalling = true;
    m_nCalled = true;
    m_fRunCallBacks();
    m_nIsCalling = false;
}

Defer::~Defer()
{
    LOG(Debug) << "class Defer constructor\n";
}