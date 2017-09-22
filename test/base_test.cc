#include <iostream>
#include "../eventloop.h"

using namespace std;

int main()
{
    EventLoop *eventloop = new EventLoop();
    eventloop->loop();
    
    return 0;
}