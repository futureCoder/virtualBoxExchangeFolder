# 即时通信软件 
## 一、实验介绍 
### 1.1 实验内容
使用C++实现一个具备服务器端和客户端的即时通信聊天室

## 二、实验步骤
### 2.1 需求分析
聊天室软件需要两个程序:
1.服务器: 能接收新的客户端连接请求,并将客户端发来的消息广播给其他所有的客户端.
2.客户端: 能连结服务器,并向服务器发送消息,同时要接收服务器发送过来的消息.
### 2.2 详细设计
客户端类需要支持如下功能:
1. 连接服务器
2. 接收用户输入的聊天消息,并发送给服务器
3. 接收并显示服务器发送过来的消息
4. 退出连接
针对这些需求,客户端需要两个进程分别支持如下功能:
子进程:
1. 等待用户输入聊天信息
2. 将聊天信息写入管道(pipe),并发送给父进程
父进程:
1. 将子进程发送的聊天信息从管道(pipe)中读取,并发送给服务器端
2. 使用epoll机制接收服务器发来的消息,并显示给用户,使用户可以看到其他用户的聊天信息

服务器类需要支持如下功能:
1. 支持多个客户端接入,实现聊天室的基本功能
2. 启动服务监听端口等待客户端连接
3. 使用epoll机制实现并发,增加效率
4. 客户端连接时,发送欢迎消息并存储连接记录
5. 将客户端发送过来的消息广播给其他所有客户端
6. 客户端退出连接时对连接信息进行清理

### 2.3 C/S模型
服务器端:
1. socket()创建监听Socket
2. bind()绑定服务器端口
3. listen()监听客户端连接
4. accept()接受连接
5. recv/send接收及发送数据
6. close()关闭socket

客户端:
1. socket()创建及监听socket
2. connect()连接服务器
3. recv/send接收及发送数据
4. close()关闭socket

#### 2.3.1 TCP服务端通信常规步骤
|步骤|接口|
|---|---|
|1. 使用socket()创建TCP套接字|socket|
|2. 将创建的套接字绑定到一个本地地址和端口上|Bind|
|3. 将套接字设为监听模式,准备接收客户端请求|listen|
|4. 等待客户请求到来:当请求到来后,接受连接请求,返回一个对应于此连接的新的套接字|accept|
|5. 用accept返回的套接字和客户端进行通信|使用write/Send或Send/Recv|
|6. 返回,等待另一个客户端请求||
|7. 关闭套接字||

#### 2.3.2 TCP客户端通信常规步骤
|步骤|接口|
|--|--|
|1. 创建套接字|socket|
|2. 使用connect建立到达服务器的连接|connect|
|3. 客户端进行通信|使用Write/Send或Send/Recv|
|4. 使用close关闭客户端连接|close|

### 2.4 基本技术介绍 
#### 2.4.1 阻塞与非阻塞socket 
通常,对一个文件描述符指定的文件或设备,有两种工作方式,阻塞与非阻塞方式.
1. 阻塞方式: 当试图对该文件描述符进行读写时,如果当时没有数据可读,或者暂时不可写,进程就进入等待状态,直到有东西可读或可写为止.
2. 非阻塞方式: 如果没有数据可读,或不可写,读写函数马上返回,而不会等待.
阻塞方式和非阻塞方式唯一的区别:是否立即返回.
非阻塞方式示例代码: 
``` fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK); ```

#### 2.4.2 epoll
epoll是Linux特有的I/O函数,是Linux内核为处理大批句柄而作了改进的poll,其特点如下:
1. epoll是Linux下多路复用I/O接口select/poll的增强版本.其实现和使用方式与select/poll有诸多不同,epoll通过一组函数来完成有关任务,而不是一个函数.
2. epoll之所以高效,是因为epoll将用户关心的文件描述符放在内核的一个事件表中,而不是像select/poll每次调用都需要重复传入文件描述符集或事件集.比如当一个时间发生(比如说读时间),epoll无须遍历整个被侦听的描述符集,只要遍历那些被内核IO事件异步唤醒而加入就绪队列的描述符集合就行了
3. epoll有两种工作方式,LT(level triggered):水平触发和ET(edge triggered):边沿触发.LT是select/poll使用的触发方式,比较低效;而ET是epoll的高速工作方式.
epoll共3个函数,如下:
创建一个epoll句柄: 
``` int epoll_create(int size) // size用来告诉内核监听的数目 ```
epoll事件注册函数:
```
/*
函数功能： epoll事件注册函数
参数epfd为epoll的句柄，即epoll_create返回值
参数op表示动作，用3个宏来表示：
EPOLL_CTL_ADD(注册新的fd到epfd)， 
EPOLL_CTL_MOD(修改已经注册的fd的监听事件)，
EPOLL_CTL_DEL(从epfd删除一个fd)；
其中参数fd为需要监听的标示符；
参数event告诉内核需要监听的事件，event的结构如下：
struct epoll_event {
  __uint32_t events; //Epoll events
  epoll_data_t data; //User data variable
};
其中介绍events是宏的集合，本项目主要使用EPOLLIN(表示对应的文件描述符可以读，即读事件发生)，其他宏类型，可以google之！
*/
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
```
等待事件产生:
```
// 等待事件的产生，函数返回需要处理的事件数目
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout)
```
因此服务端在使用epoll的时候,步骤如下:
1. 调用epoll_create函数在内核中创建一个事件表
2. 将文件描述符(监听套接字listener)添加到所创建的事件表中
3. 在主循环中,调用epoll_wait等待返回就绪的文件描述符集合
4. 分别处理就绪的事件集合,本项目只有两个事件:新用户连接事件和用户发来消息事件

将一个socket添加到事件表中的方法如下:
```
//将文件描述符fd添加到epollfd标示的内核事件表中， 并注册EPOLLIN和EPOOLET事件，EPOLLIN是数据可读事件；EPOOLET表明是ET工作方式。最后将文件描述符设置非阻塞方式
/**
  * @param epollfd: epoll句柄
  * @param fd: 文件描述符
  * @param enable_et : enable_et = true, 
     采用epoll的ET工 作方式；否则采用LT工作方式
**/
void addfd( int epollfd, int fd, bool enable_et )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if( enable_et )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
    printf("fd added to epoll!\n\n");
}
```
