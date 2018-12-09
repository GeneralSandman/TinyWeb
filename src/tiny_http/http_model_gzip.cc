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


void gzip_context_init(gzip_context_t *context)
{
    z_stream *stream_tmp = &(context->stream);

    stream_tmp->zalloc = nullptr;
    stream_tmp->zfree = nullptr;
    stream_tmp->opaque = nullptr;
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
        http_buffer *buffer)
{
    int res = deflate(&context->stream, context->flush);

    if (Z_OK != res && Z_STREAM_END != res)
    {
        LOG(Debug) << "deflate() error\n";
    }

}


gzip_status gzip_deflate_end(gzip_context_t *context)
{

}


gzip_status gzip_body(gzip_context_t *context)
{

}
