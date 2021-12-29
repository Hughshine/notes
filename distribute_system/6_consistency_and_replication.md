# 复制与一致性

复制：保证可靠性（避免单点故障）与性能（数量上的可拓展性）；但需要维持复制透明性，一些一致性保证

> 处理stale copy：
>
> 1. 服务端去处理，它可以移动copy到更近的位置 （需要用户与某个replica是近的）
> 2. 服务端去invalidate本地（影响服务端的性能）
>
> 会占用带宽；如果更新很多，访问很少，cache就很难起作用了。

我们需要scalablity，但是维护replicas需要处理consistency问题，这可能会引入更多问题。实际，会降低对consistency的限制。这种限制往往是应用相关的。incosistency的三个维度（数值差异，时间差异，更新顺序差异）

## 一致性

一致性模型：讨论如何描述与推理全局状态的一致性（对共享状态的访问，进程可以期待什么）

> client-centric consistency models是针对特定场景的简化；
>
> 讨论具体的consistency；讨论consistency的实现（replicas；how to keep consistent）

数据为中心的一致性：strict/sequential consistency, linearizability

client为中心的一致性：eventual consistency

要考虑的主要问题：update propagation method

复制协议（replication protocols）：采取某种更新传播方法以维护一致性的算法

### strict consistency

read只能读到最近的写

依赖绝对的全局时间

### sequential consistency

进程的读写如何interleave都没关系（单线程操作的effect有顺序），但要求所有的进程都看见相同的interleaving（并不讨论时间）

### linearizability

相较SC，额外有一个timestamp的要求，就是real time先发生的，effect也要先发生。这个引入的同步非常昂贵，常常只用于程序的形式化验证。

### casual consistency

只关注写：可能有因果关系的写（借助读建立起来；类似假设所有的写都具有rel，所有的读都具有acq语义），必须被所有进程按顺序的看到；其他写无所谓

### FIFO consistency

每个进程看到其他进程的写的projection，都是其program order（issue order），但不限制不同进程看到相同的交叉
