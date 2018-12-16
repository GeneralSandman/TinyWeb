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

#include <assert.h>
#include <string.h>
#include <zlib.h>

#include <assert.h>
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
    configer.setConfigerFile("../../TinyWeb.conf");
    configer.loadConfig(true);
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
    LOG(Debug) << "in-chain:num(" << conf->buffers_4k << "),size("
               << prealloc_size << ")\n";
    LOG(Debug) << "out-chain:num(" << conf->buffers_4k << "),size("
               << prealloc_size << ")\n";

    context->curr_in = context->in;
    context->curr_out = context->out;

    context->last_in = context->in;

    context->level = conf->level;
}

gzip_status gzip_deflate_init(gzip_config_t* conf,
    gzip_context_t* context)
{
    z_stream* stream_tmp = &(context->stream);

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

    before_gzip_size = in->used - in->begin;
    stream->next_in = in->begin;
    stream->avail_in = before_gzip_size;

    stream->next_out = out->begin;
    stream->avail_out = out->end - out->begin;

    printf("deflate: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
        before_gzip_size, after_gzip_size,
        stream->next_in, stream->avail_in,
        stream->next_out, stream->avail_out);

    context->flush = in->islast ? Z_FINISH : Z_NO_FLUSH;
    int res = deflate(stream, context->flush);

    after_gzip_size = out->end - out->begin - stream->avail_out;
    out->used = out->begin + after_gzip_size;

    printf("deflate: before_gzip_size(%u),after_gzip_size(%u),next_in(%p),avail_in(%u),next_out(%p),avail_out(%u)\n",
        before_gzip_size, after_gzip_size,
        stream->next_in, stream->avail_in,
        stream->next_out, stream->avail_out);

    context->curr_in = context->curr_in->next;
    context->curr_out = context->curr_out->next;

    if (Z_OK != res && Z_STREAM_END != res) {
        LOG(Debug) << "deflate() error\n";
        return gzip_error;
    }

    if (0 == stream->avail_out) {
        LOG(Debug) << "more date to compress\n";
        // TODO:get a buffer from memory-pool and
        // update the data of gzip_context_t
        return gzip_continue;
    }

    if (0 != stream->avail_in) {
        LOG(Info) << "compress error(compress incomplite)" << std::endl;
        return gzip_error;
    }

    if (Z_STREAM_END == res) {
        LOG(Info) << "stream end\n";
        res = gzip_deflate_end(context);
        return (gzip_ok == res) ? gzip_ok : gzip_error;
    }

    return gzip_continue;
}

gzip_status gzip_deflate_end(gzip_context_t* context)
{
    deflateEnd(&context->stream);
    LOG(Debug) << "gzip_deflate_end\n";
    return gzip_done;
}

gzip_status gzip_add_data(gzip_context_t* context,
    const char* data,
    unsigned int len)
{
    chain_t* chain = context->last_in;
    buffer_t* buffer;
    unsigned int buff_size;
    unsigned int predata_size;
    unsigned int empty_size;

    const char* pos = data;
    unsigned int left = len;
    unsigned int to_write = 0;

    LOG(Debug) << "all-write size:" << left << std::endl;

    while (left && nullptr != chain) {
        buffer = chain->buffer;
        buff_size = buffer->end - buffer->begin;
        predata_size = buffer->used - buffer->begin;
        empty_size = buff_size - predata_size;

        to_write = (left > empty_size) ? empty_size : left;

        if (buffer->used == buffer->end) { 
            buffer->islast = false;
            chain = chain->next;
            continue;
        } 

        memcpy((void*)buffer->used, (const void*)pos, to_write);
        LOG(Debug) << "buffer:all-size(" << buff_size
                   << "),predata-size(" << predata_size
                   << "),postdata-size(" << predata_size + to_write << ")\n";

        buffer->used = buffer->used + to_write;

        buffer->islast = true;

        left -= to_write;
        pos += buff_size;

        if (left) {
            buffer->islast = false;
            chain = chain->next;
        }
    }

    context->last_in = chain;

    LOG(Debug) << "last_in size:" << countChain(context->last_in) << std::endl;

    return gzip_ok;
}

gzip_status gzip_body(MemoryPool* pool,
    gzip_config_t* conf,
    gzip_context_t* context)
{
    gzip_deflate_init(conf, context);

    gzip_status res;

    do {
        res = gzip_deflate(context);
    } while (res == gzip_continue);

    gzip_deflate_end(context);
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
    in_chain = context->in;
    LOG(Debug) << "out size:" << countChain(chain) << std::endl;
    while (chain) {
        buffer = chain->buffer;
        size = buffer->used - buffer->begin;

        write(outputfd, (char*)buffer->begin, size);
        printf("comperss-data[%p,%u](%.*s)\n", buffer->begin, size, size, buffer->begin);

        chain = chain->next;

        if (in_chain->buffer->islast) {
            break;
        }
        in_chain = in_chain->next;
    }

    close(outputfd);
}
