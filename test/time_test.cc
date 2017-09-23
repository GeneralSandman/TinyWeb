#include "../time.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    Time now(Time::now());
    printf("%s\n", now.toString().c_str());
    std::cout << now.toFormattedString() << std::endl;
}
