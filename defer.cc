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

Defer::Defer()
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
    bool preStatusIsSucc = true;

    Exception preException("noException");

    while (!m_nChains.empty())
    {
        Chain tmp = m_nChains.front();
        m_nChains.pop_front();
        callBack c = tmp.first;
        errorBack e = tmp.second;

        if (preStatusIsSucc)
        {
            try
            {
                c();
            }
            catch (Exception &e)
            {
                preException = e;
                preStatusIsSucc = false;
            }
            catch (...)
            {
                preStatusIsSucc = false;
            }
        }
        else
        {
            try
            {
                e(preException);
                //If no exception,set preStatusIsSucc as true;
                preStatusIsSucc = true;
            }
            catch (Exception &e)
            {
                preException = e;
                preStatusIsSucc = false;
            }
            catch (...)
            {
                preStatusIsSucc = false;
            }
        }
    }
}

void Defer::errorback(const std::string &std)
{
}

Defer::~Defer()
{
    LOG(Debug) << "class Defer constructor\n";
}