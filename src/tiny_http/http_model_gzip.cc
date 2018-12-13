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


#include <tiny_http/http_model_gzip.h>
#include <tiny_base/configer.h>

#include <string.h>


void get_zip_config(gzip_config_t *gzip_conf)
{
    Configer configer = Configer::getConfigerInstance();
    BasicConf basicConf = configer.getBasicConfig();

    gzip_conf->enable = basicConf.gzip;
    gzip_conf->level = basicConf.gzip_level;
    gzip_conf->buffers_4k = basicConf.gzip_buffers_4k;
    gzip_conf->min_len = basicConf.gzip_min_len;
    gzip_conf->wbits = 1;
    gzip_conf->memlevel = 0;
}


void gzip_context_init(gzip_config_t *conf, gzip_context_t *context)
{
    z_stream *stream_tmp = &(context->stream);

    stream_tmp->zalloc = nullptr;
    stream_tmp->zfree = nullptr;
    stream_tmp->opaque = nullptr;

    context->flush = Z_NO_FLUSH;

    context->in = getNewChain(nullptr, conf->buffers_4k);
    context->out = getNewChain(nullptr, conf->buffers_4k);

    context->curr_in = context->in;
    context->curr_out = context->out;

    context->level = conf->level;
}


gzip_status gzip_deflate_init(gzip_config_t *conf,
        gzip_context_t *context)
{
    z_stream *stream_tmp = &(context->stream);

    int res = deflateInit2(stream_tmp, int(conf->level),
            Z_DEFLATED,
            - conf->wbits,
            conf->memlevel,
            Z_DEFAULT_STRATEGY);

    if (Z_OK != res)
    {
        LOG(Debug) << "deflateInit2() error\n";
        return gzip_error;
    }

    return gzip_ok;
}


gzip_status gzip_deflate(gzip_context_t *context,
        http_buffer_t *buffer)
{
    zstream * stream = &context->stream;

    stream.next_in = curr_in->buffer->begin;
    stream.avail_in = curr_int->buffer->end - curr_int->buffer->begin;

    stream.next_out = curr_out->buffer->begin;
    stream.avail_out = curr_out->buffer->end - curr_out->buffer->begin;

    int res = deflate(&context->stream, context->flush);

    if (Z_OK != res && Z_STREAM_END != res)
    {
        LOG(Debug) << "deflate() error\n";
    }

    if (0 == context->stream.avail_out)
    {
        LOG(Debug) << "more date to compress\n";
        // TODO:get a buffer from memory-pool and
        // update the data of gzip_context_t
    }

    printf("deflate: next_in(%p),avail_in(%ud),next_out(%p),avail_out(%ud)\n",
            context->stream.next_in, context->stream.avail_int, 
            context->stream.next_out, context->stream.avail_out);

    if (Z_STREAM_END == res)
    {
        res = gzip_deflate_end(context);
        return (gzip_ok == res) ? gzip_ok : gzip_error;
    }

}


gzip_status gzip_deflate_end(gzip_context_t *context)
{
    deflateEnd(context->stream);
}


gzip_status gzip_add_data(gzip_context_t *context,
        const std::string &data)
{
    http_chain_t *chain = context->curr_in;
    http_buffer_t *buffer;
    unsigned int buff_size;

    const char * pos = data.c_str();
    unsigned int left = data.size();

    while (left)
    {
        buffer = chain->buffer;
        buff_size = buffer->end - buffer->begin;

        memcpy((void *)buffer->begin, (const void *)pos, buff_size);

        left -= buff_size;
        pos+=buff_size;
    }
    assert(pos - data.c_str() == data.size());

    return gzip_ok;

}

gzip_status gzip_body(gzip_context_t *cont,
        const std::string &data)
{

    gzip_config_t conf;
    gzip_context_t context;
    get_zip_config(&conf);

    gzip_context_init(&conf, &context);

    gzip_deflate_init(&context);

    gzip_status res;
    http_buffer_t *buffer = nullptr;

    do
    {
        res = gzip_deflate(&context, buffer);

    }while(res != gzip_error)

    gzip_deflate_end(&context);

    gzip_out(&context);
}

gzip_status gzip_out(gzip_context_t *context)
{
    http_chain_t * chain;
    http_buffer_t * buffer;
    size_t size;

    chain = context->out;
    while (chain)
    {
        buffer = chain->buffer;
        size = buffer->end - buffer->begin;

        printf("%.*s", size, buffer->begin);
    }
}
