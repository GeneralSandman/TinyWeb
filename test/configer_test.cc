#include "../log.h"
#include "../api.h"
#include "../config.h"

#include <iostream>

using namespace std;

int main()
{
    {
        Configer conf("../TinyWeb.conf");
        conf.loadConfig();
        conf.test();
    }
    return 0;
}