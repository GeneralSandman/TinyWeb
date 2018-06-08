/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.generalsandman.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

// #include <tiny_base/log.h>

#include <iostream>
#include <boost/function.hpp>

typedef boost::function<void()> HttpCallback;
typedef boost::function<void()> HttpDataCallback;

class HttpParserSettings
{
  private:
	HttpCallback m_fGetMessage;
	HttpCallback m_fGetRequestLine;
	HttpCallback m_fGetHeader;
	HttpCallback m_fGetBody;
	HttpCallback m_fEndMessage;

  public:
	HttpParserSettings()
	{
		std::cout << "class HttpParserSettings constructor\n";
	}
	void setGetMessageCallback(const HttpCallback &c)
	{
		m_fGetMessage = c;
	}
	void setGetRequestLineCallback(const HttpCallback &c)
	{
		m_fGetRequestLine = c;
	}
	void setGetHeaderCallback(const HttpCallback &c)
	{
		m_fGetHeader = c;
	}
	void setGetBodyCallback(const HttpCallback &c)
	{
		m_fGetBody = c;
	}
	void setGetEndMessageCallback(const HttpCallback &c)
	{
		m_fEndMessage = c;
	}

	~HttpParserSettings()
	{
		std::cout << "class HttpParserSettings destructor\n";
	}
};

class HttpParser
{
  private:
  public:
	HttpParser()
	{
		std::cout << "class HttpParser constructor\n";
	}
	int execute(const std::string &stream, int &at)
	{
		std::cout << "function HttpParser::execute()\n";
	}
	~HttpParser()
	{
		std::cout << "class HttpParser destructor\n";
	}
};

#endif