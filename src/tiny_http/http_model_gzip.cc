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

#include <tiny_base/configer.h>
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http_model_gzip.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>

void gzip_config_init(gzip_config_t* gzip_conf)
{
    if (nullptr == gzip_conf) {
        return;
    }

    gzip_conf->enable = 0;
    gzip_conf->level = 0;
    gzip_conf->buffers_4k = 0;
    gzip_conf->min_len = 0;
    gzip_conf->wbits = 0;
    gzip_conf->memlevel = 0;
}

void get_gzip_config(gzip_config_t* gzip_conf)
{
    if (nullptr == gzip_conf) {
        return;
    }

    Configer& configer = Configer::getConfigerInstance();
    BasicConfig basicConf = configer.getBasicConfig();

    gzip_conf->enable = basicConf.gzip;
    gzip_conf->level = basicConf.gzip_level;
    gzip_conf->buffers_4k = basicConf.gzip_buffers_4k;
    gzip_conf->min_len = basicConf.gzip_min_len;
    gzip_conf->wbits = 1;
    gzip_conf->memlevel = 0;
}

void gzip_context_init(gzip_context_t* context,
    gzip_config_t* conf,
    chain_t* output)
{
    if (nullptr == output || nullptr == conf || nullptr == context) {
        return;
    }

    context->conf = conf;

    z_stream* stream_tmp = &(context->stream);
    stream_tmp->zalloc = nullptr;
    stream_tmp->zfree = nullptr;
    stream_tmp->opaque = nullptr;

    context->flush = Z_NO_FLUSH;

    context->in = nullptr;
    context->out = output;

    context->curr_in = context->in;
    context->curr_out = context->out;

    context->last_in = context->in;

    context->level = conf->level;
}

gzip_status HttpModelGzip::gzip_deflate_init()
{
    gzip_config_t* conf = m_pConfig;
    gzip_context_t* context = m_pContext;
    z_stream* stream_tmp = &(context->stream);

    // TODO:FIXME:
    int res = deflateInit2(stream_tmp,
        //int(conf->level),
        Z_DEFAULT_COMPRESSION,
        Z_DEFLATED,
        //-conf->wbits,
        //conf->memlevel,
        MAX_WBITS + 16,
        MAX_MEM_LEVEL,
        // 8,
        Z_DEFAULT_STRATEGY);

    if (Z_OK != res) {
        LOG(Debug) << "deflateInit2() error\n";
        return gzip_error;
    }

    return gzip_ok;
}

gzip_status HttpModelGzip::gzip_deflate()
{
    // LOG(Debug) << "gzip_deflate\n";
    gzip_context_t* context = m_pContext;
    unsigned int before_gzip_size = 0;
    unsigned int after_gzip_size = 0;

    if (context->curr_in == nullptr || context->curr_out == nullptr) {
        // LOG(Debug) << "curr_in or curr_out is null\n";
        return gzip_done;
    }

    z_stream* stream = &context->stream;
    buffer_t* in = context->curr_in->buffer;
    buffer_t* out = context->curr_out->buffer;

    before_gzip_size = in->used - in->deal;
    stream->next_in = in->deal;
    stream->avail_in = before_gzip_size;
    context->curr_in = context->curr_in->next;

    context->flush = Z_NO_FLUSH;
    int res;
    do {
        before_gzip_size = in->used - in->deal;
        after_gzip_size = 0;

        if ((in->islast || context->curr_in == nullptr)
            && m_nEndData) {
            // It is last buffer to compress.
            // std::cout << "set flush as Z_FINISH\n";
            context->flush = Z_FINISH;
        }

        if (out->used == out->end) {
            // FIXME: when out isn't enough.
            context->curr_out = context->curr_out->next;
            if (context->curr_out == nullptr) {
                LOG(Debug) << "out buffer is not enough\n";
                return gzip_error;
                break;
            }
            out = context->curr_out->buffer;
        }

        stream->next_out = out->used;
        stream->avail_out = out->end - out->used;

        // printf("[before deflate]: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
        //     before_gzip_size, after_gzip_size,
        //     stream->next_in, stream->avail_in,
        //     stream->next_out, stream->avail_out);
        
        res = deflate(stream, context->flush);
        after_gzip_size = out->end - out->used - stream->avail_out;

        // printf("[after deflate]: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
        //     before_gzip_size, after_gzip_size,
        //     stream->next_in, stream->avail_in,
        //     stream->next_out, stream->avail_out);

        if (Z_NO_FLUSH == context->flush && Z_OK != res) {
            LOG(Debug) << "deflate() error1\n";
            return gzip_error;
        }

        if (Z_FINISH == context->flush && Z_STREAM_END != res) {
            LOG(Debug) << "deflate() error2\n";
            // return gzip_error;
        }

        in->deal += before_gzip_size;
        out->used = out->used + after_gzip_size;

    } while (0 == stream->avail_out && (in->islast || context->curr_in == nullptr));

    if (in->islast) {
        context->curr_out->buffer->islast = true;
    }

    if (0 == stream->avail_out) {
        // LOG(Debug) << "maybe more date to compress\n";
        return gzip_continue;
    }

    if (0 != stream->avail_in) {
        LOG(Info) << "compress error(compress incomplite)\n";
        return gzip_error;
    }

    if (Z_STREAM_END == res) {
        // LOG(Info) << "stream end\n";
        return gzip_done;
    }

    return gzip_continue;
}

gzip_status HttpModelGzip::gzip_deflate_end()
{
    // LOG(Debug) << "gzip_deflate_end\n";

    gzip_context_t* context = m_pContext;
    deflateEnd(&context->stream);
    return gzip_ok;
}


gzip_status HttpModelGzip::init()
{
    get_gzip_config(m_pConfig);

    m_pOutputChain = m_pPool->getNewChain(m_pConfig->buffers_4k);
    m_pPool->mallocSpace(m_pOutputChain, 1024 * 4);

    gzip_context_init(m_pContext, m_pConfig, m_pOutputChain);
    return gzip_deflate_init();
}

gzip_status HttpModelGzip::compress(chain_t* input, chain_t*& output,
    bool endData)
{
    // LOG(Debug) << "compress\n";

    m_nEndData = endData;

    if (nullptr == input) {
        // LOG(Debug) << "input is nullptr" << std::endl;
        return gzip_ok;
    }
    // clearData(m_pOutputChain);

    m_pContext->in = input;
    m_pContext->out = m_pOutputChain;

    m_pContext->curr_in = input;
    m_pContext->curr_out = m_pOutputChain;

    m_pContext->last_in = input;

    gzip_status res;

    do {
        res = gzip_deflate();
    } while (res == gzip_continue
        && nullptr != m_pContext->curr_in);

    if (m_nEndData)
        gzip_deflate_end();

    output = m_pOutputChain;
    return gzip_ok;
}

gzip_status HttpModelGzip::uncompress(chain_t* input, chain_t* output)
{
    return gzip_ok;
}
