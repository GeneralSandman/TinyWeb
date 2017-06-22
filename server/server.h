#ifndef SERVER_H
#define SERVER_H
#include "basic_server.h"

namespace server
{
class HtmlServer : public Server
{
};
class HtmlFactory : public Factory
{
};
class HtmlProtocal : public Protocal
{
};
}

#endif