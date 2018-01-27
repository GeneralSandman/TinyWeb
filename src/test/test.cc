#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define f(s) sizeof(s)

int main()
{
    int *a;
    std::cout << sizeof(a) << std::endl;

    char *str = "11";
    std::cout << sizeof(str) << std::endl
              << strlen(str) << std::endl
              << f(str) << std::endl;

    char s[] = "11";
    std::cout << sizeof(s) << std::endl
              << strlen(s) << std::endl
              << f(s) << std::endl;

    str = "111";
    s = "22";

    return 0;
}