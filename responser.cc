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

HttpResponser::HttpResponser(WebProtocol *prot)
    : m_pProtocol(prot)
{
    LOG(Debug) << "class HttpResponse constructor\n";
}

void HttpResponser::response(const struct HttpRequest &request)
{
    std::string h = "HTTP/1.0 200 OK\r\n";
    std::string c = "Content-Type: text/html\r\n\r\n";

    const std::string f = "../www/404/404.html";
    char *filename = new char[f.size() + 1];
    char *head = filename;
    for (auto t : f)
        *(head++) = t;
    *(head++) = '\0'; // very very important

    struct stat sbuf;
    Stat(filename, &sbuf);
    size_t filesize = sbuf.st_size;
    int filefd = Open(filename, O_RDONLY, 0);
    char *srcp = (char *)Mmap(0, filesize, PROT_READ, MAP_PRIVATE, filefd, 0); //line:netp:servestatic:mmap
    Close(filefd);

    std::string write; //line:netp:servestatic:close
    write.resize(filesize);
    std::copy(srcp, srcp + filesize, write.begin());
    write = h + c + write;

    m_pProtocol->sendMessage(write);

    Reader reader("../www/404/404.html");

    std::string tmp;
    while (reader.haveMore())
    {
        tmp = reader.readLine();
        std::cout << tmp;
        m_pProtocol->sendMessage(tmp);
    }

    // std::cout << "--" << std::endl;

    Munmap(srcp, filesize);
    delete[] filename;

    m_pProtocol->m_pConnection->shutdownWrite(); //FIXME:
}

HttpResponser::~HttpResponser()
{
    LOG(Debug) << "class HttpResponse destructor\n";
}
