![](TinyWeb_logo.png)



----------------

## High-Performance Web Server which is based on C++11.

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)


- ### Event-driven Epoll + Asynchronous I/O.
- ### High-performance, Stable, Sample Configuration.
- ### Modularization Programming.

---------------------

# Quick Start

## Get Code
```
git clone https://github.com/GeneralSandman/TinyWeb
```

## Install
```
cd TinyWeb/src
mkdir build
cd build
cmake ../ && make
```

## Configure
```
sudo cp TinyWeb.conf /
vim /TinyWeb.conf
```

### Configure-file :

- ```json``` formate
- Two parts in Configure-file:
    - ```develop``` for debug
    - ```product``` for product environment


### Configure-file Example :

```
{
    "develop": {

        "basic": {
            "worker": 2,
            "pid": "/var/run/TinyWeb.pid",
            "sendfile": true,
            "mimetype": "mime.types",
            "chunked": true,
            "gzip": true,
            "gzip_level" : 2,
            "gzip_buffers_4k" : 16,
            "gzip_min_len": 1024,
            "gzip_http_version": ["1.0", "1.1"],
            "gzip_mime_type": ["text/html", "text/css"]
        },

        "fcgi": {
            "enable": true,
            "connect_timeout": 300,
            "send_timeout": 300,
            "read_timeout": 300,
            "keep_connect": true
        },

        "cache": [
            {
                "name": "cache_test",
                "server_address": "127.0.0.1:7979",
                "path": "/home/dell/TinyWeb/cache_files",
                "space_max_size": "4194304",
                "expires": "3600"
            },
            {
                "name": "cache_test2",
                "server_address": "127.0.0.1:7973",
                "path": "/home/dell/TinyWeb/cache_files",
                "space_max_size": "4194304",
                "expires": "3600"
            }
        ],

        "server": [
            {
                "listen": 9090,
                "servername": [
                    "127.0.0.1",
                    "dissigil.cn"
                ],
                "www": "/home/dell/TinyWeb/www",
                "indexpage": [
                    "index.html",
                    "index.htm",
                    "index.php"
                ],
                "errorpage": [
                    {
                        "code" : [404],
                        "path" : "/home/dell/TinyWeb/www",
                        "file" : "404.html"
                    },
                    {
                        "code" : [500,502,503,504],
                        "path" : "/home/dell/TinyWeb/www",
                        "file" : "50x.html"
                    }
                ],
                "fcgi": [
                    {
                        "pattern": "*.php",
                        "path" : "/home/dell/TinyWeb/www/test",
                        "indexpage": ["index.php", "index2.php"],
                        "listen": "127.0.0.1:9090"
                    },
                    {
                        "pattern": "*.cgi",
                        "path" : "/home/dell/TinyWeb/www/test",
                        "indexpage": ["index.php", "index2.php"],
                        "indexpage": ["index.cgi", "index2.cgi"],
                        "listen": "127.0.0.1:9091"
                    }
                ],
                "cache": "cache_test"
            }
        ],

        "log": {
            "level": "Debug",
            "path": "/home/dell/TinyWeb/log",
            "debugfile": "debug.log",
            "infofile": "info.log",
            "warnfile": "warn.log",
            "errorfile": "error.log",
            "fatalfile": "fatal.log"
        }
    }
}

```

