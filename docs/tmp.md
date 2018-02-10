> ### 一、技术要点

- Reactor模式：
    Reactor模式主要通过IO-Multiplexing(IO复用技术来实现)，具体来说就是epoll方法，为什么要选择epoll方法，[戳这里](http://blog.csdn.net/russell_tao/article/details/17119729)
- 多进程模式：
    默认配置下TinyWeb将会有一个Master进程，8个IO进程。
    多进程采用linux下Semaphore（信号量）和Shared Memory（共享内存）实现。
- Master进程负责监听连接，并分配给空闲的worker进程，IO进程负责接受链接。在这里，我把http想象为短连接，服务器处理网用户请求之后，就关闭连接，所以一个worker进程不仅仅处理一个连接
- 如何优化TinyWeb的速度，如何借助内存缓冲技术，这是我的知识的真空区



> ### 三、开发环境

1.Ubuntu 14.04

2.vscode & g++ & gdb

3.Chrome

4.git



![](http://oj8qdsa91.bkt.clouddn.com/steelsea.jpg)





一直想用C++做个项目，因为比较喜欢网络通信这部分，又想让事情变得更有些挑战性，我最终选择实现一个高性能的Web服务器。

TinyWeb代码地址:
https://github.com/GeneralSandman/TinyWeb

一、在宏观上我希望TinyWeb能够达到这样的目标:
1. 配置简单
2. 扩展容易
3. 性能高效
4. 功能强大

二、TinyWeb 目前的功能是：
1. 基本的Web服务，只能提供静态网页服务。
2. 单进程＋Reactor+非阻塞方式运行
3. 只能用于Linux系统，不专注跨平台
4. 基本的Web服务配置

三、TinyWeb 目前的发展计划：
1. 使用进程池的方式提高并发性
2. 强有力的支持多种版本Http协议
3. 负载均衡功能
4. 代理功能

四、并希望在开发的过程中能够掌握以下知识:
1. Unix环境编程的技能
2. Unix网络编程的技能
3. 巩固C++11知识
4. 掌握构建大型C++项目的技能
5. 掌握多进程、多线程同步、通信机制
6. 实现高性能可复用Memory-pool
7. 掌握基本的数据结构（红黑树、链表、Hash）
8. 能够把通用的网络部分作为单独的网络库、为以后网络开发提升效率