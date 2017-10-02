# TinyWeb
```
 _____ _           __        __   _     
|_   _(_)_ __  _   \ \      / /__| |__  
  | | | | '_ \| | | \ \ /\ / / _ \ '_ \ 
  | | | | | | | |_| |\ V  V /  __/ |_) |
  |_| |_|_| |_|\__, | \_/\_/ \___|_.__/ 
               |___/                    
```
## demon测试
| version | type | path |
|--------|--------|--------|
|  01 | 阻塞IO+有限状态机|test_server/1_0|
|02|多进程阻塞IO|test_server/1_1|
|03|多线程阻塞IO|test_server/1_2|
|04|epoll复用|test_server/2_0|
|05|epoll进程池|test_server/2_1|
|06|epoll线程池|test_server/2_2|
|07|libevent式|test_server/3_0|


```
                           _                 
 _ __ ___     __ _   ___  | |_    ___   _ __ 
| '_ ` _ \   / _` | / __| | __|  / _ \ | '__|
| | | | | | | (_| | \__ \ | |_  |  __/ | |   
|_| |_| |_|  \__,_| |___/  \__|  \___| |_|   

```
## diamond TinyWeb
/master/master.cc
                                
| version | type |
|--------|-------|
|1.0|线程池+epoll|
|1.1|添加信号处理|
|1.2|添加有限状态机|
|1.3|优化url解析|
|2.0|添加定时器|
|2.1|+链表定时器管理|
|2.2|+时间轮定时器管理|
|2.3|+最小堆定时器管理|
|3.0|+


> # 一、TinyWeb配置方法










国庆学习计划
- 入门negix
- 完成muduo的相关功能
- 实现进程池
- 实现红黑树，set集合
- 



> 1.如何进行测试

server_test 监听80端口，600s后终止程序
```
make lib
make server_test
sudo ./server_test
```

client.py 作为客户端连接
```
python client.py 9090 139.199.13.50 80
```



