# 0x01 并发算法与理论

Concurrency v.s. Parallel: 并行关注“将原问题”分解为“尽量独立的子问题”，并利用多个处理器同时求解，以得到原问题解的过程。并发关注“多个处理器面对共享内存资源时”，如何“同步多个处理器的计算”。两者有着很不同的关注点。

这门课关注各种“锁算法”更深入的理论基础，以及细粒度锁或无锁的并发数据结构。**关注并发算法的通用涉及思想和正确性原理**。

> 首先，软件愈来愈依赖并发。
> 
> 压榨每一点并发可能，很重要。
> 
> 并发同时带来正确性问题。【verification】

***

并发模型：多线程 + single shared memory (where objects live) + unpredictable asychronous delays (意味着线程彼此不是透明的，不确定对方是否还状态正常、有没有时延)。

有的问题是Turing computable but not asynchronously computable.

***

原始问题：两个进程对于某个bit要做read&write. 在非原子条件下，总是存在一个状态转移序列，使得人们碰撞。

原因：在非原子情况，对另一方动作的观察没有意义——观察后、下一个行为前，对方的状态仍然不确定。（*the famous result of Fischer, Lynch, and Paterson*）【还要理解这篇文章的大意】

> 两个人相对行走，我们要猜测对方要不要动，决定我们要不要动。我们不知道对方是在等待行动还是准备行动！【不会行动，还是在未知时间后行动】

【TODO】

所以要引入mutual exclusive.

***

## How to implement Mutual Exclusive

两种属性要求：
1. Safety Properties (Nothing bad happens ever). (Mutual Exclusive)
2. Liveness Properties (Somthing good eventually happen). (If one wants in, it gets in; If both wants in, one gets in.)

### Phone Protocol

在要进入区域时，和对方通信，问是否可以进入【不可，对方可能在做别的或者时延（asychronous系统带来的问题）】【理想的方式是persistent(like writing，不需要请求方被“请求”本身block住) and not transient(like speaking，需要及时通信、以获得对方对全局资源的状态（应该可以立刻看到共享资源状态？）)】

### Can Protocol:

 (借助interupt(每次1bit)，每次弄倒一个瓶子，引起对方注意；对方有时间时恢复这些bit), 因为对方可能来不及清除有效位【这个其实没有特别理解，can的倒下代表强制通信、询问状态？】

### Flag Protocol

Flag -- 全局、随时读取的信息（本人写、其他方读）
1. naive 平等版本：deadlock

2. （不平等协议）优先级版本 （Proof）【不好：unfair + waiting】

> 这个waiting，是指那部分【在mutual exclusion中，waiting不是一定的吗？】

## 进阶情景：生产者与消费者

1. 可以由“Can Protocol”解决！【TODO: 证明；这个Can是什么含义，与之前有什么不同（有自然的上限！达到上限、no progress，不算有问题（？）。）】

Correctness:
1. mutual exclusion + producer/consumer -> safety
2. no starvation -> liveness.

遗留问题：could also solve using flags?【TODO】

> Both problem use waiting! -> if one participant is delayed, so is everyone else.

## Readers/Writers

No mixed message 

粒度锁 => we want as much of the code as possible to execute concurrently (in parallel).


***

【need: review】