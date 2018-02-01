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

#include <tiny_core/protocol.h>
#include <tiny_base/api.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <sstream>

using namespace std;



int main()
{
    const Protocol *p1, *p2, *p3, *p4;
    p1 = new Protocol;
    p2 = new DiscardProtocol;
    p3 = new EchoProtocol;
    p4 = new WebProtocol;

    cout << typeid(*p1).name() << endl;
    cout << typeid(*p2).name() << endl;
    cout << typeid(*p3).name() << endl;
    cout << typeid(*p4).name() << endl;

    Protocol *p5, *p6, *p7, *p8;
    // p5 =  Reflect::getReflectInstance().getProtocolByName(getName(p1));
    // p6 =  Reflect::getReflectInstance().getProtocolByName(getName(p2));
    // p7 =  Reflect::getReflectInstance().getProtocolByName(getName(p3));
    // p8 =  Reflect::getReflectInstance().getProtocolByName(getName(p4));

    p5 =  getInstanceByPointer(p1);
    p6 =  getInstanceByPointer(p2);
    p7 =  getInstanceByPointer(p3);
    p8 =  getInstanceByPointer(p4);

    cout << typeid(*p5).name() << endl;
    cout << typeid(*p6).name() << endl;
    cout << typeid(*p7).name() << endl;
    cout << typeid(*p8).name() << endl;

    delete p1;
    delete p2;
    delete p3;
    delete p4;

    delete p5;
    delete p6;
    delete p7;
    delete p8;

    return 0;
}