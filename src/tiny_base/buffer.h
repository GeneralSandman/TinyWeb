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

/*---class Buffer---
 *It is very improtant to provied a network buffer to
 *socket,
 ****************************************
 *
 *put(fd):read date from fd and store into vector<char>
 *get():fetch readable date
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <unistd.h>
#include <vector>

#define K_Buffer_Prepend 8
#define K_Buffer_Size 1024
const char kCRLF[] = "\r\n";

class Buffer {
private:
    std::vector<char> m_nDatas;
    int m_nReadIndex;
    int m_nWriteIndex;

    size_t m_fPrependableBytes() const;
    size_t m_fReadableBytes() const;
    size_t m_fWriteableBytes() const;
    char* m_fBegin();
    const char* m_fBegin() const;
    char* m_fReadableBegin();
    const char* m_fReadableBegin() const;
    char* m_fWriteableBegin();
    const char* m_fWriteableBegin() const;
    void hasReadBytes(size_t len);
    void m_fEnsureWritableBytes(size_t len);
    void m_fMakeSpace(size_t len);
    void m_fClearAll();

public:
    Buffer();
    void swap(Buffer& r);
    size_t put(int fd);
    size_t readableBytes() { return m_fReadableBytes(); }
    std::string get(size_t len);
    std::string getAll();
    void shrink();
    void append(const std::string& str);
    void append(const char* str, int len);
    void prepend(const void* /*restrict*/ data, size_t len);
    const char* findCRLF() const;
    const char* findCRLF(const char* start) const;
    bool getALine(std::string& res);
    ~Buffer();
};

#endif // !BUFFER_H
