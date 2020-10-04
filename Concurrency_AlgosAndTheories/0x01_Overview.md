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

> 两个人相对行走，我们要猜测对方要不要动，决定我们要不要动。我们不知道对方是在等待行动还是准备行动！【不会行动，还是在未知时间后行动】【双方要尝试对另一条路进行读（看看有没有人）和写（走过去），但我们看到没有人、不能保证走过去时还是没有人。（永远无法清楚）】

所以要引入mutual exclusive.

***

## How to implement Mutual Exclusive

两种属性要求：
1. Safety Properties (Nothing bad happens ever). (Mutual Exclusive)
2. Liveness Properties (Somthing good eventually happen). (If one wants in, it gets in; If both wants in, one gets in.)

### Phone Protocol

在要进入区域时，和对方通信，问是否可以进入【不可，对方可能在做别的或者时延（asychronous系统带来的问题）】【理想的方式是persistent(like writing，不需要请求方被“请求”本身block住) and not transient(like speaking，需要及时通信、以获得对方对全局资源的状态（应该可以立刻看到共享资源状态？或者至少可以立即通信）)】

### Can Protocol【TODO】

【其实没看懂，这个can是在做什么：是在告知“想要通信？”如果是这个意思，send多少bits都是send一个bit的意思，为什么会需要无限的bits.】【TODO】【还是意思是，即使interrupt也可能听不见？？】

双方各自家里都设置一个标识，在对方想要和自己通信时修改它（通过interrupt）。双方保证常常去注意这个标识、并复原它。

### Flag Protocol

Flag -- 全局、随时读取的信息（本人写、其他方读）
1. naive 平等版本：有deadlock。

> 对于Alice和Bob: 
> 1. Raise flag
> 2. Wait until other's flag is down
> 3. unleash pet
> 4. lower flag when pet returns

问题：两个人棋子都升起、都在等待对方放下。

2. （不平等协议）优先级版本 （Proof）【不好：unfair + waiting（waiting是一定的吗）】

> 修改Bob的协议:
> 1. Raise flag
> 2. While Alice's flag is up, lower flag, wait for Alice's flag to go down. Raise flag
> 3. unleash pet
> 4. lower flag when pet returns

一定会先表明意向（升旗子），再看对方的状态（在没有升旗子（表明占用）的情况下，“看”本身无意义）。双方彼此知道对方的意向。（不举旗子，相当于彼此都知道对方“可能行动”；现在可以明确对方“要行动”或“没有意向行动”。）

然后只需要处理双方都要行动时的安排；我们没有办法知道先后顺序，只好规定一个先验的顺序（unfair）。

任何对共享区的访问，都要做“read”&“write”两个动作。没有办法通过“只增加共享状态位”解决同步问题（无限的..）。

```
Proof(ME).
    反证，假设两个pets都在，推理矛盾。
    A或是B，一定看到两面旗。
    一定不会进入。
Proof(No deadlock).
    两个人都想进入时，Bob gives Alice priority.
```


## 进阶情景：生产者与消费者

问题：生产者消费者不能同时进入“仓库”；当仓库满时，生产者不再生产；仓库空时，消费者不再消费。

1. 可以由“Can Protocol”解决！【TODO: 证明；这个Can是什么含义，与之前有什么不同（有自然的上限！达到上限、no progress，不算有问题——双方不会无限制地连续进入。）（双方的资源是彼此消耗的，所以一方只需要做一个动作（我认为这一点很重要））】

这个Can，和interrupt没有关系，就是一位的全局变量。两方，分别只做reset和set的东西，不需要“原子动作”保证连续操作。

```
Alice:
while(true) {
    while(can.isUp()) {}
    pet.release();
    pet.recapture();
    can.reset();
}

Bob:
while(true) {
    while(can.isDown()) {}
    pond.stockWithFood();
    can.knockOver();
}
```

Correctness:
1. mutual exclusion + producer/consumer -> safety
2. no starvation(if Bob always willing to feed, and pets always famished, then pets eat infinitely often) -> liveness.

```
Proof(ME):
    假设can is down. 
        只有pet可以进入，ME holds.(down->down)
        为了将can reset，pet必须出来. 所以在down->up的过程是满足ME的。
    假设can is up.
        此时只有Bob可以进入，MEholds.(up->up).
        为了将can knock over, Bob必须出来. 所以 up->down的过程是满足ME的。
    没有其他状态，QeD.
```

遗留问题：could also solve using flags?【TODO】

> Both problem use waiting! -> if one participant is delayed, so is everyone else.

## Readers/Writers

No mixed message: 与我们的机器内存行为紧密相关。如果我们可以保证不同的核每次可以以 one word 为单位去写，如果我们要写一连串word，如何保证它是准确的？

We want to devise a protocol so that: 
* writer writes one letter at a time
* reader reads one letter at a time
* And Reader sees: old message or new message but mixed message

使用mutual exclusion很容易实现。但这需要waiting，即使使用粒度锁。

粒度锁 => we want as much of the code as possible to execute concurrently (in parallel). 

【We can solve R/W without mutual exclusion! 在以后会提到。】

***

【need: review】