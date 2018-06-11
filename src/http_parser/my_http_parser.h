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
#include <map>
#include <boost/function.hpp>

#define CR '\r'
#define LF '\n'
#define isNum(c) ('0' <= (c) && (c) <= '9')
#define isAlpha(c) ('a' <= (c) && (c) <= 'z' || 'A' <= (c) && (c) <= 'Z')
#define isAlphaNum(c) (isNum(c) || isAlpha(c))
#define toLower(c) (unsigned char)(('A' <= (c) && (c) <= 'Z') ? c | 0x20 : c)
#define toUpper(c) (unsigned char)(('a' <= (c) && (c) <= 'z') ? c | 0x20 : c)//not finished

//the values set to HttpParser::m_nType
enum httpParserType
{
	HTTP_REQUEST = 1,
	HTTP_RESPONSE,
	HTTP_BOTH
};

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
		std::cout << "class HttpParserSettings constructor\n";
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
		std::cout << "class HttpParserSettings destructor\n";
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
		  m_pData(nullptr)
	{
		std::cout << "class HttpParser constructor\n";
	}

	void setType(enum httpParserType type);

	enum http_errno getErrno()
	{
		return (enum http_errno)m_nErrno;
	}

	int invokeByName(const char *funName,
					 const char *data,
					 unsigned int len)
	{
		std::cout << "invoke function by name:" << funName << std::endl;
		if (m_pSettings == nullptr)
			return -1;
		std::string fname(funName);
		HttpCallback fun = m_pSettings->getMethodByName(fname);
		if (fun == nullptr)
			return -1;

		return fun();
	}

	int execute(const std::string &stream, int &at, int len);

	~HttpParser()
	{
		std::cout << "class HttpParser destructor\n";
	}
};

#endif