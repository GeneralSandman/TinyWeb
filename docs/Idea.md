
> # 1. 各种成熟与不成熟想法
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
- Semphore,SharedMemory,Cache
- 思考mime.type的设计，如何配置 include /home/li/TinyWeb/mime.type
- 为Connection添加close()功能来作为对shutdownWrite()的补充
- 如何支持中文url??????
- 如何使用进程池来处理多个连接
    - master进程负责EventLoop.loop()循环，负责分配连接
    - slaver进程监听与master的socket，接受请求，工作
    - 一个Connection只能属于一个进程，不能多个进程同时处理
    - Connection的读，写，关闭事件均有master进程负责，
        并把相应的事件传给slaver
- 每个Process均有一个EventLoop，不过监听的event不同
    - master监听网络事件
    - slaver监听master的通信事件，和每个Connection的定时器事件
- 进程间通信用信号和共享内存
    - 信号控制进程的工作，停止
    - 共享内存更偏重于业务逻辑
- master和worker实际上是对processpoll和process的高层封装
    - 通过对他们的信号进行控制，进而实现对master对worker的控制，
        - 如：平滑启动，强制关闭所有进程，重启的一系列高层决策，
        - processpoll只负责接受命令，分发信号，处理信号
- Nginx封装的锁都不会直接使用信号量,因为一旦获取信号量互斥锁失败,进程
就会进入睡眠状态,这会导致其他请求“饿死”。
- master进程不需要处理网络事件,它不负责业务的执行,只会通过管理worker等子进程
来实现重启服务、平滑升级、更换日志文件、配置文件实时生效等功能。
- 为Factory添加writeCompleteCallback
- 因为listen-Socket是可以复用的，而已经创建起连接的Socket是不可复用的，下一个新连接必定要使用新的connect-Socket,他们决定了Accepter和Connector在细节上略有不同。
- 三种工作状态：Server和Proxy
- 为Protocol添加makeConnection()功能，作为客户端使用
- Connector只监听读事件，错误事件
- Connection监听读、写、写完、错误、关闭事件
- Connection和Connector的Channel不同，也就决定了监听对象的不同，Connector监听的是连接建立的时候的事件，而Connection要对连接成功之后的事件进行负责。注意事件范围。
- ConnectSocket 和已经建立连接后的Connection中的socket是相同的
如何处理两者都关闭file descriptor 出错的问题。
连接池的概念...
从池中获取有效连接时有三种情况：
  1. 连接池中存在有效链接时，从连接池获取；
  2. 当连接数不超过最大连接数时候，尝试建立新连接；
  3. 连接数达到最大，放入等待队列等候处理。
  4. 用户在使用完该连接后需主动将连接归还给连接池，在归还连接时， 该连接有可能已经失效。
  5. 连接池对象一般作为一个全局性的对象或者在需要地方将其传入。 

> # 在宏观上需要改进的意向

- 进程池机制（平滑启动）
- cache
- proxyer
- 负载均衡
- 进程间的消息传递
    - 信号   1 
    - 共享内存  1
    - socketpair    1
- 进程间的同步
    - 原子操作（基于原子变量实现的自旋锁）1
    - 信号量   (会导致睡眠) 实现互斥量 
    - 文件锁   (会导致睡眠)
    - 基于以上三种实现mutex

```
不要随意地使用信号量互斥锁,这会使得
worker进程在得不到锁时进入睡眠状态,从而导致这个worker进程上的其他请求被“饿死”。
鉴于此,Nginx使用原子操作、信号量和文件锁实现了一套ngx_shmtx_t互斥锁,当操作系统
支持原子操作时ngx_shmtx_t就由原子变量实现,否则将由文件锁来实现。顾名思义,
ngx_shmtx_t锁是可以在共享内存上使用的,它是Nginx中最常见的锁。
```

- Nginx各进程间共享数据的主要方式就是使用共享内存(在使用共享内存时,Nginx一般是由master进程创建,在master进程fork出worker子进程后,所有的进程开始使用这块内存中
的数据)。

