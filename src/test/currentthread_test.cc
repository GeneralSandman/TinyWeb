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

#include <tiny_base/api.h>
#include <tiny_core/currentthread.h>

#include <iostream>
#include <string>

using namespace std;

void *thr_fun(void *)
{
    pid_t main_pid = getpid();
    cout << "thr_fun pid:" << main_pid << endl;
}
int main()
{
    pid_t main_pid = getpid();
    cout << "main pid:" << main_pid << endl;

    pthread_t ntid;
    int res = pthread_create(&ntid, NULL, thr_fun, NULL);
    if (res != 0)
        handle_error("pthread_create error");

    sleep(1);

    return 0;
}