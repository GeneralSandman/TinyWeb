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
    Stat(filename, &sbuf);
    if (!S_ISREG(sbuf.st_mode))
        return false;

    res.size = sbuf.st_size;
    convertToStr(&sbuf.st_mtim.tv_sec, res.lastModified, 33, true);

    return true;
}

void HttpResponser::m_fCreateResponse(const struct HttpRequest &request,
                                      struct HttpResponse &response)
{
    std::string docs = "../www";
    std::string file = docs + request.line.url;
    // std::string file = docs + "/404/404.html";
    
    struct HtmlFileStatus fileStatus;
    if (setStatus(file, fileStatus))
    {
        //
        response.line.version = "HTTP/1.1";
        response.line.statusCode = "200";
        response.line.status = "OK";

        //
        response.header.date = "test local time";
        response.header.server = "ubuntu";
        response.header.lastModified = fileStatus.lastModified;
        response.header.contentLength = std::to_string(fileStatus.size);
        response.header.contentType = "text/html";

        //
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
    m_pProtocol->m_pConnection->shutdownWrite(); //FIXME:

    printHttpRequest(request);
    printHttpResponse(resp);
}

HttpResponser::~HttpResponser()
{
    LOG(Debug) << "class HttpResponse destructor\n";
}
