# 0x01 并发算法与理论

## 课程动机和专注点

Concurrency v.s. Parallel: 并行关注“将原问题”分解为“尽量独立的子问题”，并利用多个处理器同时求解，以得到原问题解的过程。并发关注“多个处理器面对共享内存资源时”，如何“同步多个处理器的计算”。两者有着很不同的关注点。

> 并发考虑不同线程间对于共享资源的通信、同步；并行化考虑大问题的拆分。

这门课关注各种“锁算法”更深入的理论基础，以及细粒度锁或无锁的并发数据结构。**关注并发算法的通用设计思想和正确性原理**。

> 软件效率愈来愈依赖并发，单硬件上限已经不够，需要靠并发承载软件的膨胀。压榨每一点并发可能，很重要。并发同时带来正确性问题，也关注使效率更高的技巧。这需要关注与并发相关的体系结构。
>
> 【verification】
>
> 通用并发算法，锁理论，是这节课的关注点。正确性比实用性更重要，不允许任何错误出现。
>
> 开始要区分硬件软件术语，processor，threads...

***

并发模型：多线程 + single shared memory (where objects live) + unpredictable asychronous delays (意味着线程彼此不是透明的，不确定对方是否还状态正常、有没有时延)。

有的问题是Turing computable but not asynchronously computable.（TODO，后面会看这类问题）

***

## 一些情景

并行的整数测试：输出从1到10^10的所有质数，有多个cpu。

* **并行（静态分配任务）使用并发同步（动态分配任务）获得更好效率**：workload不是线性的；cpu性能可能不同；可能有动态场景影响每个cpu的性能。是无法找到一个准确的静态任务划分方式，使得每个cpu被充分利用，最终并行化效果最好。

* **动态任务分配需要引入共享信息，表示接下来要做的任务：**需要保证互斥访问（一次任务分配）。

  ```
  // 需要同步的函数
  public long getAndIncrement() {
  	return value++ 
  }
  value++ 实际是读+写两步操作。问题在于两步之间可能插入其他操作。写的逻辑基于读，如果其他线程在中间读了，会导致其他线程逻辑出错，系统中相同任务被领取了多次；如果其他线程在中间写，则意味着写操作被覆盖。
  语言常常提供并发关键字，比如java的synchronized
  public long getAndIncrement() {
  	synchronized {
  		tmp = value;
  		value = tmp + 1;
  	}
  	return tmp;
  }
  ```

* **不借助互斥行不行，是否有一种协议可以有效完成读写**：不行

原始问题：两个进程对于某个bit要做read&write. 在非原子条件下，总是存在一个状态转移序列，使得人们碰撞。

原因：在非原子情况，对另一方动作的观察没有意义——观察后、下一个行为前，对方的状态仍然不确定。（*the famous result of Fischer, Lynch, and Paterson*）**【还要理解这篇文章的大意】**

也就是阅读信息和行动无法一定连贯在一起，行动前的准确信息由于新线程的参与永远无法保证。

> 两个人相对行走，我们要猜测对方要不要动，决定我们要不要动。我们不知道对方是在等待行动还是准备行动！【不会行动，还是在未知时间后行动】【双方要尝试对另一条路进行读（看看有没有人）和写（走过去），但我们看到没有人、不能保证走过去时还是没有人。（永远无法清楚）】

所以要引入机制支持mutual exclusive.

***

## How to implement Mutual Exclusive

两类属性要求：总在进展，并且坏事永远不会发生。“进展”与“坏事”对于特定问题需要定义。
1. Safety Properties (Nothing bad happens ever). (Mutual Exclusive)

2. Liveness Properties (Somthing good eventually happen). (If one wants in, it gets in; If both wants in, one gets in.)

   deadlock, circular-waiting; livelock(state changes, but no effect);

   starvation: 一个线程永远无法得到共享资源。一个线程若在deadlock/livelock中，那一定是starve的；一个算法即使不会有deadlock/livelock，也“可能”造成starvation，如果一些线程永远“没抢到”资源【比如操作系统中，优先级算法以避免线程抢占时有的线程很久没有抢到的问题】。

***

情景：两家人带小宠物去池塘，小宠物间会争吵。

### [不可行，liveness] Phone Protocol

在要进入区域时，和对方通信，问是否可以进入。

不可，对方可能在做别的或者时延（asychronous系统带来的问题）。

理想信息交换方式应是persistent(like writing，**不需要使信息提供方不断主动提供信息（我方主动行为依赖另一方的主动行为便不好——transient）**，进而导致请求方被“请求”本身block住) and not transient(like speaking)

