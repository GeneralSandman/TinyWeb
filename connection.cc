#include "connection.h"

Connection::Connection(EventLoop *loop, int connectfd)
    : m_nChannel(loop, connectfd)
{
}

Connection::~Connection()
{
}
