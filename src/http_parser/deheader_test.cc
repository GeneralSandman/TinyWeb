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

#include "http_responser.h"
#include "http_parser.h"
#include "http_model_file.h"

#include <iostream>
#include <string>

using namespace std;

void testDeheaders()
{

    HttpResponseLine_testing line;
    line.http_version_major = 1;
    line.http_version_minor = 1;
    line.status = HTTP_STATUS_FORBIDDEN;
    std::string line_str;
    responseLineToStr(&line, line_str);


    HttpHeaders_testing headers;
    headers.file_type = "text/html";
    headers.valid_content_length = 1;
    headers.connection_keep_alive = 1;
    headers.chunked = 1;
    headers.server = 1;
    std::string res;
    responseHeadersToStr(&headers, res);

    std::cout << line_str << res << std::endl;

    for (auto i = headers.generals.begin(); i != headers.generals.end(); i++)
    {
        delete (*i);
        *i = nullptr;
    }
}


void testResponser()
{

}


void testGetMimeType(void)
{

    std::vector<std::string> fileNames;
    fileNames.push_back("dd.html");
    fileNames.push_back("dd/dd");
    fileNames.push_back("dd/1.jpeg");
    fileNames.push_back("dd/1.js");
    fileNames.push_back("dddd/ds.css");

    for (auto f: fileNames)
    {
        std::cout << f << " " << getType(f) << " " << getMimeType(getType(f)) << std::endl;
    }
}

int main()
{
    //testDeheaders();
    testGetMimeType();
    return 0;
}
