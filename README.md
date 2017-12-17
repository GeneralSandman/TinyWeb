```
 _____ _           __        __   _     
|_   _(_)_ __  _   \ \      / /__| |__  
  | | | | '_ \| | | \ \ /\ / / _ \ '_ \ 
  | | | | | | | |_| |\ V  V /  __/ |_) |
  |_| |_|_| |_|\__, | \_/\_/ \___|_.__/ 
               |___/                    
```

TinyWeb是一款基于C++11开发的高性能Web服务器（性能还需要测试），基本方式进程池+事件驱动。在开发的时候参考Nginx的设计原理，后添加了自己的想法进行设计。他的优点主要是：高性能，配置方便，占用内存小，可方便进行热部署。实现了Web服务器的基本功能：代理，负载均衡，高性能日志。
本项目在设计的时候注重模块化设计，其中日志功能、配置功能、网络部分均可抽出为其他开发者使用。详情见下文分析。
其中网络部分被抽出作为一个时间驱动式网络库，可以自定义通信逻辑，被应用在另一个独立项目：[Sigil（一个分布式键值数据库）](https://www.github.com/GeneralSandman/Sigil)

- 参考muduo实现高性能事件驱动式网络库
- 参考nginx实现高效进程通信模型
- 参考twisted实现异步式编程


> # 一、如何安装TinyWeb
```
git clone https://www.github.com/GeneralSandman/TinyWeb
cd TinyWeb
make TinyWeb
make clean
```

> # 二、TinyWeb配置方法
名称不区分大小写，数值区分大小写;#开头为注释行
|名称|含义|
|-|-|
|***基本配置***|
|listen|监听端口|
|processpoll|进程池数量|
|Docs|html路径|
|HostName|网址|
|***日志配置***|
|LogLevel|日志等级|
|LogPath|日志路径|
|DebugFile|Debug等级日志的文件名|
|InfoFile|Info等级日志的文件名|
|WarnFile|Warn等级日志的文件名|
|ErrorFile|Error等级日志的文件名|
|FatalFile|Fatal等级日志的文件名|
|***负载均衡***|
|||
|||
|||



> # 三、启动TinyWeb

- 直接启动，默认配置文件为```/TinyWeb.conf```，该文件不存在，或格式错误时均返回错误。

```
sudo ./TinyWeb
```

- 启动时指定配置文件路径,该文件不存在，或格式错误时返回错误。

```
sudo ./TinyWeb -c /home/li/TinyWeb.conf
```

- 命令行控制以Debug模式运行,忽略配置中的等级。

```
sudo ./TinyWeb -d -c /home/li/TinyWeb.conf
```

- 如果日志等级为```Debug```,不会切换至daemon进程，可由终端控制



> # 四.如何进行测试test下的相关例程（主要是单元测试）


- ```server_test``` 监听80端口，600s后终止程序
```
make lib
make server_test
sudo ./server_test
```

- ```client.py``` 作为客户端连接
```
python client.py 9090 139.199.13.50 80
```


------------------



> # 设计难点及解决之道
|难点|解决办法|
|-|-|
|进程间通信机制||
|||
|||
|||



> # 各种成熟与不成熟想法
- design cache class
- nginx定义了结构体描述接受到信号的行为，定义一个数组注册收到信号的行为。
- 提升程序为demon进程（demon程序标准输入输出如何处理?）
- 优化loggger，解决logger生命周期过短的问题
- Semphore
- SharedMemory
- parser 可以精确地识别出路径名，和url（需要更加健壮）(应用正则表达式验证正确性)
- 优化logger，
- 如何处理请求目录
- 通过list维护一批请求头，响应头
- 如何处理url中包含```../```的问题
- 添加内存缓存 mushroom
- 将sigil升级为一个高效的内存缓存系统，为web缓存提供帮助
- 添加proxy
- add MIME type
- 为Connection添加close()功能来作为对shutdownWrite()的补充
- 如何支持中文url??????
- 如何使用进程池来处理多个连接
    - master进程负责EventLoop.loop()循环，负责分配连接
    - slaver进程监听与master的socket，接受请求，工作
    - 一个Connection只能属于一个进程，不能多个进程同时处理
    - Connection的读，写，关闭事件均有master进程负责，
        并把相应的事件传给slaver
- EventLoop与ProcessPoll的逻辑关系如何设计？？？？？
- 每个Process均有一个EventLoop，不过监听的event不同
    - master监听网络事件
    - slaver监听master的通信事件，和每个Connection的定时器事件
- 残留的Connection应该有Server释放
- 残留的Protocol应该由Factory释放
- Connection不知道Protocol的存在，Server不知道Factory的存在
- 为每项功能添加日志！！！！！！！！
- 目前TinyWeb只是监控网络IO，也就是说，网络事件是异步的，而其中每一项应答的处理是阻塞的，每个连接占用一定的资源。
- 把分析网络请求的过程也弄成defer化，不只是单纯的
- defer
    - 如何处理参数不确定的问题
    - 实现异步编程
- processpoll和process通过底层的信号控制
- 进程间通信用信号和共享内存
    - 信号控制进程的工作，停止
    - 共享内存更偏重于业务逻辑
- 思考：进程之间通信能不能用现有的Connection
- fork 之后复制的内存如何销毁的问题！！！！！！！！
- 优化创建进程的方式：通过vfork和exec
- master和worker实际上是对processpoll和process的高层封装
    - 通过对他们的信号进行控制，进而实现对master对worker的控制，
        - 如：平滑启动，强制关闭所有进程，重启的一系列高层决策，
        - processpoll只负责接受命令，分发信号，处理信号

> # 在宏观上需要改进的意向

- defer
- 进程池机制（平滑启动）
- memorypool
- cache
- proxyer
- 负载均衡

> # 模块的开发文档


各种类模块的说明：
- distingush Protocol & Connection
    - Protocol 用来处理用户逻辑，简单的维护用户上下文，不负责数据传输，
        通过简单的重载basic Protocol的共有函数来实现处理Connection读到
        Buffer中的数据。
    - Connection 用来与底层的IO-Multiplexing 进行交互，负责数据的传输，
        维护者数据缓冲，监听着各种连接事件
    - 简单来说：Connection负责数据传输，不处理用户逻辑；Protocol不处理数据传输，
     负责处理逻辑



> # test目录下相关测试的说明

> # multiwebserver目录下web服务器各种版本的说明。
|目录|版本|
|-|-|
||单进程+阻塞IO|
||多进程+阻塞IO|
||单进程多线程+阻塞IO|
||单进程+epoll+非阻塞IO|
||进程池+epoll+非阻塞IO|
||单进程多线程+epoll+非阻塞IO|


> # 用户可重用或可更改部分的操作方法

> ## 1.Configer的使用方法

- 获得Configer实例（配置项还未加载，需调用loadConfig()）
```
Configer &getConfigerInstance(const std::string file = "")
```

- 调整confige文件
```
setConfigerFile(const std::string &file)
```

- 装载配置文件（不需调用，setConfigerFile会装载配置文件）
```
loadConfig()
```

- 获取配置项数值
```
std::string getConfigValue(const std::string &);
```

- 注意

这样会出错：
```
Configer configer = Configer::getConfigerInstance();
```

必须用引用接受：
```
Configer &configer = Configer::getConfigerInstance();
```

推荐用法 1：
```
std::string a = "../TinyWeb.conf";
setConfigerFile(a);
if (loadConfig())
    std::cout << "load config successfull\n";
else
    std::cout << "load config failed\n";
Configer::getConfigerInstance().test();
cout << getConfigValue("loglevel");
```

使用方法 2：
```
Configer &configer = Configer::getConfigerInstance();
configer.setConfigerFile("../TinyWeb.conf");
if (configer.loadConfig())
    std::cout << "load config successfull\n";
else
    std::cout << "load config failed\n";
configer.test();
cout << configer.getConfigValue("loglevel");
```

```

```


> ## 2. Protocol 使用方法

- 如果想要自定义协议类，只需要继承Protocol,并重载相关函数（见WebProtocol定义）


--------------


# 参考文献：

1.APUE

2.UNP卷1

3.UNP卷2

4.C++ Primer

5.[Linux 多线程服务端编程：使用 muduo C++ 网络库](https://github.com/chenshuo/documents)

6.[C++ 官方参考手册](http://en.cppreference.com/w/cpp)

7.[高性能服务器编程](http://blog.csdn.net/column/details/high-perf-network.html)
