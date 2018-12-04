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

#include <tiny_base/buffer.h>
#include <tiny_base/log.h>

#include <sys/uio.h>
#include <algorithm>

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

void Buffer::hasReadBytes(size_t len)
{
    if (len < m_fReadableBytes())
        m_nReadIndex += len;
    else
        m_fClearAll();
}

void Buffer::m_fEnsureWritableBytes(size_t len)
{
    if (m_fWriteableBytes() < len)
        m_fMakeSpace(len);
}

inline void Buffer::m_fMakeSpace(size_t len)
{
    if (m_fPrependableBytes() + m_fWriteableBytes() < len + K_Buffer_Prepend)
    {
        m_nDatas.resize(m_nWriteIndex + len);
    }
    else
    {
        size_t read = m_fReadableBytes();
        std::copy(m_fReadableBegin(),
                m_fWriteableBegin(),
                m_fBegin() + K_Buffer_Prepend);
        m_nReadIndex = K_Buffer_Prepend;
        m_nWriteIndex = m_nReadIndex + read;
    }
}

void Buffer::m_fClearAll()
{
    m_nReadIndex = K_Buffer_Prepend;
    m_nWriteIndex = K_Buffer_Prepend;
}

Buffer::Buffer()
    : m_nDatas(K_Buffer_Prepend + K_Buffer_Size),
    m_nReadIndex(K_Buffer_Prepend),
    m_nWriteIndex(K_Buffer_Prepend)
{
    LOG(Debug) << "class Buffer constructor\n";
}

void Buffer::swap(Buffer &r)
{
    m_nDatas.swap(r.m_nDatas);
    std::swap(m_nReadIndex, r.m_nReadIndex);
    std::swap(m_nWriteIndex, r.m_nWriteIndex);
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
        //error
    }
    else if (size_t(n) <= writable)
    {
        m_nWriteIndex += n;
    }
    else
    {
        m_nWriteIndex = m_nDatas.size();
        append(extrabuf, n - writable);
    }
    return n;
}

std::string Buffer::get(size_t len)
{
    if (len > m_fReadableBytes())
        len = m_fReadableBytes();
    std::string result(m_fReadableBegin(), len);
    hasReadBytes(len);
    return result;
}

std::string Buffer::getAll()
{
    return get(m_fReadableBytes());
}

void Buffer::shrink()
{
    Buffer tmp;
    tmp.append(getAll());
    swap(tmp);
}

void Buffer::append(const std::string &str)
{
    append(str.c_str(), str.size());
}

void Buffer::append(const char *str, int len)
{
    std::copy(str, str + len, m_fWriteableBegin());
    m_nWriteIndex += len;
}

void Buffer::prepend(const void * /*restrict*/ data, size_t len)
{
    m_nReadIndex -= len;
    const char *d = static_cast<const char *>(data);
    std::copy(d, d + len, m_fReadableBegin());
}

const char *Buffer::findCRLF() const
{
    const char *crlf = std::search(m_fReadableBegin(),
            m_fWriteableBegin(),
            kCRLF, kCRLF + 2);
    return crlf == m_fWriteableBegin() ? NULL : crlf;
}

const char *Buffer::findCRLF(const char *start) const
{
    const char *crlf = std::search(start,
            m_fWriteableBegin(),
            kCRLF, kCRLF + 2);
    return crlf == m_fWriteableBegin() ? NULL : crlf;
}

bool Buffer::getALine(std::string &line)
{
    const char *line_end = findCRLF();
    if (line_end == nullptr)
        return false; //This Buffer hasn't complete line

    int len = line_end - m_fReadableBegin();

    std::string tmp(m_fReadableBegin(), len);
    line.swap(tmp);
    hasReadBytes(len + 2); //leave the crlf
    return true;
}

Buffer::~Buffer()
{
    LOG(Debug) << "class Buffer destructor\n";
}
