#include <cstring>
#include <iostream>
#include <vector>

using namespace std;
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <vector>


int main() {

    int i = 1;
    std::cout << i++ << " " << i << " " << i++ << std::endl;
    std::cout << i << std::endl;

    std::cout << std::endl;

    i = 1;
    std::cout << i++ << std::endl;
    std::cout << i << std::endl;
    std::cout << i++ << std::endl;
}