### Configure Explaination :


 <table>
    <tr>
        <td></td> 
        <td>name</td> 
        <td>meaning</td> 
        <td>type</td>
    </tr>
    <tr>
        <td rowspan="11">basic</td> 
        <td>worker</td> 
        <td>worker number</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>pid</td> 
        <td>file record master's pid</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>sendfile</td> 
        <td>turn on/off sendfile</td> 
        <td>bool</td>
    </tr>
    <tr>
        <td>mimetype</td> 
        <td>mimetype file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>chunked</td> 
        <td>turn on/off chunked</td> 
        <td>bool</td>
    </tr>
    <tr>
        <td>gzip</td> 
        <td>turn on/off gzip</td> 
        <td>bool</td>
    </tr>
    <tr>
        <td>gzip_level</td> 
        <td>compress level</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>gzip_buffers_4k</td> 
        <td>buffer number for compress</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>gzip_min_len</td> 
        <td>compress body if longer this value</td>
        <td>int</td>
    </tr>
    <tr>
        <td>gzip_http_version</td> 
        <td>apply gzip configure if http-version match</td> 
        <td>string list</td>
    </tr>
    <tr>
        <td>gzip_mime_type</td> 
        <td>compress file which has those mime-type</td> 
        <td>string list</td>
    </tr>
    <tr>
        <td rowspan="5">fcgi</td> 
        <td>enable</td> 
        <td>turn on/off</td> 
        <td>bool</td>
    </tr>
    <tr>
        <td>connect_timeout</td> 
        <td>connect timeout</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>send_timeout</td> 
        <td>send timeout</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>read_timeout</td> 
        <td>read timeout</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>keep_connect</td> 
        <td>keep connect</td> 
        <td>bool</td>
    </tr>
    <tr>
        <td rowspan="5">cache (type cache-struct list)</td> 
        <td>name</td> 
        <td>cache name</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>server_address</td> 
        <td>source server address</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>path</td> 
        <td>path of stroe cache file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>space_max_size</td> 
        <td>max size of cache(kB)</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>expires</td> 
        <td>expires time (second)</td> 
        <td>int</td>
    </tr>
    <tr>
        <td rowspan="7">server (type is list)</td> 
        <td>listen</td> 
        <td>list port</td> 
        <td>int</td>
    </tr>
    <tr>
        <td>servername</td> 
        <td>server host name</td> 
        <td>string list</td>
    </tr>
    <tr>
        <td>www</td> 
        <td>website path</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>indexpage</td> 
        <td>default index page for url is path</td> 
        <td>string list</td>
    </tr>
    <tr>
        <td>errorpage</td> 
        <td>default error page</td> 
        <td>errorpage-struct list</td>
    </tr>
    <tr>
        <td>fcgi</td> 
        <td>fcgi config</td> 
        <td>fcgi-struct list</td>
    </tr>
    <tr>
        <td>cache</td> 
        <td>cache name in cache struct</td> 
        <td>name</td>
    </tr>
    <tr>
        <td rowspan="7">log</td> 
        <td>level</td> 
        <td>log level</td> 
        <td>string of Debug/Info/Warn/Error/Fatal</td>
    </tr>
    <tr>
        <td>path</td> 
        <td>log path</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>debugfile</td> 
        <td>debug-log file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>infofile</td> 
        <td>info-log file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>warnfile</td> 
        <td>warn-log file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>errorfile</td> 
        <td>error-log file</td> 
        <td>string</td>
    </tr>
    <tr>
        <td>fatalfile</td> 
        <td>fatal-log file</td> 
        <td>string</td>
    </tr>
</table>

### ```errorpage``` struct :

|name|meaning|type|
|----|----|----|
|code|http status code|int list|
|path|error page path|string|
|file|error page file|string|


### ```fcgi``` struct :

|name|meaning|type|
|----|----|----|
|indexpage|index page|string list|
|pattern|fcgi pattern|string|
|path|fcgi file path|string|
|listen|fcgi server address|string|



## Start Server

- Start server with default file ```/TinyWeb.conf```.   Return error and exit if config-file doesn't exits.

```
sudo ./TinyWeb
```

- Get master's pid.
```
cat <pid-file>
```

- A configure file will be appointed when using    ```-c <config-file>```.

```
sudo ./TinyWeb -c <config-file>
```

- Start server with Debug level and ignore the level of configure file.(Server is under terminal's control because it's not a daemon.)

```
sudo ./TinyWeb -d -c <config-file>
```

- Stop server safely.
```
sudo ./TinyWeb -o stop

cat <pid-file>
sudo kill -s SIGQUIT <master-pid> 
```

- Stop server immeditely.
```
sudo ./TinyWeb -o stop

cat <pid-file>
sudo kill -s SIGTERM <master-pid> 
sudo kill -s SIGINT <master-pid> 
```

- Restart server.
```
sudo ./TinyWeb -o restart

cat <pid-file>
sudo kill -s SIGUSR1 <master-pid> 
```

- Reload server with new configure file.
```
sudo ./TinyWeb -o reload [-c <config-file>]

cat /var/run/TinyWeb.pid
sudo kill -s SIGUSR2 <master-pid> 
```

- Test configure file is vliad or not.
```
sudo ./TinyWeb --tcf /path/path/configfile
```

- Get TinyWeb version.
```
sudo ./TinyWeb -v
```


---------------------

# Version Information

| Version | Improvements |
| --------|--------------|
|||
|||

---------------------

# Guide
- ## [TinyWeb Develop Guides](http://www.dissigil.cn/TinyWeb-0/)

- ## [Configure File Guides](http://www.dissigil.cn/TinyWeb-configure/)


--------------

# [Develop Idea & Challenge](http://github.com/GeneralSandman/docs/Idea.md)

---------------



# Reference：

1. ## C++ Primer

2. ## [Advanced Programming in the Unix Environment](https://en.wikipedia.org/wiki/Advanced_Programming_in_the_Unix_Environment)

3. ## [UNIX Network Programming](https://en.wikipedia.org/wiki/UNIX_Network_Programming)

4. ## [Linux 多线程服务端编程：使用 muduo C++ 网络库](https://github.com/chenshuo/documents)

5. ## [C++ Reference](http://en.cppreference.com/w/cpp)

6. ## [高性能服务器编程](http://blog.csdn.net/column/details/high-perf-network.html)