### [不可行] Can Protocol

drug can 主动告知对方信息（不需要对方反馈），但该信息需要对方抹去（只能通过**有限的方式影响对方家里的信息**，比如只能拽倒can，不能扶起can；用来模拟线程间的interrupt）。

双方各自家里都设置一个标识，在对方想要和自己通信时修改它（通过interrupt）。双方保证常常去注意这个标识、并复原它。

> 目前这样理解can的含义：drug 一次 can 表示进行外出或返回一次动作，数drug数量是瞬间的事。每一次想要外出时，停等检查翻倒的can数量，直到为偶数，立刻翻到对方的一个。其他时候可以作其他事。
>
> 值得注意的是，我们不能令can的语义是“让对方外出”，即notify() wait()语义。人们的生活不止遛狗，并且人们可以任意溜多次狗。
>
> ```
> while true:
> 	do()
> 	notify()
> 	wait()
> 
> while true:
> 	wait() -- 结束等待时复原can
> 	do()
> 	notify()
> 	
> 这个约束太强了，不是mutual recursion了；能不能适用于更多线程也需要想，TODO
> ```

### Flag Protocol

Flag -- 全局、随时读取的信息（本人写（无限制的修改）、其他方读）
1. naive 平等版本：有deadlock。

> 对于Alice和Bob: 
> 1. Raise flag
> 2. Wait until other's flag is down（等待其他线程做出行为）
> 3. unleash pet
> 4. lower flag when pet returns

问题：两个人棋子都升起、都在等待对方放下。

2. （不平等协议）优先级版本 （Proof）【不好：unfair + waiting（指升旗后高优先级线程再不进展、影响了低优先级线程——是不支持fault-tolerance的一个体现）】

> 修改Bob的协议:
> 1. Raise flag
> 2. While Alice's flag is up, lower flag, wait for Alice's flag to go down. Raise flag
> 3. unleash pet
> 4. lower flag when pet returns

一定会先表明意向（升旗子），再看对方的状态（在没有升旗子（表明占用）的情况下，“看”本身无意义）。双方彼此知道对方的意向。（不举旗子，相当于彼此都知道对方“可能行动”；现在可以明确对方“要行动”或“没有意向行动”。）

然后只需要处理双方都要行动时的安排；我们没有办法知道先后顺序，只好规定一个先验的顺序（unfair）。

```
Proof(ME).
    反证，假设两个pets都在，推理矛盾。
    A或是B，一定看到两面旗。
    一定不会进入。
Proof(No deadlock).
    两个人都想进入时，Bob gives Alice priority.
```

并没有彻底的完美情景。但也可用：mutual exclusion 不能被及时的message passing、中断处理（私有数据），只能借助基于共享内存的持久数据传递。

mutual exclusion 是一个协调问题（coordination problem）。对于它本身而言，其语义本质**（need explanation）** 需要waiting，没有办法避免。不过其他协调问题是可能不需要waiting的。解决ME，必须需要信息交换。

## 进阶情景：生产者与消费者

问题：生产者消费者不能同时进入“仓库”；当仓库满时，生产者不再生产；仓库空时，消费者不再消费。

1. 可以由“Can Protocol”解决！【TODO: 证明；这个Can是什么含义，与之前有什么不同（有自然的上限！达到上限、no progress，不算有问题——双方不会无限制地连续进入。）（双方的资源是彼此消耗的，所以一方只需要做一个动作（我认为这一点很重要））】

> 与ME的区别在于，该场景增加了一个限制，就是任一方不会无限制进入（仓库有上限和下限），一定是个交互式访问仓库的过程，这意味着己方行动前，对方的状态的可能性是有限的，可以用有限个罐子建模（只能打翻对方罐子、不能修正对方罐子）。

这个Can，就是一位的全局变量。两方，分别只做reset和set，意味着只要发生了变化，就一定能确定这个变化是哪个线程产生的，不需要“原子动作”保证连续操作。

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
2. no starvation(if Bob always willing to feed, and pets always famished, then pets eat infinitely often) -> liveness.（反证：证明没有“no food，且B无法提供食物 indefinitely”的情景；B无法提供食物，那么这时can的状态是down，这意味着A可以操作，A最后的操作是reset can，那么罐子就会复原，B又可以提供食物了。）

