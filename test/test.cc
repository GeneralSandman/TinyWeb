#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

using namespace std;

int main()
{
    std::string type;
    std::hash<std::string> h;

    while (1)
    {
        cin >> type;
        cout << h(type);
    }

    return 0;
}