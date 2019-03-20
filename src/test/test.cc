#include <cstring>
#include <iostream>
#include <vector>

using namespace std;
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <vector>

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

int main__()
{
    char str[] = "0000000000000000\r\n";

    unsigned int size = 65535;
    sprintf(str, "%16x\r\n", size);

    size_t len = strlen(str);
    printf("size:%u,len:%u,data:%.*s(end)", sizeof(str), len, len, str);
}

typedef struct block {
    int a;
    block* next;

} block;

int main1()
{

    int len = 10;

    block* header = nullptr;
    block* tmp = nullptr;

    while (len--) {
        header = new block;
        header->a = len;
        header->next = tmp;

        tmp = header;
    }

    tmp = header;
    while (tmp) {
        std::cout << tmp->a << " ";
        tmp = tmp->next;
    }
    std::cout << std::endl;

    int target = 9;
    block* new_header = new block;
    new_header->next = header;

    block* pre = new_header;
    tmp = header;
    while (tmp) {
        if (tmp->a == target) {
            pre->next = tmp->next;
            delete tmp;
            break;
        }

        tmp = tmp->next;
        pre = pre->next;
    }

    header = new_header->next;
    tmp = header;
    while (tmp) {
        std::cout << tmp->a << " ";
        tmp = tmp->next;
    }
    std::cout << std::endl;

    return 0;
}

#include <math.h>




int main()
{
    vector<ssize_t> nums;

    nums.push_back(128);
    nums.push_back(129);
    nums.push_back(255);
    nums.push_back(256);
    nums.push_back(257);
    nums.push_back(511);
    nums.push_back(512);
    nums.push_back(513);
    nums.push_back(1023);
    nums.push_back(1024);
    nums.push_back(1025);
    nums.push_back(2047);
    nums.push_back(2048);
    nums.push_back(2049);
    nums.push_back(4095);
    nums.push_back(4096);
    nums.push_back(4097);

    for (auto t:nums) {
        std::cout << t << "-" << logarithm2(t) << "-" << ROUND_UP2N(t) << "-" << FREELIST_INDEX(t) << std::endl;
    }

    return 0;
}
