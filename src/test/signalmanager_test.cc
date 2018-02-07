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
// #include <tiny_struct/string_t.h>

#include <signal.h>
#include <iostream>
#include <vector>
using namespace std;

SignalManager man;

vector<Signal> s;

#include <sys/types.h>
#include <unistd.h>

void print(int i)
{
    cout << getpid() << i << endl;
}

SignalCallback c = &print;

#include <string>

static void parentSignalHandler(int sign);
static void childSignalHandler(int sign);

const vector<Signal> ParentSignals = {
    Signal(SIGINT, "SIGINT", "killAll", parentSignalHandler),
    Signal(SIGTERM, "SIGTERM", "killSoftly", parentSignalHandler),
    Signal(SIGHUP, "SIGHUP", "reload", parentSignalHandler),
    Signal(SIGCHLD, "SIGCHLD", "child dead", parentSignalHandler),
    Signal(SIGPIPE, "SIGPIPE", "pipe error", parentSignalHandler)};

const vector<Signal> ChildSignals = {
    Signal(SIGINT, "SIGINT", "killAll", childSignalHandler),
    Signal(SIGTERM, "SIGTERM", "killSoftly", childSignalHandler),
    Signal(SIGHUP, "SIGHUP", "reload", childSignalHandler),
    Signal(SIGCHLD, "SIGCHLD", "child dead", childSignalHandler),
    Signal(SIGPIPE, "SIGPIPE", "pipe error", childSignalHandler)};

static void parentSignalHandler(int sign)
{
    cout << "parent signal manager get signal:" << sign << endl;
    int index = 0;
    for (; index < ParentSignals.size(); index++)
    {
        if (ParentSignals[index].isSame(sign))
            break;
    }
    if (index == ParentSignals.size())
        return;

    switch (sign)
    {
    case SIGINT:
        interrupt = 1;
        break;
    case SIGTERM:
        term = 1;
        break;
    case SIGHUP:
        hup = 1;
        break;
    case SIGCHLD:
        child = 1;
        break;
    case SIGPIPE:
        pipe = 1;
        break;
    }
}

static void childSignalHandler(int sign)
{
    cout << "child signal manager get signal:" << sign << endl;
    int index = 0;
    for (; index < ParentSignals.size(); index++)
    {
        if (ParentSignals[index].isSame(sign))
            break;
    }
    if (index == ParentSignals.size())
        return;

    switch (sign)
    {
    case SIGINT:
        interrupt = 1;
        break;
    case SIGTERM:
        term = 1;
        break;
    case SIGHUP:
        hup = 1;
        break;
    case SIGCHLD:
        child = 1;
        break;
    case SIGPIPE:
        pipe = 1;
        break;
    }
}

void setSignalHandlers_parent()
{

    for (auto t : ParentSignals)
        man.addSignal(t);
}

void setSignalHandlers_child()
{
    for (auto t : ChildSignals)
        man.addSignal(t);
}

int main()
{
    int signal;
    cin >> signal;

    pid_t pid = fork();
    if (pid > 0)
    {
        setSignalHandlers_parent();
        sleep(3);
        kill(pid, signal);
        sleep(20);
        // pause();
    }
    else if (pid == 0)
    {
        setSignalHandlers_child();
        sleep(10);
        // pause();
    }
    else
    {
        cout << "fork error\n";
    }

    return 0;
}
