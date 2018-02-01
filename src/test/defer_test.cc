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

#include <tiny_base/log.h>
#include <tiny_base/exception.h>
#include <tiny_core/defer.h>

#include <iostream>
#include <deque>
#include <boost/function.hpp>
#include <boost/bind.hpp>


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

    defer.addErrorBack(doneFail);

    defer.callback();
    return 0;
}