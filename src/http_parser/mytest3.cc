#include "my_http_parser.h"

#include <stdio.h>
#include <iostream>
#include <cstring>

using namespace std;

#define offsetof__(s, m) (size_t) & (((s *)0)->m)

int main()
{
        HttpHeaders *headers = new HttpHeaders;
        std::cout << sizeof(*headers) << std::endl;
        std::cout << sizeof(headers->host) << std::endl;
        std::cout << offsetof__(HttpHeaders, host) << std::endl;
        std::cout << offsetof__(HttpHeaders, connection) << std::endl;

        delete headers;
        return 0;
}
