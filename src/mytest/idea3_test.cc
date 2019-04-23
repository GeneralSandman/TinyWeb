/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <iostream>
#include <regex>

using namespace std;

bool regex__(const std::string& str, const std::string& pattern)
{
    bool res;
    try {
        std::regex reg(pattern);
        res = std::regex_match(str, reg);
    } catch (const std::exception& ex) {
        // std::cout << "regex_match error\n";
        // std::cout << ex.what() << std::endl;
        return false;
    }

    // std::cout << "[" << str << "  " << pattern << "]:";
    // if (res) {
    //     std::cout << "match success" << std::endl;
    // } else {
    //     std::cout << "match failed" << std::endl;
    // }
    return res;
}

typedef struct  
{
    const char* str;
    const char* pattern;

    bool match;
} regex_test_t;

regex_test_t regexs[] = {
    {
        .str = "sdsd.php",
        .pattern = ".*\\.php$",
        .match = true,
    },

    {
        .str = "ssssphp",
        .pattern = ".*\\.php$",
        .match = false,
    },

    {
        .str = "sdsf.php.",
        .pattern = ".*\\.php$",
        .match = false,
    },

    {
        .str = "*..php",
        .pattern = ".*\\.php$",
        .match = true,
    },
    {
        .str = "sds.phpss",
        .pattern = ".*\\.php$",
        .match = true,
    },
    {
        .str = "sdsdfphp",
        .pattern = ".*\\.php$",
        .match = true,
    },

};

void test()
{
    int alltest = 0;
    int passtest = 0;

    unsigned int len = sizeof(regexs) / sizeof(regexs[0]);

    for (int i = 0; i < len; i++) {
        bool res = regex__(regexs[i].str, regexs[i].pattern);
    }

}

void test1()
{
    std::vector<std::string> patterns;
    patterns.push_back(".*\.php$");
    patterns.push_back(".*\.html$");
    patterns.push_back(".*\.js$");
    patterns.push_back(".*\.css$");
    patterns.push_back(".*\.png$");
    patterns.push_back(".*\.(jpg|jpeg|gif)$");
    patterns.push_back(".*\.(jpg|jpeg|gif)$");
    patterns.push_back(".*\.(jpg|jpeg|gif)$");

    std::vector<std::string> strs;
    strs.push_back("/dss.php");
    strs.push_back("/dss.html");
    strs.push_back("/dss.js");
    strs.push_back("/dss.css");
    strs.push_back("/dss.png");
    strs.push_back("/dss.gif");
    strs.push_back("/dss.jpg");
    strs.push_back("/dss.jpeg");

    bool res;
    for (unsigned int i = 0; i < strs.size(); i++) {
        std::cout << i << ")\n";

        for (auto p : patterns) {
            res = regex__(strs[i], p);
            if (res) {
                break;
            }
        }
    }

}

void test2()
{
    std::vector<std::string> patterns;
    patterns.push_back(".*\\.php$");


    std::vector<std::string> strs;
    strs.push_back("/dss.php");
    strs.push_back(".php");
    strs.push_back("/dss.phpss");
    strs.push_back("/dssphp");


    bool res;
    for (unsigned int i = 0; i < strs.size(); i++) {
        std::cout << i << ")\n";

        for (auto p : patterns) {
            res = regex__(strs[i], p);
            if (res) {
                break;
            }
        }
    }

}

int _main()
{
    std::string a;
    std::string b;
    cin >> a;
    cin >> b;

    regex__(a, b);
    return 0;
}

int main()
{
    test();
    // test2();
}
