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

#include <tiny_base/log.h>
#include <tiny_base/configer.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http_model_gzip.h>

#include <string.h>
#include <zlib.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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

void gzip_context_init(MemoryPool* pool,
    gzip_config_t* conf,
    gzip_context_t* context)
{
    if (nullptr == pool || nullptr == conf || nullptr == context) {
        return;
    }

    context->conf = conf;

    z_stream* stream_tmp = &(context->stream);

    stream_tmp->zalloc = nullptr;
    stream_tmp->zfree = nullptr;
    stream_tmp->opaque = nullptr;

    context->flush = Z_NO_FLUSH;

    context->in = pool->getNewChain(conf->buffers_4k);
    context->out = pool->getNewChain(conf->buffers_4k);

    unsigned int prealloc_size = 4 * 1024;
    pool->mallocSpace(context->in, prealloc_size);
    pool->mallocSpace(context->out, prealloc_size);
    LOG(Debug) << "prealloc in-chain:num(" << conf->buffers_4k << "),size("
               << prealloc_size << ")\n";
    LOG(Debug) << "prealloc out-chain:num(" << conf->buffers_4k << "),size("
               << prealloc_size << ")\n";

    context->curr_in = context->in;
    context->curr_out = context->out;

    context->last_in = context->in;

    context->level = conf->level;
}

gzip_status gzip_add_data(gzip_context_t* context,
    const char* data,
    unsigned int len)
{
    chain_t* chain;
    buffer_t* buffer;
    unsigned int buff_size;
    unsigned int predata_size;
    unsigned int empty_size;

    const char* pos = data;
    unsigned int left = len;
    unsigned int to_write = 0;

    LOG(Debug) << "all-write size:" << left << std::endl;

    chain = context->last_in;
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
        LOG(Debug) << "buffer:all-size(" << buff_size
                   << "),predata-size(" << predata_size
                   << "),postdata-size(" << predata_size + to_write << ")\n";

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

    context->last_in = chain;

    // unsigned int l = countChain(context->last_in);
    // LOG(Debug) << "last_in size:" << l << std::endl;
    // for (auto t = context->in; t != nullptr; t = t->next) {
        // if (t->buffer->islast) {
            // std::cout << "-";
        // } else {
            // std::cout << "+";
        // }
    // }

    return gzip_ok;
}

gzip_status gzip_deflate_init(gzip_context_t* context)
{
    gzip_config_t* conf = context->conf;
    z_stream* stream_tmp = &(context->stream);

    // TODO:FIXME:
    int res = deflateInit2(stream_tmp,
        //int(conf->level),
        Z_DEFAULT_COMPRESSION,
        Z_DEFLATED,
        //-conf->wbits,
        //conf->memlevel,
        MAX_WBITS + 16,
        8,
        Z_DEFAULT_STRATEGY);

    if (Z_OK != res) {
        LOG(Debug) << "deflateInit2() error\n";
        return gzip_error;
    }

    return gzip_ok;
}

gzip_status gzip_deflate(gzip_context_t* context)
{
    unsigned int before_gzip_size = 0;
    unsigned int after_gzip_size = 0;

    if (context->curr_in == nullptr || context->curr_out == nullptr) {
        LOG(Debug) << "curr_in or curr_out is null\n";
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

        if (in->islast) {
            std::cout << "set flush as Z_FINISH\n";
            context->flush = Z_FINISH;
        }

        if (out->used == out->end) {
            // FIXME: when out isn't enough.
            context->curr_out = context->curr_out->next;
            out = context->curr_out->buffer;
        }

        stream->next_out = out->used;
        stream->avail_out = out->end - out->used;

        printf("[before deflate]: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
            before_gzip_size, after_gzip_size,
            stream->next_in, stream->avail_in,
            stream->next_out, stream->avail_out);
        res = deflate(stream, context->flush);
        after_gzip_size = out->end - out->used - stream->avail_out;

        printf("[after deflate]: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
            before_gzip_size, after_gzip_size,
            stream->next_in, stream->avail_in,
            stream->next_out, stream->avail_out);

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

    } while (0 == stream->avail_out && in->islast);

    if (in->islast) {
        context->curr_out->buffer->islast = true;
    }

    if (0 == stream->avail_out) {
        LOG(Debug) << "maybe more date to compress\n";
        return gzip_continue;
    }

    if (0 != stream->avail_in) {
        LOG(Info) << "compress error(compress incomplite)\n";
        return gzip_error;
    }

    if (Z_STREAM_END == res) {
        LOG(Info) << "stream end\n";
        return gzip_done;
    }

    return gzip_continue;
}

gzip_status gzip_deflate_end(gzip_context_t* context)
{
    deflateEnd(&context->stream);
    LOG(Debug) << "gzip_deflate_end\n";
    return gzip_ok;
}

gzip_status gzip_body(gzip_context_t* context)
{
    gzip_deflate_init(context);

    gzip_status res;

    do {
        res = gzip_deflate(context);
    } while (res == gzip_continue);

    gzip_deflate_end(context);

    return gzip_ok;
}

gzip_status gzip_out(gzip_context_t* context,
    const std::string& outputfile)
{
    chain_t* chain;
    buffer_t* buffer;
    unsigned int size;
    int outputfd;

    chain_t* in_chain;
    buffer_t* in_buffer;

    //open output file
    outputfd = open(outputfile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (-1 == outputfd) {
        printf("create input-file(%s) error\n", outputfile.c_str());
        return gzip_error;
    }

    chain = context->out;
    // LOG(Debug) << "out size:" << countChain(chain) << std::endl;
    // for (auto t = context->out; t != nullptr; t = t->next) {
        // if (t->buffer->islast) {
            // std::cout << "-";
        // } else {
            // std::cout << "+";
        // }
    // }
    while (chain) {
        buffer = chain->buffer;
        size = buffer->used - buffer->begin;

        if (!size) {
            break;
        }

        write(outputfd, (char*)buffer->begin, size);
        printf("comperss-data[%p,%u](%.*s)\n", buffer->begin, size, size, buffer->begin);

        if (buffer->islast) {
            break;
        }
        chain = chain->next;
    }

    close(outputfd);
}
