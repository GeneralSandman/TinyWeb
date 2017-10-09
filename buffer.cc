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

#include "buffer.h"
#include "log.h"

#include <sys/uio.h>

Buffer::Buffer()
{
    LOG(Debug) << "class Buffer constructor\n";
}

size_t Buffer::put(int fd)
{
    //read date from fd and store to vector<char>
    //invoke readv()

    char extrabuf[65536];
    struct iovec result[2];
    const size_t writable = writableBytes();
    result[0].iov_base = begin() + writerIndex_;
    result[0].iov_len = writable;
    result[1].iov_base = extrabuf;
    result[1].iov_len = sizeof extrabuf;
    ssize_t n = readv(fd, result, 2);
    if (n < 0)
    {
        *savedErrno = errno;
    }
    else if (implicit_cast<size_t>(n) <= writable)
    {
        writerIndex_ += n;
    }
    else
    {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    // if (n == writable + sizeof extrabuf)
    // {
    //   goto line_30;
    // }
    return n;
}

std::string Buffer::get()
{
    //fetch readable date
}

Buffer::~Buffer()
{
    LOG(Debug) << "class Buffer destructor\n";
}