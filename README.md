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
- 提升程序为demon进程（demon程序标准输入输出如何处理?）
- 优化loggger，解决logger生命周期过短的问题
- client 如果没有足够的内存接受数据，connection会一直存在（不属于bug）
- Semphore
- SharedMemory

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

- 直接启动，默认配置文件为```/TinyWeb.conf```，该文件不存在，或格式错误时返回错误
```
sudo ./TinyWeb
```

- 启动时指定配置文件路径,该文件不存在，或格式错误时返回错误
```
sudo ./TinyWeb -c /home/li/TinyWeb.conf
```


------------------

> # 1.Configer的使用方法
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


> # 2. Protocol 使用方法

- 如果想要自定义协议类，只需要继承Protocol,并重载相关函数（见WebProtocol定义）

