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

#include "defer.h"
#include "exception.h"

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
      m_nPreException("noException")
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
    m_nNextChain = true;
    m_fRunCallBacks();
}

void Defer::errorback(Exception &exc)
{
    m_nNextChain = false;
    m_nPreException = exc;
    m_fRunCallBacks();
}

Defer::~Defer()
{
    LOG(Debug) << "class Defer constructor\n";
}