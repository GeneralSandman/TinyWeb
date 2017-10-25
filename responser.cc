/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include "responser.h"
#include "protocol.h"
#include "http.h"
#include "api.h"
#include "log.h"
#include "reader.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <algorithm>

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
    if (stat(filename, &sbuf) < 0)
    {
        std::cout << "can't find this file\n";
        return false;
    }
    if (!S_ISREG(sbuf.st_mode))
        return false;

    res.size = sbuf.st_size;
    res.contentType = getType(file);
    convertToStr(&sbuf.st_mtim.tv_sec, res.lastModified, 33, true);

    return true;
}

std::string getType(const std::string &f)
{
    std::string res;
    int flag = 0;
    bool findPoint = false;
    for (int i = f.size() - 1; i >= 0; i--)
    {
        if (f[i] == '.')
        {
            findPoint = true;
            break;
        }
        if (f[i] == '/')
            break; //encounter a slash
        res = f[i] + res;
    }
    if (!findPoint)
        res = "";
    return res;
}

std::string getMIMEType(const std::string &type)
{
    std::string res;

    if ("html" == type)
    {
        res = "text/html";
    }
    else if ("htm" == type)
    {
        res = "text/htm";
    }
    else if ("css" == type)
    {
        res = "text/css";
    }
    else if ("js" == type)
    {
        res = "application/javascript";
    }
    else if ("bmp" == type)
    {
        res = "image/bmp";
    }
    else if ("gif" == type)
    {
        res = "image/gif";
    }
    else if ("jpeg" == type)
    {
        res = "image/jpeg";
    }
    else if ("png" == type)
    {
        res = "image/png";
    }

    return res;
}

bool HttpResponser::m_fCreateResponse(const struct HttpRequest &request,
                                      struct HttpResponse &response)
{
    std::string docs = "/home/li/TinyWeb/www";

    std::vector<std::string> vec;
    splitString(request.line.url, "?", vec);

    std::string file;
    if (vec.size() == 0)
        file = docs + request.line.url;
    else
        file = docs + vec[0];

    bool findPoint = false;

    for (int i = file.size(); i >= 0; i--)
    {
        if (file[i] == '.')
        {
            findPoint = true;
            break;
        }
        else if (file[i] == '/')
        {
            break;
        }
    }

    if (!findPoint)
        file += "/index.html";

    // std::cout << file << std::endl;
    LOG(Info) << file << std::endl;

    struct HtmlFileStatus fileStatus;
    if (setStatus(file, fileStatus))
    {
        //
        response.line.version = "HTTP/1.1";
        response.line.statusCode = "200";
        response.line.status = "OK";

        //
        response.header.date = fileStatus.lastModified;
        response.header.server = "ubuntu";
        response.header.lastModified = fileStatus.lastModified;
        response.header.contentLength = std::to_string(fileStatus.size);
        response.header.connection = "Keep-Alive";
        response.header.contentType = getMIMEType(fileStatus.contentType);

        //
        int filefd = Open(file.c_str(), O_RDONLY, 0);
        char *srcp = (char *)Mmap(0, fileStatus.size, PROT_READ, MAP_PRIVATE, filefd, 0); //line:netp:servestatic:mmap
        Close(filefd);
        response.body.text.resize(fileStatus.size);
        std::copy(srcp, srcp + fileStatus.size, response.body.text.begin());
        Munmap(srcp, fileStatus.size);
    }

    else
    { //haven't this file,return 404 file
        //
        file = docs + "/404.html";

        setStatus(file, fileStatus);
        response.line.version = "HTTP/1.1";
        response.line.statusCode = "404";
        response.line.status = "OK";

        //
        response.header.date = fileStatus.lastModified;
        response.header.server = "ubuntu";
        response.header.lastModified = fileStatus.lastModified;
        response.header.contentLength = std::to_string(fileStatus.size);

        response.header.contentType = "text/html";
        response.header.connection = "close";

        int filefd = Open(file.c_str(), O_RDONLY, 0);
        char *srcp = (char *)Mmap(0, fileStatus.size, PROT_READ, MAP_PRIVATE, filefd, 0); //line:netp:servestatic:mmap
        Close(filefd);
        response.body.text.resize(fileStatus.size);
        std::copy(srcp, srcp + fileStatus.size, response.body.text.begin());
        Munmap(srcp, fileStatus.size);
    }
}

void HttpResponser::m_fSendResponse(const struct HttpResponse &response)
{
    std::string to_write;
    convertHttpResponseToString(response, to_write);
    m_pProtocol->sendMessage(to_write);
    LOG(Info) << "response :" << std::endl;
}

HttpResponser::HttpResponser(WebProtocol *prot)
    : m_pProtocol(prot)
{
    LOG(Debug) << "class HttpResponse constructor\n";
}

void HttpResponser::response(const struct HttpRequest &request)
{
    struct HttpResponse resp;
    m_fCreateResponse(request, resp);
    m_fSendResponse(resp);
    // m_pProtocol->m_pConnection->shutdownWrite(); //FIXME:
    // g_loop->runAfter(60,
    //  boost::bind(m_pProtocol->m_pConnection->shutdownWrite()));
    //FIXME:upgrade eventloop class as signal class paradia
    //we will close this connection in 100 seconds

    // printHttpRequest(request);
    // printHttpResponse(resp);
}

HttpResponser::~HttpResponser()
{
    LOG(Debug) << "class HttpResponse destructor\n";
}
