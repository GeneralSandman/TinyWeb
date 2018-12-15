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

#ifndef HTTP_MODEL_GZIP_H
#define HTTP_MODEL_GZIP_H

#include <tiny_base/memorypool.h>

#include <string>
#include <zlib.h>

typedef struct gzip_config_t {
    bool enable;
    bool have_buffer;
    unsigned int level;
    unsigned int buffers_4k;
    unsigned int min_len;
    unsigned int wbits;
    unsigned int memlevel;
} gzip_config_t;

typedef struct gzip_context_t {
    z_stream stream;
    int flush;

    chain_t* in;
    chain_t* out;

    chain_t* curr_in;
    chain_t* curr_out;

    chain_t* last_in;
    chain_t* last_out;

    unsigned int level;
} gzip_context_t;

enum gzip_status {
    gzip_ok = 0,
    gzip_error,
    gzip_continue,
    gzip_done
};

void get_zip_config(gzip_config_t* conf);

void gzip_context_init(MemoryPool* pool, gzip_config_t* conf, gzip_context_t* context);
gzip_status gzip_deflate_init(gzip_config_t* conf,
    gzip_context_t* context);
gzip_status gzip_deflate(gzip_context_t* context, buffer_t* buffer);
gzip_status gzip_deflate_end(gzip_context_t* context);

gzip_status gzip_add_data(gzip_context_t* context,
    const char* data,
    unsigned int len);
gzip_status gzip_body(MemoryPool* pool,
    gzip_context_t* context,
    const char* data,
    unsigned int len);

gzip_status gzip_out(gzip_context_t* context,
    const std::string& file);

#endif
