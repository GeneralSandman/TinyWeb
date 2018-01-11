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
- 实现一个高效的内存池，为每个TCP连接维持内存池。参考STL实现。
    - 通过成员函数allocate和deallocate分配释放内存。
    - 当需要的内存大于128Bytes时：直接分配内存（第一级内存配置器）。
        - 直接调用malloc，free，
        - 处理内存不足的情况
    - 当小于128Bytes时：使用内存池分配配（第二级内存配置器）。
        - 16种内存大小，链表维护他们，
        - 如果内存不足时转向第一级配置器。
    - 要考虑内存不足时的处理，
    - 考虑造成的内存碎片
    - 效率

- 增加异步读磁盘
- nginx定义了结构体描述接受到信号的行为，定义一个数组注册收到信号的行为。
- 提升程序为demon进程（demon程序标准输入输出如何处理?）
- Semphore,SharedMemory,Cache
- 优化Parser
- 通过list维护一批请求头，响应头
- 如何处理url中包含```../```的问题
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
- 残留的Connection应该由Server释放
- 残留的Protocol应该由Factory释放
- Connection不知道Protocol的存在，Server不知道Factory的存在
- processpoll和process通过底层的信号控制
- 进程间通信用信号和共享内存
    - 信号控制进程的工作，停止
    - 共享内存更偏重于业务逻辑
- fork 之后复制的内存如何销毁的问题！！！！！！！！
- 优化创建进程的方式：通过vfork和exec
- master和worker实际上是对processpoll和process的高层封装
    - 通过对他们的信号进行控制，进而实现对master对worker的控制，
        - 如：平滑启动，强制关闭所有进程，重启的一系列高层决策，
        - processpoll只负责接受命令，分发信号，处理信号
- Nginx封装的锁都不会直接使用信号量,因为一旦获取信号量互斥锁失败,进程
就会进入睡眠状态,这会导致其他请求“饿死”。
- master进程不需要处理网络事件,它不负责业务的执行,只会通过管理worker等子进程
来实现重启服务、平滑升级、更换日志文件、配置文件实时生效等功能。
- 计划完成Client，Connector，Proxy,memorypool类。
- 重读代码，配置各个模块的日志结构。
- 为Factory添加writeCompleteCallback
- Accepter 只有一个，负责所有到来的连接
- Connector 与Accepter不同，只负责一个Connection，
- 因为listen-Socket是可以复用的，而已经创建起连接的Socket是不可复用的，下一个新连接必定要使用新的connect-Socket,他们决定了Accepter和Connector在细节上略有不同。
- 三种工作状态：Server和Proxy
- 为Protocol添加makeConnection()功能，作为客户端使用
- Connector只监听读事件，错误事件
- Connection监听读、写、写完、错误、关闭事件

- Connection和Connector的Channel不同，也就决定了监听对象的不同，Connector监听的是连接建立的时候的事件，而Connection要对连接成功之后的事件进行负责。注意事件范围。

