#include "../processpool.h"

#include <iostream>

using namespace std;

int main()
{
    {

        ProcessPool pool;
        pool.start(1);
        // sleep(5);
        // pool.killAll();
        // sleep(5);

        pause();
    }

    return 0;
}