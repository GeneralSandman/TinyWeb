#include <iostream>
#include <deque>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../log.h"
#include "../exception.h"

typedef boost::function<void()> callBack;
typedef boost::function<void(Exception &)> errorBack;

typedef std::pair<callBack, char **> callBackPair;
typedef std::pair<errorBack, char **> errorBackPair;

typedef std::pair<callBack, errorBack> Chain;

void defaultFun()
{
    // std::cout << "defaultFun()\n";
}

void defauleFunExce(Exception &e)
{
    // std::c
}

class Defer
{
  private:
    std::deque<callBack> m_nCallBackChains;
    std::deque<errorBack> m_nErrorBackChains;

    std::deque<Chain> m_nChains;

  public:
    Defer()
    {
        LOG(Debug) << "class Defer constructor\n";
    }
    void addCallBack(callBack c)
    {
        addCallBacks(c, defauleFunExce);
    }
    void addErrorBack(errorBack e)
    {
        addCallBacks(defaultFun, e);
    }
    void addCallBacks(callBack c, errorBack e)
    {
        m_nChains.push_back(Chain(c, e));
    }

    void runCallBacks(Exception &e)
    {
    }

    void callback()
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
    void errorback(const std::string &std)
    {
    }
    ~Defer()
    {
        LOG(Debug) << "class Defer constructor\n";
    }
};

void succ1()
{
    std::cout << "succ1():no throwing exception\n";
}

void fail1(Exception &exc)
{
    std::cout << "fail1():no throwing exception\n";
}

void succ2()
{
    std::cout << "succ2():throw exception[error3]\n";
    Exception e("error3");
    throw e;
}

void fail2(Exception &exc)
{
    std::cout << "fail2():catch exceptrion and no throwing exception\n";
    std::cout << exc.what() << std::endl;
}

void succ3()
{
    std::cout << "succ3():throw exception\n";
    Exception e("error4");
    throw e;
}

void fail3(Exception &exc)
{
    std::cout << "fail3():catch exceptrion and throw exception\n";
    std::cout << exc.what() << std::endl;
    Exception e("error5");
    throw e;
}

void doneSucc()
{
    std::cout << "chains done\n";
}

void doneFail(Exception &exc)
{
    std::cout << "chains done with error\n";
}

int main()
{
    Defer defer;

    defer.addCallBack(succ1);
    defer.addErrorBack(fail1);

    defer.addCallBack(succ2);
    defer.addErrorBack(fail2);

    defer.addCallBack(succ3);
    defer.addErrorBack(fail3);

    // defer.addCallBacks(doneSucc, doneFail);
    // defer.addCallBack(doneSucc);
    defer.addErrorBack(doneFail);

    defer.callback();
    return 0;
}

void fun()
{
    Exception e("111");
    throw e;
}

int __main()
{
    try
    {
        fun();
    }
    catch (Exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}