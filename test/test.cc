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

#include "../api.h"
#include "../protocol.h"

using namespace std;

class B
{
  public:
    virtual ~B(){};
};

class D : public B
{
};

// int len;
// const char *str = typeid(*p).name();

// std::cout << typeid(*p).name() << endl;
// std::cout << str << endl;

// while ((*str) != '\0')
// {
//     std::cout << *str;
//     str++;
// }

void m_fGetLines(const std::string &s, std::vector<std::string> &res, std::string &restOpenLine)
{
    const char crlf[] = "\r\n";

    auto search_begin = s.begin();
    auto line_end = s.end();

    while ((line_end = std::search(search_begin, s.end(), crlf, crlf + 2)) != s.end())
    {
        std::string tmp(search_begin, line_end);
        res.push_back(tmp);
        search_begin = line_end + 2;
    }

    //stroe restOpenLine
    if (s.end() > search_begin)
    {
        restOpenLine.resize(s.end() - search_begin);
        std::copy(search_begin, s.end(), restOpenLine.begin());
    }
}

struct HtmlFileStatus
{
    std::string name;
    long long size;
    char lastModified[33];
};

void convertToStr(const time_t *src, char *buf, int size, bool isLocal)
{
    struct tm *tmp = nullptr;
    if (isLocal)
        tmp = localtime(src);
    else
        tmp = gmtime(src);
    strftime(buf, size, "%a, %d %B %Y %H:%M:%S %Z", tmp);
}

bool setStatus(const std::string &file, struct HtmlFileStatus &res)
{
    res.name = file;

    ///////
    char *filename = new char[file.size() + 1];
    char *head = filename;
    for (auto t : file)
        *(head++) = t;
    *(head++) = '\0'; // very very important
    ////////
    struct stat sbuf;
    Stat(filename, &sbuf);
    if (!S_ISREG(sbuf.st_mode))
        return false;

    res.size = sbuf.st_size;
    convertToStr(&sbuf.st_mtim.tv_sec, res.lastModified, 33, true);

    return true;
}

int main()
{

    std::string f;
    cin >> f;

    struct HtmlFileStatus status;
    if (setStatus(f, status))
    {
        cout << status.name << endl;
        cout << status.size << endl;
        cout << status.lastModified << endl;
    }

    return 0;
}

int __11main()
{
    std::string h = "HTTP/1.0 200 OK\r\n";
    std::string c = "Content-Type: text/html\r\n\r\n";
    std::string html = "hello world";

    const std::string s = h + c + html;

    std::vector<std::string> res;
    std::string rest;

    m_fGetLines(s, res, rest);

    for (auto t : res)
        cout << "-" << t << "-\n";
    cout << rest << endl;

    return 0;
}

int ____main()
{
    D d;
    const B *p2 = &d;
    cout << typeid(p2).name() << endl;
    cout << typeid(*p2).name() << endl;

    return 0;
}

int __main()
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