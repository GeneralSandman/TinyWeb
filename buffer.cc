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

inline size_t Buffer::m_fPrependableBytes() const
{
    return m_nReadIndex;
}

inline size_t Buffer::m_fReadableBytes() const
{
    return m_nWriteIndex - m_nReadIndex;
}

inline size_t Buffer::m_fWriteableBytes() const
{
    return m_nDatas.size() - m_nWriteIndex;
}

inline char *Buffer::m_fBegin()
{
    return &(*m_nDatas.begin());
}

inline const char *Buffer::m_fBegin() const
{
    return &(*m_nDatas.begin());
}

inline char *Buffer::m_fReadableBegin()
{
    return m_fBegin() + m_nReadIndex;
}

inline const char *Buffer::m_fReadableBegin() const
{
    return m_fBegin() + m_nReadIndex;
}

inline char *Buffer::m_fWriteableBegin()
{
    return m_fBegin() + m_nWriteIndex;
}

inline const char *Buffer::m_fWriteableBegin() const
{
    return m_fBegin() + m_nWriteIndex;
}

inline void Buffer::m_fMakeSpace(size_t len)
{
    if (m_fPrependableBytes() + m_fWriteableBytes() < len + 8)
    {
        m_nDatas.resize(m_nWriteIndex + len);
    }
    else
    {
        size_t read = m_fReadableBytes();
        std::copy(m_fReadableBegin(),
                  m_fWriteableBegin(),
                  m_fBegin() + 8);
        m_nReadIndex = 8;
        m_nWriteIndex = m_nReadIndex + read;
    }
}

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
    const size_t writable = m_fWriteableBytes();
    result[0].iov_base = m_fWriteableBegin();
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