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

#include <tiny_base/configer.h>
#include <tiny_base/file.h>
#include <tiny_http/http_model_file.h>

#include <iostream>

using namespace std;

void initConfige()
{

}

void testFile()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";
    std::cout << std::endl;

    std::string servername = "dissigil.cn";
    std::string wwwpath;

    ServerConfig server = configer.getServerConfig(servername);
    wwwpath = server.www;

    std::cout << "indexpage:";
    for (auto t : server.indexpage) {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    vector<std::string> files;
    files.push_back("/");
    files.push_back("/index.html");
    files.push_back("/index.htm");
    files.push_back("/index.php");
    files.push_back("/test/test.php");
    files.push_back("/blog/");
    files.push_back("/tests/");
    files.push_back("/ajsldfaj.html");

    int file_type;
    int return_val;

    for (auto f : files) {

        f = wwwpath + f;
        std::cout << f << std::endl;

        HttpFile file;
        file_type = isRegularFile(f);
        if (0 == file_type) {
            std::cout << "file:\n";
            return_val = file.setFile(f);
        } else if (1 == file_type) {
            std::cout << "path:\n";
            return_val = file.setPathWithDefault(f, server.indexpage);
            // TODO:if hasn't default index page in , go to sepical file
        } else if (-1 == file_type) {
            std::cout << "sepical file:\n";
        }

        if (!return_val) {
            std::cout << "++file exit" << std::endl;
        } else {
            std::cout << "--file not exit" << std::endl;
        }

        std::cout << std::endl;
    }
}

void testFile2()
{
}

int main()
{
    testFile();
    testFile2();

    return 0;
}
