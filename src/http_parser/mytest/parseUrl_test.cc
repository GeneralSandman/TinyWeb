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

#include "../http_parser.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct testUrl
{
    const char *url;
    bool valid;
    const char *host;
    const char *port;
    const char *path;
} testUrl;

testUrl urls[] = {

    {   // 0
        .url = "http://127.0.0.1:9090/index.html",
        .valid = true,
        .host = "127.0.0.1",
        .port = "9090",
        .path = "/index.html",
    },

    {   
        .url = "https://www.dissigil.cn",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "",
        .path = "",
    },

    {
        .url = "https://www.dissigil.cn/index.html",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "",
        .path = "/index.html",
    },

    {
        .url = "http://dissigil.cn/",
        .valid = true,
        .host = "dissigil.cn",
        .port = "",
        .path = "/",
    },

    {
        .url = "http://www.dissigil.cn:80",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "80",
        .path = "",
    },

    {
        .url = "http://www.dissigil.cn:8080/",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "8080",
        .path = "/",
    },

    {   
        .url = "http://www.dissigil.cn:8080/index/index.html",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "8080",
        .path = "/index/index.html",
    },

    {   
        .url = "www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = "",
    },

    {   
        .url = "www.dissigil.cn/index.html",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {//index : 9
        .url = "www.dissigil.cn:80",
        .valid = false,
        .host = "",
        .port = "",
        .path = "",
    },

    {
        .url = "www.dissigil.cn:80/index.html",
        .valid = false,
        .host = "",
        .port = "",
        .path = "",
    },

    {
        .url = "www.dissigil.cn/home/index.html",
        .valid = false,
        .host = "",
        .port = "",
        .path = "",
    },

    {
        .url = "http/www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http//www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http:www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http:/www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http:://www.dissigil.cn",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "/index.html",
        .valid = true,
        .host = "",
        .port = "",
        .path = "/index.html"},

    {
        .url = "http://127.0.0.1:9999/",
        .valid = true,
        .host = "127.0.0.1",
        .port = "9999",
        .path = "/"},

    {   // 19
        .url = "http://127.0.0.1/",
        .valid = true,
        .host = "127.0.0.1",
        .port = "",
        .path = "/"},

    {
        .url = "/asdf/adsd/sdf.html",
        .valid = true,
        .host = "",
        .port = "",
        .path = "/asdf/adsd/sdf.html"},

    {   //userinfo
        .url = "http://a:b@host.com:8080/p/a/t/h?query=string#hash",
        .valid = true,
        .host = "host.com",
        .port = "8080",
        .path = "/p/a/t/h"},

    {
        .url = "http://hostname:80/home?query=li#head",
        .valid = true,
        .host = "hostname",
        .port = "80",
        .path = "/home"},

    {
        .url = "http://hostname:80/home#?query=li#head",
        .valid = true,
        .host = "hostname",
        .port = "80",
        .path = "/home"},

    {
        .url = "http://hostname:80/home??query=li##head",
        .valid = true,
        .host = "hostname",
        .port = "80",
        .path = "/home"},

    {
        .url = "http://hostname:80/home?abcd=abcd?query=li##head",
        .valid = true,
        .host = "hostname",
        .port = "80",
        .path = "/home"},

    {
        .url = "http://hostname:80/hom#e?query=li#head",
        .valid = true,
        .host = "hostname",
        .port = "80",
        .path = "/hom"},

    {
        .url = "/home/index.html", //false
        .valid = true,
        .host = "",
        .port = "",
        .path = "/home/index.html"},

    {
        .url = "http://foo boar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 29
        .url = "http://foo\nboar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://foo\rboar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://hostname/",
        .valid = true,
        .host = "hostname",
        .port = "",
        .path = "/"},

    {
        .url = "http://hostname:444/",
        .valid = true,
        .host = "hostname",
        .port = "444",
        .path = "/"},

    {   
        .url = "hostname:443",
        .valid = false,
        .host = "hostname",
        .port = "443",
        .path = ""},

    {
        .url = "http://[1:2::3:4]/",
        .valid = true,
        .host = "1:2::3:4",
        .port = "",
        .path = "/"},

    {   // 35
        .url = "http://[1:2::3:4]:67/",
        .valid = true,
        .host = "1:2::3:4",
        .port = "67",
        .path = "/"},

    {   // 36
        .url = "http://[1:2::3:4]:67/index/hom#e?query=li#head",
        .valid = true,
        .host = "1:2::3:4",
        .port = "67",
        .path = "/index/hom"},

    {   // 37
        .url = "http://[2001:0000:0000:0000:0000:0000:1.9.1.1]/",
        .valid = true,
        .host = "2001:0000:0000:0000:0000:0000:1.9.1.1",
        .port = "",
        .path = "/"},

    {
        .url = "http://a.tbcdn.cn/p/fp/2010c/??fp-header-min.css,fp-base-min.css,"
            "fp-channel-min.css,fp-product-min.css,fp-mall-min.css,fp-category-min.css,"
            "fp-sub-min.css,fp-gdp4p-min.css,fp-css3-min.css,fp-misc-min.css?t=20101022.css",
        .valid = true,
        .host = "a.tbcdn.cn",
        .port = "",
        .path = "/p/fp/2010c/"},

    {   // 39
        .url = "/toto.html?toto=a%20b",
        .valid = true,
        .host = "",
        .port = "",
        .path = "/toto.html"},

    {
        .url = "/toto.html#titi",
        .valid = true,
        .host = "",
        .port = "",
        .path = "/toto.html"},

    {
        .url = "http://www.webmasterworld.com/r.cgi?f=21&d=8405&url=",
        .valid = true,
        .host = "www.webmasterworld.com",
        .port = "",
        .path = "/r.cgi"},

    {
        .url = "http://host.com:8080/p/a/t/h?query=string#hash",
        .valid = true,
        .host = "host.com",
        .port = "8080",
        .path = "/p/a/t/h"},

    {
        .url = "http://a:b@host.com:8080/p/a/t/h?query=string#hash",
        .valid = true,
        .host = "host.com",
        .port = "8080",
        .path = "/p/a/t/h"},

    {
        .url = "http://a:b@@hostname:443/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://:443/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://hostname:/",
        .valid = false, //FIXME:
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "a:b@hostname:443",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = ":443",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   //49
        .url = "hostname:",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   
        .url = "hostname:443/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "/foo bar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://a%20:b@host.com/",
        .valid = true,
        .host = "host.com",
        .port = "",
        .path = "/"},

    {
        .url = "/foo\rbar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://hostname::443/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://a::b@host.com/",
        .valid = true,
        .host = "host.com",
        .port = "",
        .path = "/"},

    {
        .url = "/foo\nbar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://@hostname/fo",
        .valid = true, //FIXME:
        .host = "hostname",
        .port = "",
        .path = "/fo"},

    {
        .url = "http://host\name/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 59
        .url = "http://host%name/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   
        .url = "http://host;ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://a!;-_!=+$@host.com/",
        .valid = true,
        .host = "host.com",
        .port = "",
        .path = "/"},

    {
        .url = "http://@/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://@jljljl/fo",
        .valid = true,
        .host = "jljljl",
        .port = "",
        .path = "/fo"},

    {
        .url = "http://toto@/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http:///fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host=ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://[fe80::a%25eth0]/",
        .valid = true,
        .host = "fe80::a%25eth0",
        .port = "",
        .path = "/"},

    {
        .url = "http://[fe80::a%eth0]/",
        .valid = true,
        .host = "fe80::a%eth0",
        .port = "",
        .path = "/"},

    {   // 69
        .url = "http://[fe80::a%]/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://[fe80::a%$HOME]/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://[%eth0]/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "/foo\tbar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "/foo\fbar/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "https://www.dissigil.cn/home#sd#qu?query=li",
        .valid = true,
        .host = "www.dissigil.cn",
        .port = "",
        .path = "/home"},

    {
        .url = "http://#fragment/",
        .valid = false,
        .host = "",
        .port = "",
        .path = "",
    },

    {
        .url = "http://?queurystring/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 77
        .url = "http://@hostname/",
        .valid = true,
        .host = "hostname",
        .port = "",
        .path = "/"},

    {
        .url = "http:///",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 79
        .url = "http://ABC.com/%7esmith/home.html",
        .valid = true,
        .host = "ABC.com",
        .port = "",
        .path = "/%7esmith/home.html"},

    {
        .url = "http://ABC.com/%7Esmith/home.html",
        .valid = true,
        .host = "ABC.com",
        .port = "",
        .path = "/%7Esmith/home.html"},

    {
        .url = "http://abc.com:80/~smith/home.html",
        .valid = true,
        .host = "abc.com",
        .port = "80",
        .path = "/~smith/home.html"},

    {   //82
        .url = "https://did.cn/../dsf",
        .valid = true, //FIXME: not sure valid or invalid
        .host = "did.cn",
        .port = "",
        .path = "/../dsf"},

    {
        .url = "https://dsjfll.cb.cb//",
        .valid = true, //FIXME: not sure valid or invalid
        .host = "dsjfll.cb.cb",
        .port = "",
        .path = "//"},

    {
        .url = "https:///ksdjf.cn/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "https://did.cn/../dsf",
        .valid = true, //FIXME: not sure valid or invalid
        .host = "did.cn",
        .port = "",
        .path = "/../dsf"},

    {
        .url = "https://pingtas.qq.com/webview/pingd?"
            "dm=join.qq.com&pvi=3220461568&si=s2709209088&"
            "url=/apply.php&arg=&ty=1&rdm=&rurl=&rarg=&adt="
            "&r2=49873873&r3=-1&r4=1&fl=&scr=1366x768&scl=24-"
            "bit&lg=zh-cn&jv=&tz=-8&ct=&ext=adid=&pf=&random=1528878932585",
        .valid = true,
        .host = "pingtas.qq.com",
        .port = "",
        .path = "/webview/pingd"},

    {
        .url = "http://www.baidu.com/link?url=-xBr1W_69PMdd5rdglCQljN1Rm5jA-7umx885_TZEZS",
        .valid = true,
        .host = "www.baidu.com",
        .port = "",
        .path = "/link"},

    {
        .url = "http://host%name/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 89
        .url = "http://host;ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://hos'name/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host\"ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host=ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host>ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host<ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host,ame/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host%name/fo",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {
        .url = "http://host/toto.html?toto=a%20b",
        .valid = true,
        .host = "host",
        .port = "",
        .path = "/toto.html"},

    {
        .url = "http://[]/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""},

    {   // 99
        .url = "http://[%eth]/",
        .valid = false,
        .host = "",
        .port = "",
        .path = ""
    }

};

void testParseUrl()
{
    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    HttpParserSettings settings;

    int len = sizeof(urls) / sizeof(urls[0]);

    HttpParser parser(&settings);
    parser.setType(HTTP_TYPE_REQUEST);

    for (int i = 0; i < len; i++)
    {
        alltest++;

        Url *result = new Url;
        // std::cout << i << "):" << std::endl;
        int tmp = parser.parseUrl(urls[i].url,
                strlen(urls[i].url),
                result);

        bool res = (tmp == -1) ? false : true;
        if (res == urls[i].valid)
        {
            if (res)
            {
                bool sameHost = (0==strncmp(urls[i].host, result->data + result->fields[HTTP_UF_HOST].offset, result->fields[HTTP_UF_HOST].len));
                bool samePort = (0==strncmp(urls[i].port, result->data + result->fields[HTTP_UF_PORT].offset, result->fields[HTTP_UF_PORT].len));
                bool samePath = (0==strncmp(urls[i].path, result->data + result->fields[HTTP_UF_PATH].offset, result->fields[HTTP_UF_PATH].len));

                if(sameHost && samePath && samePort)
                {
                    passtest++;
                }
                else
                {
                    notPass.push_back(i);
                }

            }
            else
            {
                passtest++;
            }
        }
        else
        {
            notPass.push_back(i);
        }

        if (res)
        {
            //printUrl(result);
        }
        else
        {
            //std::cout << "url invalid\n";
        }

        delete result;
    }

    std::cout << "[Parse Url Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass url index:\n";
        for (auto t : notPass)
            std::cout<< t << " ";
        std::cout << std::endl;
    }
    //We can't judge this urls is valid or invalid,
    //because '@' , IPv6 , UserInfo , port make judgement
    //become complicate.
    //So, we need to using api HttpParser::parseHost()
    //to solve this problem.
    //If you want get more information about host,
    //place view parseHost_test.cc file.
    //Good Luck, ^_^_^_^_^_^
}

int main()
{
    testParseUrl();
    return 0;
}
