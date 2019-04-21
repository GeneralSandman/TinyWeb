#include <cstring>
#include <iostream>
#include <vector>

using namespace std;
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <map>
#include <vector>

static int
checkForZeroes(char* buf, int n)
{
    int i, j;
    unsigned long* lbuf = (unsigned long*)buf;
    assert(n % sizeof(unsigned long) == 0);

    for (i = 0; i * sizeof(unsigned long) < n; i++) {
        if (lbuf[i] != 0L)
            return i * sizeof(unsigned long);
    }
    for (j = 0; i * sizeof(unsigned long) + j < n; j++) {
        if (buf[i * sizeof(unsigned long) + j] != 0)
            break;
    }

    return i * sizeof(unsigned long) + j;
}

int main1()
{
    char* buf = "";
    int nread = strlen(buf);
    int nzeroes = checkForZeroes(buf, nread & -8);

    std::cout << "nzeors:" << nzeroes << std::endl;

    return 0;
}

typedef struct header {
    int a;
    int b;

} header;

#include <boost/bind.hpp>
#include <boost/function.hpp>

typedef boost::function<void(const header*)> Callback1;
typedef boost::function<void()> Callback2;

void print_header(header* h)
{
    if (nullptr == h) {
        return;
    }
    std::cout << "header a:" << h->a << ",b:" << h->b << std::endl;
};

int main()
{
    header* h1 = new header;
    h1->a = 1;
    h1->b = 1;

    header* h2 = new header;
    h2->a = 2;
    h2->b = 2;

    Callback2 func = boost::bind(print_header, h1);
    func();

    func = boost::bind(print_header, h2);
    func();

    auto print_header2 = [h1]()
    {
        if (nullptr == h1) {
            return;
        }
        std::cout << "header a:" << h1->a << ",b:" << h1->b << std::endl;
    };

    func = print_header2;
    func();

    delete h1;
    delete h2;
}
