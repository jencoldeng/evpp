evpp 
---


<a href="https://github.com/Qihoo360/evpp/releases"><img src="https://img.shields.io/github/release/Qihoo360/evpp.svg" alt="Github release"></a>
<a href="https://travis-ci.org/Qihoo360/evpp"><img src="https://travis-ci.org/Qihoo360/evpp.svg?branch=master" alt="Build status"></a>
[![Platform](https://img.shields.io/badge/platform-%20%20%20%20Linux,%20BSD,%20OS%20X,%20Windows-green.svg?style=flat)](https://github.com/Qihoo360/evpp)
[![License](https://img.shields.io/badge/license-%20%20BSD%203%20clause-yellow.svg?style=flat)](LICENSE)
[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](http://www.repostatus.org/badges/latest/active.svg)](http://www.repostatus.org/#active)

# 项目简介

从[Qihoo360/evpp](https://github.com/Qihoo360/evpp)fork 出来的项目，感谢Qihoo360/evpp团队，evpp是一个优秀的项目。

# 主要修改几个用得不太习惯的地方：

1.日志回调函数：evpp本身的日志，可以在CMake文件中配置绑定到glog，但是我的程序里并不适用glog，所以我修改了这部分，可以让用户指定自己的日志输出函数。见[evpp/logger.h](evpp/logger.h)，调用方可以调用`set_log_handler()`或者`set_log_stdout()`把日志输出到控制台(调试)或者输出到自己定义的回调函数.

2.HTTP的初始化：`Server::Init()`函数限定一定要监听"0.0.0.0"，这在很多情况下都不能这样做，只能绑定某个IP，于是我改了一下接口：
```
    bool Init(const std::string& ip_port);//新增
    bool Init(const std::string& ip, int listen_port); //新增需要用户指定IP
    bool Init(const std::string& ip, const std::vector<int>& listen_ports); //新增需要用户指定IP
    bool Init(const std::string& ip, const std::string& listen_ports/*like "80,8080,443"*/); //新增需要用户指定IP
```

3.上面修改的`Server::Init()`函数与测试用例可能不兼容，请在CMakeList中注释掉测试用例和example
```
#if (NOT EVPP_VCPKG_BUILD)
#    add_subdirectory (test)
#    add_subdirectory (examples)
#    add_subdirectory (benchmark)
#endif ()
```

4.修正了几个地方缺少vector的include语句

# Introduction  [中文说明](readme_cn.md)

[evpp] is a modern C++ network library for developing high performance network services in TCP/UDP/HTTP protocols.
[evpp] provides a TCP Server to support multi-threaded nonblocking event-drive server and also a HTTP, UDP Server to support http and udp prococol.

# Features

1. Modern C++11 interface
1. Modern functional/bind style callback instead of C-style function pointer.
1. Multi-core friendly and thread-safe
1. A nonblocking multi-threaded TCP server
1. A nonblocking TCP client
1. A nonblocking multi-threaded HTTP server based on the buildin http server of libevent
1. A nonblocking HTTP client
1. A nonblocking multi-threaded UDP server
1. Async DNS resolving
1. EventLoop/ThreadPool/Timer
2. Well tested — [evpp] is well tested with unit tests and stress tested daily in production. It has been used in production and processes 1000+ billions networking communications every day in our production
3. Easy install — [evpp] can be packaged as a deb, rpm, tar.gz with a single command for straight forward distribution and integration

And also provides some libraries based on [evpp]:

1. [evmc] a nonblocking async C++ memcached (or membase cluster) client library. This library is currently used in production which sends more than 300 billions requests every day. See [evmc readme](/apps/evmc/readme.md) for more details.
2. [evnsq] a nonblocking async C++ NSQ client library. This library is currently used in production which processes more than 130 billions messages every day. See [evnsq readme](/apps/evnsq/readme.md) for more details.

NOTE: master is our development branch and may not be stable at all times.

# Origin

In our business system, we need to build a TCP long-connection Gateway and other TCP services. When we do a survey of the open sources, we cannot find any appropriate network library for our demands. According to our own business characteristic, an ideal C++ network library must have the features below: 

1. A simple enough C++ interface
2. Support multi-threads and multi-processes
3. Based on [libevent] is preferable for our projects. Given your older applications were based on [libevent], it was preferable to have your new framework also be based on it, so as to reduce the overall time/effort/cost to completion. Actually, we do have some older applications which were based on [libevent].

As described above, there are not many options to choose from. So we developed one ourself. The design of the interface is highly inspired by [muduo] and [Golang]. Let's take some examples to exaplain this: 

1. `Duration` : This is a time inteval class, with a time unit. It is referenced to the implementation of `Duration` of the [Golang] project. We have seen some many cases that the time interval without a unit. For example, what does `timeout` mean?  Seconds, milliseconds or microseconds? We need to read the document carefully, even more, we need to read the implementation codes. Our `Duration` class has self-explations with the time unit. Additionally `std::chrono::duration` in the STL of C++11 has the similar implementations, but it is a little bit complicated.
2. `Buffer` : This is a memory buffer class. It uses the advantages of the two projects [muduo] and [Golang].
3. `http::Server` : This is a HTTP server class with a working threads pool. It is thread-safe to dispatch tasks
4. We simply use a string with the format of `"ip:port"` to represent a network address. This is referenced to the design of [Golang].
5. `httpc::ConnPool` : This is HTTP client connection pool with highly performance. In the future we can add more features to this class : load balance and failover.

In addition, in the implematations we pay seriously attations to thread-safe problems. An event-related resource must be initialized and released in its own `EventLoop` thread, so that we can minimize the possibility of thread-safe error. In order to achieve this goal we reimplemented `event_add` and` event_del` and other functions. Each call to `event_add`, we stored the resource into thread local storage, and in the call `event_del`, we checked it whether it is stored in the thread local storage. And then we checked all the threads local storages to see whether there are resources not destructively released when the process was exiting. The detail codes are here [https://github.com/Qihoo360/evpp/blob/master/evpp/inner_pre.cc#L36~L87](https://github.com/Qihoo360/evpp/blob/master/evpp/inner_pre.cc#L36~L87). We are so harshly pursuit the thread safety to make a program can quietly exit or reload, because we have a deep understanding of "developing a system to run forever and developing a system to quietly exit after running a period of time are totally two different things".


# Getting Started

Please see [Quick Start](docs/quick_start.md)

# Benchmark

### Benchmark Reports

[The IO Event performance benchmark against Boost.Asio](docs/benchmark_ioevent_performance_vs_asio.md) : [evpp] is higher than [asio] about **20%~50%** in this case

[The ping-pong benchmark against Boost.Asio](docs/benchmark_ping_pong_spend_time_vs_asio.md) : [evpp] is higher than [asio] about **5%~20%** in this case

[The throughput benchmark against libevent2](docs/benchmark_throughput_vs_libevent.md) : [evpp] is higher than [libevent] about **17%~130%** in this case 

[The throughput benchmark against Boost.Asio](docs/benchmark_throughput_vs_asio.md) : [evpp] and [asio] have the similar performance in this case

[The throughput benchmark against Boost.Asio(中文)](docs/benchmark_throughput_vs_asio_cn.md) : [evpp] and [asio] have the similar performance in this case

[The throughput benchmark against muduo(中文)](docs/benchmark_throughput_vs_muduo_cn.md) : [evpp] and [muduo] have the similar performance in this case

### Throughput

The throughput benchmark of [evpp] is **17%~130%** higher than [libevent2] and similar with [boost.asio] and [muduo].
Although [evpp] is based on [libevent], [evpp] has a better throughput benchmark than [libevent]. That's because [evpp] implements its own IO buffer instead of [libevent]'s evbuffer. 

![](https://raw.githubusercontent.com/zieckey/resources/master/evpp/benchmark/throughput/evpp-vs-libevent-1thread-all.png)
![](https://raw.githubusercontent.com/zieckey/resources/master/evpp/benchmark/throughput/evpp-vs-asio-1thread-all.png)

# Examples

## An echo TCP server

```cpp
#include <evpp/tcp_server.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>

int main(int argc, char* argv[]) {
    std::string addr = "0.0.0.0:9099";
    int thread_num = 4;
    evpp::EventLoop loop;
    evpp::TCPServer server(&loop, addr, "TCPEchoServer", thread_num);
    server.SetMessageCallback([](const evpp::TCPConnPtr& conn,
                                 evpp::Buffer* msg,
                                 evpp::Timestamp ts) {
        conn->Send(msg);
    });
    server.SetConnectionCallback([](const evpp::TCPConnPtr& conn) {
        if (conn->IsConnected()) {
            LOG_INFO << "A new connection from " << conn->remote_addr();
        } else {
            LOG_INFO << "Lost the connection from " << conn->remote_addr();
        }
    });
    server.Init();
    server.Start();
    loop.Run();
    return 0;
}
```

### An echo HTTP server

```cpp
#include <evpp/http/http_server.h>

int main(int argc, char* argv[]) {
    std::vector<int> ports = { 9009, 23456, 23457 };
    int thread_num = 2;
    evpp::http::Server server(thread_num);
    server.RegisterHandler("/echo",
                           [](evpp::EventLoop* loop,
                              const evpp::http::ContextPtr& ctx,
                              const evpp::http::HTTPSendResponseCallback& cb) {
        cb(ctx->body().ToString()); }
    );
    server.Init(ports);
    server.Start();
    while (!server.IsStopped()) {
        usleep(1);
    }
    return 0;
}

```


### An echo UDP server

```cpp
#include <evpp/udp/udp_server.h>
#include <evpp/udp/udp_message.h>

int main(int argc, char* argv[]) {
    std::vector<int> ports = { 1053, 5353 };
    evpp::udp::Server server;
    server.SetMessageHandler([](evpp::EventLoop* loop, evpp::udp::MessagePtr& msg) {
        evpp::udp::SendMessage(msg);
    });
    server.Init(ports);
    server.Start();

    while (!server.IsStopped()) {
        usleep(1);
    }
    return 0;
}
```

### More examples

Please see the source code in [`examples`](https://github.com/Qihoo360/evpp/tree/master/examples).

# TODO

1. An async redis client
2. Add `zipkin` tracing support
3. Add examples : asio chat room
4. Fix the comments written in mandarin problem
5. Add benchmark against with boost.asio/cpp-netlib/beast http library/muduo/libevent/libuv ... 

# In Production

<img src="http://i.imgur.com/dcHpCm4.png" height = "100" width = "120" alt="Qihoo">

> Welcome to join this list :-)


# Thanks

Thanks for the support of [Qihoo360](http://www.360.cn "http://www.360.cn").

Thanks for [libevent], [glog], [gtest], [Golang], [LevelDB], [rapidjson] projects.

[evpp] is highly inspired by [muduo]. Thanks [Chen Shuo](https://github.com/chenshuo "https://github.com/chenshuo")


[gtest]:https://github.com/google/googletest
[glog]:https://github.com/google/glog
[Golang]:https://golang.org
[muduo]:https://github.com/chenshuo/muduo
[libevent]:https://github.com/libevent/libevent
[libevent2]:https://github.com/libevent/libevent
[LevelDB]:https://github.com/google/leveldb
[rapidjson]:https://github.com/miloyip/rapidjson
[Boost.Asio]:http://www.boost.org/
[boost.asio]:http://www.boost.org/
[asio]:http://www.boost.org/
[boost]:http://www.boost.org/
[evpp]:https://github.com/Qihoo360/evpp
[evmc]:https://github.com/Qihoo360/evpp/tree/master/apps/evmc
[evnsq]:https://github.com/Qihoo360/evpp/tree/master/apps/evnsq