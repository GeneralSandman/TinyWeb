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

#include <tiny_base/file.h>
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>

#include <fcntl.h>
#include <string.h>
#include <string>
#include <sys/stat.h>

int isRegularFile(const std::string& fname)
{
    struct stat info;
    int return_val;

    return_val = stat(fname.c_str(), &info);
    if (return_val < 0) {
        // std::cout << "[Debug] responser get file (" << fname << ") stat failed" << std::endl;
        if (errno == ENOENT) {
            // std::cout << "no such file or directory"
            // << std::endl;
        }
        return -1;
    }

    if (S_ISDIR(info.st_mode)) {
        return 1;
    }

    if (S_ISREG(info.st_mode)) {
        return 0;
    }

    return return_val;
}

std::string getType(const std::string& f)
{
    std::string res;
    bool findPoint = false;
    for (int i = f.size() - 1; i >= 0; i--) {
        if (f[i] == '.') {
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

int File::setFile(const std::string& fname)
{
    name = fname;
    offset = 0;
    valid = false;

    int return_val = stat(name.c_str(), &info);
    if (return_val < 0) {
        if (errno == ENOENT) {
        }
        return -1;
    }

    return_val = open(name.c_str(), O_RDONLY);
    if (return_val < 0) {
        if (errno == EACCES) {
            // std::cout << "owner hasn't read permission\n";
        }
        return -2;
    }

    type = getType(name);
    std::cout << "type:" << type << std::endl;
    valid = true;
    fd = return_val;

    LOG(Debug) << "response file:" << name << std::endl;
    return 0;
}

inline unsigned int File::getFileSize()
{
    if (valid) {
        return info.st_size;
    }
    return 0;
}

chain_t* File::appendData(chain_t* dest, const char* data, unsigned int len)
{
    if (nullptr == dest || nullptr == data || 0 == len) {
        return dest;
    }

    chain_t* chain;
    buffer_t* buffer;
    unsigned int buff_size;
    unsigned int predata_size;
    unsigned int empty_size;

    const char* pos = data;
    unsigned int left = len;
    unsigned int to_write = 0;

    // LOG(Debug) << "append size:" << left << std::endl;

    chain = dest;
    while (left && nullptr != chain) {
        buffer = chain->buffer;
        buff_size = buffer->end - buffer->begin;
        predata_size = buffer->used - buffer->begin;
        empty_size = buff_size - predata_size;

        if (!empty_size) {
            // This chain is full, change to next chain.
            buffer->islast = false;
            chain = chain->next;
            continue;
        }

        to_write = (left > empty_size) ? empty_size : left;
        memcpy((void*)buffer->used, (const void*)pos, to_write);
        // LOG(Debug) << "buffer:all-size(" << buff_size
        // << "),predata-size(" << predata_size
        // << "),postdata-size(" << predata_size + to_write << ")\n";

        buffer->used = buffer->used + to_write;
        buffer->islast = true;

        left -= to_write;
        pos += to_write;

        if (left) {
            // This chain is full, change to next chain.
            buffer->islast = false;
            chain = chain->next;
        }
    }

    return chain;
}

void File::getData(chain_t* chain)
{
    //Make sure enough space for data.

    if (!valid || chain == nullptr || chain->buffer == nullptr) {
        return;
    }

    unsigned int chain_size;
    chain_t* end_chain;
    chain_t* tmp_chain;
    buffer_t* tmp_buffer;
    char* read_buffer;
    unsigned int buffer_size;
    unsigned int read_len;

    chain_size = 0;
    tmp_chain = chain;
    buffer_size = chain->buffer->end - chain->buffer->begin;
    if (!buffer_size) {
        buffer_size = 4 * 1024;
    }
    read_buffer = (char*)malloc(buffer_size);

    while (tmp_chain) {
        tmp_buffer = tmp_chain->buffer;
        chain_size += tmp_buffer->end - tmp_buffer->begin;
        tmp_chain = tmp_chain->next;
    }
    if (info.st_size > chain_size) {
        LOG(Warn) << "chain size is not enough\n";
    }

    end_chain = chain;
    while ((read_len = pread(fd, (void*)read_buffer, buffer_size, offset))) {
        offset += read_len;
        end_chain = appendData(end_chain, read_buffer, read_len);
    }

    // Only for debug.
    unsigned int l = countChain(chain);
    LOG(Debug) << "chain size(" << l << "),status(";
    for (auto t = chain; t != nullptr; t = t->next) {
        if (t->buffer->islast) {
            std::cout << "-";
        } else {
            std::cout << "+";
        }
    }
    std::cout << ")\n";

    free((void*)read_buffer);
}

// using for Range && Content-Range.
void File::getDate(chain_t* chain, off_t begin, off_t end)
{
}
// int sendfile(int outFd, HttpFile* file)
// {
//     if (outFd == 0) {
//         LOG(Debug) << "[model file] sendfile " << file->name << std::endl;
//         return 1;
//     }

//     // TODO: Change this code to tiny_core_model.
//     ssize_t res = sendfile(outFd, file->fd, &(file->offset), file->info.st_size);
//     if (res < 0) {
//         //print_error("open html file error");//TODO
//         std::cout << "[Debug] responser sendfile (" << file->name << ") failed" << std::endl;
//         printf("%s\n", strerror(errno));
//     }
//     //std::cout << "[Debug] responser sendfile size =" << res << std::endl;

//     return res;
// }