### 按什么原则划分请求的阶段
1. 将阻塞进程的方法按照相关的触发事件分解为两个阶段
一个本身可能导致进程休眠的方法或系统调用,一般都能够分解为多个更小的方法或者
系统调用,这些调用间可以通过事件触发关联起来。大部分情况下,一个阻塞进程的方法调
用时可以划分为两个阶段:阻塞方法改为非阻塞方法调用,这个调用非阻塞方法并将进程归
还给事件分发器的阶段就是第一阶段;增加新的处理阶段(第二阶段)用于处理非阻塞方法
最终返回的结果,这里的结果返回事件就是第二阶段的触发事件。
例如,在使用send调用发送数据给用户时,如果使用阻塞socket句柄,那么send调用在向
操作系统内核发出数据包后就必须把当前进程休眠,直到成功发出数据才能“醒来”。这时的
send调用发送数据并等待结果。我们需要把send调用分解为两个阶段:发送且不等待结果阶
段、send结果返回阶段。因此,可以使用非阻塞socket句柄,这样调用send发送数据后,进程
是不会进入休眠的,这就是发送且不等待结果阶段;再把socket句柄加入到事件收集器中就
可以等待相应的事件触发下一个阶段,send发送的数据被对方收到后这个事件就会触发send
结果返回阶段。这个send调用就是请求的划分阶段点。
(2)将阻塞方法调用按照时间分解为多个阶段的方法调用
注意,系统中的事件收集、分发者并非可以处理任何事件。如果按照前一种方式试图划
分某个方法时,那么可能会发现找出的触发事件不能够被事件收集、分发器所处理,这时只
能按照执行时间来拆分这个方法了。
例如读取文件的调用(非异步I/O),如果我们读取10MB的文件,这些文件在磁盘中的块未必是连续的,这意味着当这10MB文件内容不在操作系统的缓存中时,可能需要多次驱
动硬盘寻址。在寻址过程中,进程多半会休眠或者等待。我们可能会希望像上文所说的那样
把读取文件调用分解成两个阶段:发送读取命令且不等待结果阶段、读取结果返回阶段。这
样当然很好,可惜的是,如果我们的事件收集、分发者不支持这么做,该怎么办?例如,在
Linux上Nginx的事件模块在没打开异步I/O时就不支持这种方法,像ngx_epoll_module模块主要
是针对网络事件的,而主机的磁盘事件目前还不支持(必须通过内核异步I/O)。这时,我
们可以这样来分解读取文件调用:把10MB均分成1000份,每次只读取10KB。这样,读取
10KB的时间就是可控的,意味着这个事件接收器占用进程的时间不会太久,整个系统可以
及时地处理其他请求。
那么,在读取0KB~10KB的阶段完成后,怎样进入10KB~20KB阶段呢?这有很多种方
式,如读取完10KB文件后,可能需要使用网络来发送它们,这时可以由网络事件来触发。
或者,如果没有网络事件,也可以设置一个简单的定时器,在某个时间点后再次调用下一个
阶段。
(3)在“无所事事”且必须等待系统的响应,从而导致进程空转时,使用定时器划分阶
段
有时阻塞的代码段可能是这样的:进行某个无阻塞的系统调用后,必须通过持续的检查
标志位来确定是否继续向下执行,当标志位没有获得满足时就循环地检查下去。这样的代码
段本身没有阻塞方法调用,可实际上是阻塞进程的。这时,应该使用定时器来代替循环检查
标志,这样定时器事件发生时就会先检查标志,如果标志位不满足,就立刻归还进程控制
权,同时继续加入期望的下一个定时器事件。
(4)如果阻塞方法完全无法继续划分,则必须使用独立的进程执行这个阻塞方法
如果某个方法调用时可能导致进程休眠,或者占用进程时间过长,可是又无法将该方法
分解为不阻塞的方法,那么这种情况是与事件驱动架构相违背的。通常是由于这个方法的实现者没有开放非阻塞接口所导致,这时必须通过产生新的进程或者指定某个非事件分发者进
程来执行阻塞方法,并在阻塞方法执行完毕时向事件收集、分发者进程发送事件通知继续执
行。因此,至少要拆分为两个阶段:阻塞方法执行前阶段、阻塞方法执行后阶段,而阻塞方
法的执行要使用单独的进程去调度,并在方法返回后发送事件通知。一旦出现上面这种设
计,我们必须审视这样的事件消费者是否足够合理,有没有必要用这种违反事件驱动架构的
方式来解决阻塞问题。
请求的多阶段异步处理将会提高网络性能、降低请求的时延,在与事件驱动架构配合工
作后,可以使得Web服务器同时处理十万甚至百万级别的并发连接,我们在开发Nginx模块时
必须遵循这一原则。


> # saaaaaaa
1. 性能
    - 网络性能：能否处理高并发连接
    - 单次请求延迟：在高并发的情况下维持较低的请求延迟
    - 网络效率：如长连接相比与短连接减少握手次数，提高网络效率
2. 可配置性
3. 可见性：某些关键组件可以被监控

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
