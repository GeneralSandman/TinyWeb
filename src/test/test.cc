#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
#include <boost/function.hpp>
// #include <functional>
#include <boost/bind.hpp>

typedef function<void(int)> handler;

typedef void (*s_handler)(int);
void set(handler h)
{
    s_handler res;
    // res = h;
    // (*res)(1);
}

void test(int i)
{
    std::cout << i << endl;
}

int main()
{
    set(boost::bind(&test, _1));

    s_handler res = test;
    res(1);
    return 0;
}
