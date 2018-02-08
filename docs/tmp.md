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

