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

// #include "../api.h"
// #include "../protocol.h"
#include "../responser.h"

using namespace std;

int main()
{
    std::vector<std::string> files;
    files.push_back("asdsfk.js");
    files.push_back("asdsfk1.2.12.js");
    files.push_back("asdsfk.min.css");
    files.push_back("asd");

    for (auto f : files)
        std::cout << getType(f) << endl;

    return 0;
}