```
Proof(ME):
    # 证明can发生的四种可能变化的结点或区间（也就是所有时间），ME都holds。
    假设can is down. 
        只有pet可以进入，ME holds.(down->down，即只要罐子翻倒期间)
        为了将can reset（only Alice can do this），pet会出来，Bob也不会在此时进入，所以在down->up的过程（相比down->down是很短的时间）是满足ME的。
    假设can is up.
        此时只有Bob可以进入，MEholds.(up->up).
        为了将can knock over（Bob can do this）, Bob会先出来，pet不会在此时进入，之后才能进入pet，所以 up->down的过程是满足ME的。
    没有其他迁移，QeD.
```

> Both problem use waiting! -> if one participant is delayed, so is everyone else. waiting意味着让计算具有了一定顺序性。这不是希望的，希望可以减少，但有时受决定于问题的性质。

遗留问题：could also solve using flags? 我认为是可以的。

> 仍然让Alice相对Bob优先；
> 假设有global旗帜表示食物是否空了，与是否满了。
> A想进入时，如果食物是空的，不升旗等待；如果食物非空则升旗，等待B降旗；B降旗，则进入临界区。
> B想进入时，如果食物是满的，不升旗等待；如果食物非满则升旗，若A升旗，则B降旗defer；A降旗，则进入临界区。
> 两者在临界区内时，会准确调整旗帜状态。

证明ME：如果两个人都在临界区，那么A/B都会升旗，但是两人进入临界区都会在对方降旗时进入。
证明deadlock-free：B defer
证明starvation-free：no food, 则食物非满；B无法提供食物，则A升旗，A升旗后看到no food，会降旗，最终使B可以进入。

## Readers/Writers

No mixed message: 与我们的机器内存行为紧密相关。如果我们可以保证不同的核每次可以以 one word 为单位去写，如果我们要写一连串word，如何保证它是准确的？

We want to devise a protocol so that: 
* writer writes one letter at a time
* reader reads one letter at a time【保证读的粒度】
* And Reader sees: old message or new message but mixed message

使用mutual exclusion很容易实现。但这需要waiting，即使使用粒度锁。

粒度锁 => we want as much of the code as possible to execute concurrently (in parallel). 

【We can solve R/W without mutual exclusion! 在以后会提到。】

***

并发性的一点点提升，可以带来巨大的效率提高。（Amdahl's Law）

***

## Safety and Liveness

safety: something bad will never happen.

liveness: something good will happen, but we don't know when.

### For sequential programs

safety: the program will never produce a wrong result (partial correctness).

liveness: the program will produce a result(termination).

### For state-transition graphs

safety property: 若不满足该属性，不满足的情景总是在有限次转移中出现（"if something bad happens on an infinite run, then it happens already on some finite prefix."）【也意味着，若R的每个前缀满足P，R满足P】

liveness property: 错误永远不会在有限次转移后出现。(也即我们关注的不是有限序列的性质)（those properties whose violation never has a finite witness）【也意味着：对于任意有限转移R，存在R的拓展R1，R1满足P】。【这两个等价吗，疑惑，TODO；可能只是在说明一个情景】

换句话说：

safety property: the properties that can be checked on finite executions. （如果坏事发生了，一定是在有限的执行就发生了）

liveness property: the properties that cannot be checked on finite executions. (And they need to be checked on infinite executions).（坏事是关于无限的，比如“线程永远不再有机会执行”）

> 注意区分safety property 和 safety 本身

### Example

1. Mutual exclusion: safety. 虽然我们希望永远满足mutual exclusion，但如果违背它，这个违背的执行序列是有限长度的。
2. Bounded overtaking: safety. 在a想要进入critical region时，b最多进入了一次。
3. Starvation freedom: liveness. 假设B不会永远呆在critical section，当A想要进入critical section, 它最终会进入。【locally，关注每一个线程的局部视角】。【“不会进入”是一个关于无限执行的性质，我们无法在有限的执行就判断一个线程永远无法再进入临界区】
4. Deadlock freedom: liveness. 有一些进程一直在进展。 【globally，关注总体视角，不关心每一个线程的情况】。虽然也可以叙述为“不是所有的线程会同时get stuck”，但它需要在任意execution上都要验证，在有限的execution上get stuck或未get stuck，与无限的execution get stuck or not 无关。

starvation-freedom 是强过 deadlock-freedom的。

## 2021 Updated

看待一个并发程序：并发对象(concurrent objects) + their clients

设计一个新的并发算法是难的，课程中要做的是理解已有的并发算法，把握其正确性的直觉，进而理解并发背后的数学特点，在自己写并发程序时可以很好地利用这些并发算法，写出来好的concurrent objects。并了解并发的限度（并发可计算性）。—— 达到“并发算法也就那么回事”的程度。

弱内存模型是有难度的，这门课在介绍高级语言的memory model后，介绍的并发对象都是在SC model上。