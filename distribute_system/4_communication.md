## 通信类型

持久与瞬时通信（非持久），异步与同步

* 通信的三个同步时刻：request submission/delivery/after processing.
* 通信机制本身，可以有storage facility：（是否选择这种实现，也和要实现的通信问题有关）
  * 瞬时通信的发送者与接收者必须同时存在，中途退出会导致传输失败（Client/Server模型属于此类，Client在等待回复是会阻塞（当然可以异步，但这不是协议本身做到的），Server一般只等待通信，不会主动通信）；
    * 错误必须被良好处理，因为client在等待；
      * 持久通信+异步通信：queued messages，sender不需要等待及时的回复，可以做其他事
  * 持久通信的例子是电子邮件，发送者与接收者不必同时在，有一个信息的存储方（当然，也因而不能保证被阅读）。


## RPC

stub？

PRC建立在谁之上？

使用的信息是谁的？有什么特别的吗？应对什么问题？如何处理引用相关？被调用的函数放在哪儿？引用作为参数有意义吗？

故障处理：五类可能的故障

1. client 找不到 server
   1. 使 procedure 有一个特殊的返回值标识改错误发生
   2. 异常
2. client 到 server 的 request msg 丢失
   1. timer for client, 超时重发 => 丢失了许多消息，server down
      1. 每个消息都会有sequence number，使server可以区分retrans msg，避免重复执行（尤其是需要避免不幂等的过程调用）
      2. TCP connection 先断？重新发送消息（without increment the request sequence number）。只需要server保持旧回复一段时间。
3. server 到 client 的 reply msg 丢失
   1. timer for server: retransmit
4. server 在收到 request msg 后 crash（看起来主要是client在做一些事？我觉得这里ppt上说的很不清楚，TODO）
   1. 需要确定过程调用是否完成了（crash发生在PC之前还是之后）
      1. 保证服务完成至少一次：等待服务器启动、重发请求
      2. 保证服务完成至多一次：立刻结束，报告failure
         1. server也可以持久性存储所有的replies，重启后resend
      3. 不做任何保证：什么也不做（？）
5. client 在发出 request msg 后 crash：孤儿计算（active computation whose result is not waited），4个处理方案
   1. 灭绝 extermination：日志记录，重启后看日志，清除遗留孤儿
   2. 轮回 reincarnation：重启时广播“新的epoch”，收到广播，则终止全部远程计算
   3. 温柔轮回 gentle reincarnation：收到广播，每个机器会寻找远程计算的所有者，仅在找不到时结束计算（owner应该和epoch标记配合？）
   4. 期限 expiration：赋予RPC时间配额，未完成则主动申请

Interface Definition languagem 用以明确 interface（数据类型和过程的接口）=> 生成cleint/server stub

* msg is represented machine independently - serialization
* 调用与被调用过程处于不同的地址空间
  * 不能发送指针/系统相关信息
  * 参数传递很昂贵！
  * 会破坏透明性（TODO）

## 异步RPC

前述RPC会等待调用结束。也有不等待的版本。

1. server首先返回一个accept request（client只需要wait for acceptance），在运算后打断client
2. one-way RPC，client不去wait for acceptance

## 动态绑定：client如何locate server?

hardwire: fast but inflexible

Dynamic binding: registering, call a binder (也有deregister过程)

1. servers 提供给binder：name, version number, uid, a handle used to locate(system dependent)
2. server也可以deregister某个服务：name，version，uid
3. 第一次调用某个远程过程时，看到还没有绑定到服务器（client stub），于是申请向binder请求导入version X interface
4. 绑定服务器会检查是否已经有若干个具有该version X name N interface
   1. 如果不存在，调用失败
   2. 存在，则绑定器返回其handle和uid给client stub
5. client stub使用该句柄作为发送请求消息的地址，消息包含参数和uid，uid是给内核用来定向的.

优势：

1. 灵活性
2. multiple server

   1. 可以自然地负载均衡
   2. 提供周期性地轮询，自动取消注册失败的服务器，达到一定容错
   3. 可以辅助auth
3. binder 可以去验证client 和 server的version

缺点：

1. 会引入一定的overhead
2. binder本身可能会成为大型分布式系统的性能瓶颈
