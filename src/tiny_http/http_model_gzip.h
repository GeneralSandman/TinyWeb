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
    gzip_config_t* conf;

    z_stream stream;
    int flush;

    chain_t* in;
    chain_t* out;

    chain_t* curr_in;
    chain_t* curr_out;

    chain_t* last_in;

    unsigned int level;
} gzip_context_t;

enum gzip_status {
    gzip_ok = 0,
    gzip_error,
    gzip_continue,
    gzip_done,
};

void gzip_config_init(gzip_config_t* conf);
void get_gzip_config(gzip_config_t* conf);

void gzip_context_init(gzip_context_t* context,
    gzip_config_t* conf,
    chain_t* output);

class HttpModelGzip {
private:
    MemoryPool* m_pPool;

    gzip_config_t* m_pConfig;
    gzip_context_t* m_pContext;

    chain_t* m_pOutputChain;

    bool m_nEndData;


    gzip_status gzip_deflate_init();
    gzip_status gzip_deflate();
    gzip_status gzip_deflate_end();


public:
    HttpModelGzip(MemoryPool* pool)
    {
        // LOG(Debug) << "class HttpModelGzip constructor\n";
        m_pPool = pool;
        m_pConfig = new gzip_config_t;
        m_pContext = new gzip_context_t;
        m_nEndData = false;
    }

    gzip_status init();
    chain_t* getOutputChain()
    {
        return m_pOutputChain;
    }
    gzip_status compress(chain_t* input, chain_t* & output, bool endData);
    gzip_status uncompress(chain_t* input, chain_t* output);

    gzip_status gzip_out(gzip_context_t* context,
        const std::string& file);
    
    ~HttpModelGzip()
    {
        // LOG(Debug) << "class HttpModelGzip destructor\n";

        delete m_pConfig;
        delete m_pContext;
    }
};

#endif
