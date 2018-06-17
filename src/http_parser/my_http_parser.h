/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.generalsandman.cn
 */

/*---class HttpParser---
 *
 ****************************************
 *
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

// #include <tiny_base/log.h>

#include "http.h"

#include <iostream>
#include <stdio.h>
#include <map>
#include <list>
#include <boost/function.hpp>

#define CR '\r'
#define LF '\n'
#define isNum(c) ('0' <= (c) && (c) <= '9')
#define isAlpha(c) ('a' <= (c) && (c) <= 'z' || 'A' <= (c) && (c) <= 'Z')
#define isAlphaNum(c) (isNum(c) || isAlpha(c))
#define toLower(c) (unsigned char)(c | 0x20)
// #define toUpper(c) (unsigned char)(('a' <= (c) && (c) <= 'z') ? c | 0x20 : c) //not finished
#define isHexChar(c) (isNum(c) || ('a' <= toLower(c) && toLower(c) <= 'f'))

#define isMarkChar(c) ((c) == '-' || (c) == '_' || (c) == '.' ||                              \
					   (c) == '!' || (c) == '~' || (c) == '*' || (c) == '\'' || (c) == '(' || \
					   (c) == ')')
#define isUserInfoChar(c) (isAlphaNum(c) || isMarkChar(c) || (c) == '%' ||                       \
						   (c) == ';' || (c) == ':' || (c) == '&' || (c) == '=' || (c) == '+' || \
						   (c) == '$' || (c) == ',')

#define isHostChar(c) (isAlphaNum(c) || (c) == '.' || (c) == '-' || (c) == '_')

#define isIpv4Char(c) (isNum(c) || (c) == '.')
#define isIpv6Char(c) (isHexChar(c) || (c) == ':' || (c) == '.')

//the values set to HttpParser::m_nType
enum httpParserType
{
	HTTP_REQUEST = 1,
	HTTP_RESPONSE,
	HTTP_BOTH
};

enum httpUrlField
{
	HTTP_UF_SCHEMA = 0,
	HTTP_UF_HOST = 1,
	HTTP_UF_PORT = 2,
	HTTP_UF_PATH = 3,
	HTTP_UF_QUERY = 4,
	HTTP_UF_FRAGMENT = 5,
	HTTP_UF_USERINFO = 6,
	HTTP_UF_MAX = 7
};

enum state
{
	s_error = 1,

	s_start_resp_or_requ,

	s_resp_start,
	s_resp_H,
	s_resp_HT,
	s_resp_HTT,
	s_resp_HTTP,
	s_resp_HTTP_slash,
	s_resp_version_major,
	s_resp_version_dot,
	s_resp_version_minor,
	s_resp_status_code_start,
	s_resp_status_code,
	s_resp_status_phrase_start,
	s_resp_status_phrase,
	s_resp_line_almost_done,
	s_resp_line_done,

	s_requ_start,
	s_requ_method_start,
	s_requ_method,
	s_requ_url_begin,
	s_requ_url, //add
	s_requ_schema,
	s_requ_schema_slash,
	s_requ_schema_slash_slash,
	s_requ_server_start,
	s_requ_server,
	s_requ_path,
	s_requ_server_at,
	s_requ_query_string_start,
	s_requ_query_string,
	s_requ_fragment_start,
	s_requ_fragment,

	s_requ_HTTP_start,
	s_requ_H,
	s_requ_HT,
	s_requ_HTT,
	s_requ_HTTP,
	s_requ_HTTP_slash,
	s_requ_version_major,
	s_requ_version_dot,
	s_requ_version_minor,
	s_requ_line_almost_done,
	s_requ_line_done,

	//heaser statue
	s_header_start,
	s_header,
	s_header_almost_done,
	s_header_done,
	s_headers_almost_done,
	s_headers_done,

	s_body_start,
	s_body,
	s_body_done,

	s_chunk

};

enum http_host_state
{
	s_http_host_error = 1,
	s_http_userinfo_start,
	s_http_userinfo,
	s_http_host_start,
	s_http_host_v6_start,
	s_http_host_v4,
	s_http_host_v6,
	s_http_host_v6_end,
	s_http_host_v6_zone_start,
	s_http_host_v6_zone,
	s_http_host_port_start,
	s_http_host_port
};

enum http_header_state
{
	s_http_header_error = 1,

	s_http_header_start,
	s_http_header_key_start,
	s_http_header_key,
	s_http_header_colon,
	s_http_header_space,
	s_http_header_value_start,
	s_http_header_value,
	s_http_header_almost_done,
	s_http_header_done,
	s_http_headers_almost_done,
	s_http_headers_done,
};

typedef struct Url
{
	unsigned int port : 16;
	unsigned int field_set : 16;
	char *data;
	struct
	{
		unsigned int offset : 16;
		unsigned int len : 16;
	} fields[HTTP_UF_MAX];
} Url;

enum httpHeaderField
{
	HTTP_HF__SCHEMA = 0,
	HTTP_HF__HOST = 1,
	HTTP_HF_KEEP_CONNECTION = 2,
	HTTP_HF_CONTENT_LENGTH = 3,
	HTTP_HF_UPGRADE = 4,
	HTTP_HF_TRANSFER_ENCODING_CHUNKED = 5,
	HTTP_HF__USERINFO = 6,
	HTTP_HF__MAX = 7
};

typedef struct HttpHeader
{
	std::string key; //take place with string_t
	std::string value;
} HttpHeader;

typedef struct HttpHeaders
{
	HttpHeader *host;
	HttpHeader *connection;
	HttpHeader *content_lenght;
	HttpHeader *transfer_encoding;
	HttpHeader *if_modified_since;
	HttpHeader *referer;
	HttpHeader *upgrade;

	std::list<HttpHeader *> generals; //take place in list_t
} HttpHeaders;

void printUrl(const Url *url);

typedef boost::function<int()> HttpCallback;
typedef boost::function<int()> HttpDataCallback;

class HttpParserSettings
{
  private:
	std::map<std::string, HttpCallback> m_nReflection;

	HttpCallback m_fGetMessage;
	HttpCallback m_fGetUrl;
	HttpCallback m_fGetStatus;
	HttpCallback m_fGetRequestLine;

	HttpCallback m_fGetHeader;
	HttpCallback m_fHeaderKey;
	HttpCallback m_fHeaderValue;

	HttpCallback m_fGetBody;
	HttpCallback m_fEndMessage;

	HttpCallback m_fGetChunk;
	HttpCallback m_fEndChunk;

  public:
	HttpParserSettings()
	{
		// std::cout << "class HttpParserSettings constructor\n";
	}

	void setGetMessageCallback(const HttpCallback &c)
	{
		m_fGetMessage = c;
		m_nReflection["getMessage"] = c;
	}

	void setGetRequestLineCallback(const HttpCallback &c)
	{
		m_fGetRequestLine = c;
		m_nReflection["getRequestLine"] = c;
	}

	void setGetHeaderCallback(const HttpCallback &c)
	{
		m_fGetHeader = c;
		m_nReflection["getHeader"] = c;
	}

	void setGetBodyCallback(const HttpCallback &c)
	{
		m_fGetBody = c;
		m_nReflection["getBody"] = c;
	}

	void setGetEndMessageCallback(const HttpCallback &c)
	{
		m_fEndMessage = c;
		m_nReflection["endMessage"] = c;
	}

	HttpCallback getMethodByName(const std::string &fname)
	{
		auto p = m_nReflection.find(fname);
		// if (p == m_nReflection.end())
		// 	return nullptr;
		// else
		return m_nReflection[fname];
		//FIXME:
	}

	~HttpParserSettings()
	{
		// std::cout << "class HttpParserSettings destructor\n";
	}

	friend class HttpParser;
};

class HttpParser
{
  private:
	HttpParserSettings *m_pSettings;

	unsigned int m_nType : 2; //Http request or response
	unsigned int m_nFlags : 8;
	unsigned int m_nState;
	unsigned int m_nHeaderState;

	unsigned long long m_nRead;
	unsigned long long m_nContentLength;

	unsigned short m_nHttpVersionMajor;
	unsigned short m_nHttpVersionMinor;
	unsigned int m_nStatusCode : 16;
	unsigned int m_nMethod : 8;
	unsigned int m_nErrno : 7;
	unsigned int m_nIsUpgrade : 1;

	void *m_pData;

  public:
	HttpParser(HttpParserSettings *set = nullptr)
		: m_pSettings(set),
		  m_nType(0),//FIXME:
		  m_nFlags(0),
		  m_nState(0),
		  m_nHeaderState(0),
		  m_nRead(0),
		  m_nContentLength(0),
		  m_nHttpVersionMajor(0),
		  m_nHttpVersionMinor(0),
		  m_nStatusCode(0),
		  m_nMethod(0),
		  m_nErrno(0),
		  m_nIsUpgrade(0),
		  m_pData(nullptr)
	{
		// std::cout << "class HttpParser constructor\n";
	}

	void setType(enum httpParserType type);

	enum http_errno getErrno()
	{
		return (enum http_errno)m_nErrno;
	}

	int invokeByName(const char *funName,
					 const char *data,
					 unsigned int len);

	// enum state
	enum http_host_state parseHostChar(const char ch, enum http_host_state s);
	int parseHost(const char *stream, int &at, int len, Url *&result, bool has_at_char);

	enum state parseUrlChar(const char ch, enum state s);
	int parseUrl(const char *stream, int &at, int len, Url *&result);

	enum http_header_state parseHeaderChar(const char ch, enum http_header_state s);
	int parseHeader(const char *stream, int &at, int len);

	int execute(const char *stream, int &at, int len);

	~HttpParser()
	{
		// std::cout << "class HttpParser destructor\n";
	}
};

#endif