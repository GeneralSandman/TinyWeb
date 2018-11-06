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

#include "http.h"
#include "http_model_file.h"
#include "http_responser.h"

#include <iostream>
#include <fstream>
#include <fcntl.h>

using namespace std;

void response(const std::string &fname)
{
    File file;
    int return_val = initFile(&file, fname);

    std::string res;
    if (return_val == -1)
    {
        //404
        specialResponseBody(HTTP_STATUS_NOT_FOUND, res);
    }
    else if (return_val == -2)
    {
        //5xx
    }
    else if (return_val == -3)
    {
        //403
        specialResponseBody(HTTP_STATUS_FORBIDDEN, res);
    }
    else if (return_val == -4)
    {
        //5xx
    }
    else if (return_val == 0)
    {
        sendfile(0, &file);
    }

    std::cout << res << std::endl;

    destoryFile(&file);
}

void convertHeadersToString(HttpResponseHeaders *headers,
                            std::string &result)
{
    for (auto t : headers->generals)
    {
    }
}

void constructResponseByRequest(const HttpRequest *request,
                                HttpResponse *resp)
{
    resp->type = HTTP_TYPE_RESPONSE;
    resp->http_version_major = request->http_version_major;
    resp->http_version_minor = request->http_version_minor;

    char *data = request->url->data;
    unsigned int file_offset = request->url->fields[HTTP_UF_PATH].offset;
    unsigned int file_len = request->url->fields[HTTP_UF_PATH].len;
    std::string filename(data + file_offset, file_len);
    std::cout << "file name :" << filename << std::endl;

    int return_val = initFile(&(resp->file), filename);
    std::cout << "initFile() return_val :" << return_val << std::endl;

    resp->status = HTTP_STATUS_OK;
    resp->special_response = (atoi(httpStatusCode(resp->status)) - 300 > 0);

    //TODO:construct response header
    std::string server = "TinyWeb/0.0.1 (Ubuntu)";
    std::string date = "Sun, 04 Nov 2018 07:09:51 GMT";
    std::string contentType = "text/html";
    std::string lastModified = "Fri, 20 Apr 2018 08:12:56 GMT";

    HttpHeader *header = new HttpHeader;


}

void convertRespToStr(const HttpResponse *resp, std::string &res)
{
    res = "HTTP/" + char(resp->http_version_major + '0') + '.' + char(resp->http_version_minor + '0');
    res += std::string(httpStatusCode(resp->status));
    res += " ";
    res += std::string(httpStatusStr(resp->status));
    res += "\r\n";
    res += "Content-Length: 10\r\n";
    res += "Connection: close\r\n";
    res += "Server: TinyWeb/0.0.1(ubuntu)\r\n";
    res += "\r\n";
}

void testResponse()
{
    HttpRequest req;
    HttpResponse resp;
    std::string res;

    constructResponseByRequest(&req, &resp);
    convertRespToStr(&resp, res);

    std::cout << res << std::endl;
}

int main()
{
    testResponse();

    return 0;
}
