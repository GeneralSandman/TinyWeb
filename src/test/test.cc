#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <iostream>


int main()
{
    vector<int> v(8);
    std::cout << v.size() << std::endl;
    std::cout << v.capacity() << std::endl;

    vector<int> v2;
    v2.reserve(8);
    std::cout << v2.size() << std::endl;
    std::cout << v2.capacity() << std::endl;

    int size;
    std::cout << r << std::endl;

    return 0;
}
