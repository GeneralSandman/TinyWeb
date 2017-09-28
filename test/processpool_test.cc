#include "../processpool.h"

#include <iostream>

using namespace std;

int main()
{
    {

        ProcessPool pool;
        pool.start(3);
    }

    return 0;
}