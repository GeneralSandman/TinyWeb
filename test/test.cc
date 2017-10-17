#include <iostream>
#include <fstream>
#include <vector>

#include "../api.h"

using namespace std;

int main()
{

    std::ofstream m_nDebugLogFile;
    m_nDebugLogFile.open("/home/li/TinyWeb/log/TinyWeb.c",
                         std::ios::in | std::ios::app);
    if (!m_nDebugLogFile.is_open())
    {
        std::cout << "open filed\n";
    }
    return 0;
}

int _main()
{
    string line = "GET /index.htm2 HTTP/1.0";
    std::vector<std::string> res;
    splitString(line, " ", res);
    if (res.size() != 3)
        return false;
    std::cout << res[0] << std::endl;
    std::cout << res[1] << std::endl;
    std::cout << res[2] << std::endl;

    return 0;
}