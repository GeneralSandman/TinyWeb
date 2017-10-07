# TinyWeb

```
 _____ _           __        __   _     
|_   _(_)_ __  _   \ \      / /__| |__  
  | | | | '_ \| | | \ \ /\ / / _ \ '_ \ 
  | | | | | | | |_| |\ V  V /  __/ |_) |
  |_| |_|_| |_|\__, | \_/\_/ \___|_.__/ 
               |___/                    
```



```
                           _                 
 _ __ ___     __ _   ___  | |_    ___   _ __ 
| '_ ` _ \   / _` | / __| | __|  / _ \ | '__|
| | | | | | | (_| | \__ \ | |_  |  __/ | |   
|_| |_| |_|  \__,_| |___/  \__|  \___| |_|   

```
## diamond TinyWeb
                                
| version | type |
|--------|-------|



> # 一、TinyWeb配置方法
名称不区分大小写，数值区分;#开头为注释行
|名称|含义|
|-|-|
|listen|监听端口|
|processpoll|进程池数量|
|Docs|html路径|
|HostName|网址|
|***日志配置***|
|LogLevel|日志等级|
|LogPath|日志路径|
|DebugFile||
|InfoFile||
|WarnFile||
|ErrorFile||
|FatalFile||






升级计划
- 完成buffer类
- 提升程序为demon进程
- 检测80端口，提空http服务
- 添加protocol类，在此类中定义通信的格式，如何对信息做出回应
- Server类的constructor应该用protocol类作为参数，Server类从中获取回调函数并初始化相关信息
- Configer类升级为单例模式，以便于各个类的使用（类似与Logger）
- ./master -c 

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

> # 2.如何启动

- 直接启动，默认配置文件为```/TinyWeb.conf```，如果该配置文件不存在，将会创建此文件
```
./TinyWeb
```

- 启动时指定配置文件路径
```
./TinyWeb -c /home/li/TinyWeb.conf
```