- 这个消息的格式似乎过于简单了,没错,因为Nginx仅用这个频道同步master进程与worker进程间的状态,这点从针对command成员已经定义的命令就可以看出来,如下所示。
- master又
是如何启动、停止worker子进程的呢?正是通过socketpair产生的套接字发送命令的,即每次要派生一个子进程之前,都会先调用socketpair方法。
- 接收频道消息的套接字添加到epoll中
- 使用信号量作为互斥锁有可能导致进程
睡眠,因此,要谨慎使用,特别是对于Nginx这种每一个进程同时处理着数以万计请求的服
务器来说,这种导致睡眠的操作将有可能造成性能大幅降低。
- 信号量提供的用法非常多,但Nginx仅把它作为简单的互斥锁来使用
- 但Nginx封装的文件锁仅用于保护代码段的顺序执行(例如,在进行负
载均衡时,使用互斥锁保证同一时刻仅有一个worker进程可以处理新的TCP连接),使用方
式要简单得多:一个lock_file文件对应一个全局互斥锁,而且它对master进程或者worker进程
都生效。因此,对于l_start、l_len、l_pid,都填为0,而l_whence则填为SEEK_SET,只需要
这个文件提供一个锁。l_type的值则取决于用户是想实现阻塞睡眠锁还是想实现非阻塞不会
睡眠的锁。


各种类模块的说明：
- Protocol & Connection
    - Protocol 用来处理用户逻辑，简单的维护用户上下文，不负责数据传输，
        通过简单的重载basic Protocol的共有函数来实现处理Connection读到
        Buffer中的数据。
    - Connection 用来与底层的IO-Multiplexing 进行交互，负责数据的传输，
        维护者数据缓冲，监听着各种连接事件
    - 简单来说：Connection负责数据传输，不处理用户逻辑；Protocol不处理数据传输，
     负责处理逻辑
- EventLoop 管理SignalManager，在loop()中根据信号处理函数设置的全局变量去进行状态控制。

- struct chain_t判断是最后一个chain的办法
    - 下一阶段要append(data) 所在的buffer_t 中```islast == true```
    - 



先读取文件的stat
1. 路径不存在 ->　返回特殊页面
2. 路径存在： 是目录 -> 通过配置将路径补全为index页面
            是正常文件 -> 读取


sendfile 和 connection 中outputBuffer的逻辑交互如何处理


将core中buffer的设计更改为 new-memorypool中buffer的设计，buffer 中有数据结构表示是否文件还是内存中的数据。


    proxy_connect_timeout 10;
    proxy_read_timeout 180;
    proxy_send_timeout 5;
    proxy_buffer_size 16k;
    proxy_buffers 4 32k;
    proxy_busy_buffers_size 96k;
    proxy_temp_file_write_size 96k;
    proxy_temp_path /tmp/temp_dir;
    proxy_cache_path /tmp/cache levels=1:2 keys_zone=cache_one:100m inactive=1d max_size=10g;

proxy_connect_timeout 服务器连接的超时时间
proxy_read_timeout 连接成功后,等候后端服务器响应时间
proxy_send_timeout 后端服务器数据回传时间
proxy_buffer_size 缓冲区的大小
proxy_buffers 每个连接设置缓冲区的数量为number，每块缓冲区的大小为size
proxy_busy_buffers_size 开启缓冲响应的功能以后，在没有读到全部响应的情况下，写缓冲到达一定大小时，nginx一定会向客户端发送响应，直到缓冲小于此值。
proxy_temp_file_write_size 设置nginx每次写数据到临时文件的size(大小)限制
proxy_temp_path 从后端服务器接收的临时文件的存放路径
proxy_cache_path 设置缓存的路径和其他参数。被缓存的数据如果在inactive参数（当前为1天）指定的时间内未被访问，就会被从缓存中移除


        location ~ .*\.(gif|jpg|png|css|js)(.*) {
                proxy_pass http://ip地址:90;
                proxy_redirect off;
                proxy_set_header Host $host;
                proxy_cache cache_one;
                proxy_cache_valid 200 302 24h;
                proxy_cache_valid 301 30d;
                proxy_cache_valid any 5m;
                expires 90d;
                add_header wall  "hey!guys!give me a star.";
        }

proxy_pass nginx缓存里拿不到资源，向该地址转发请求，拿到新的资源，并进行缓存
proxy_redirect 设置后端服务器“Location”响应头和“Refresh”响应头的替换文本
proxy_set_header 允许重新定义或者添加发往后端服务器的请求头
proxy_cache 指定用于页面缓存的共享内存，对应http层设置的keys_zone
proxy_cache_valid 为不同的响应状态码设置不同的缓存时间
expires 缓存时间



缓存系统设计
