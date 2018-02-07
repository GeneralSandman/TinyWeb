![](TinyWeb_logo.png)

# <a href='https://www.dissigil.cn'><img src='https://github.com/GeneralSandman/TinyWeb/blob/master/TinyWeb_logo.png' height='60' alt='TinyWeb'></a>

----------------

TinyWeb is a high-performance web server which is based on C++11.

- TinyWeb utilizes what's called an event-driven programming model.
- The implementation method is epoll+no-blocking I/O.
- The gobals of this application are high-performance,stability,simple configuration,
and low resource consumption.
- Module design is a efficient approach to design this system.

---------------------

## Quick Start

### Get Code
```
git clone https://github.com/GeneralSandman/TinyWeb
```

### Install
```
cd TinyWeb/src
make
make clean
```

### Configure
```
sudo cp TinyWeb.conf /
vim /TinyWeb.conf
```

Each line is a comment line which begin with #.

|name|meaning|
|-|-|
|***Basic-configure***|
|listen|listen-port|
|processpoll|number of process|
|Docs|path of html|
|HostName|the host of web|
|***Log-configure***|
|LogLevel|log level|
|LogPath|log path|
|DebugFile|Debug level file|
|InfoFile|Info level file||
|WarnFile|Warn level file||
|ErrorFile|Error level file||
|FatalFile|Fatal level file||

### Start Server

- Start server with default file ```/TinyWeb.conf```.You need to create this file if it does not exist otherwise server return error and exit.

```
sudo ./TinyWeb
```

- A configure file will be appointed when using ```-c <file>```.

```
sudo ./TinyWeb -c /home/li/TinyWeb.conf
```

- Start server with Debug level and ignore the level of configure file.(Server is under terminal's control because It is not a daemon process)

```
sudo ./TinyWeb -d -c /home/li/TinyWeb.conf
```



--------------


## Reference：

1.[Advanced Programming in the Unix Environment](https://en.wikipedia.org/wiki/Advanced_Programming_in_the_Unix_Environment)

2.[UNIX Network Programming](https://en.wikipedia.org/wiki/UNIX_Network_Programming)

3.C++ Primer

5.[Linux 多线程服务端编程：使用 muduo C++ 网络库](https://github.com/chenshuo/documents)

6.[C++ Reference](http://en.cppreference.com/w/cpp)

7.[高性能服务器编程](http://blog.csdn.net/column/details/high-perf-network.html)
