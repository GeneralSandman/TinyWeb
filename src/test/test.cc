#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <iostream>


int main_()
{
    vector<int> v(8);
    std::cout << v.size() << std::endl;
    std::cout << v.capacity() << std::endl;

    vector<int> v2;
    v2.reserve(8);
    std::cout << v2.size() << std::endl;
    std::cout << v2.capacity() << std::endl;

    int size;
    // std::cout << r << std::endl;

    return 0;
}

int main()
{
    char str[] = "0000000000000000\r\n";

    unsigned int size = 65535;
    sprintf(str, "%16x\r\n", size);

    size_t len = strlen(str);
    printf("size:%u,len:%u,data:%.*s(end)", sizeof(str), len, len, str);

}
