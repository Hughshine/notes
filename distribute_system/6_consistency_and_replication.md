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

## 同步操作与一致性

Weak consistency 在同步（sync，应该就是sc fence）完成后，令共享数据保持一致；
1. 同步变量（fence variable）是SC的
2. 所有前面的写都完成了，才可以进行同步操作
3. 同步操作完成了，才可以进行后面的读或写

Release consistency: 退出临界区时，保证共享数据一致。acq/rel lock: 进入/退出临界区。
1. （该进程的）acq成功了，才能对共享数据操作
2. 在进行release时，所有读写都已完成
3. 对于同步变量的访问，FIFO consistent（Not SC）。

Entry consistency: 进入临界区时，共享数据一致
1. 在acq时，同步该锁保护的变量进来（但不保证全局的同步）

## client-centural

eventual consistency: 用户因为移动访问不同的replicas；如果不更新，副本应逐渐一致。

单调读：保证读的数据只会越来越新
单调写：写操作顺序完成
写后读：前面的写，后面的读可以看到（保证读最新）
读后写：后面的写，一定会覆盖前面的某个读

（几个图看懂）纵轴是local store，不同进程在不同local store上有一定顺序的读写，并会进行一定的同步，要判断的是，这些同步，是否保证得了单个进程的读写的特定性质。

## Quorum-based protocols

读团大小，写团大小：加和必须>N，以保证能取到一个新的写。写团必须大于一半，以保证不会同时写。
