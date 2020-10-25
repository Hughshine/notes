# Mutual Exclusion

形式化“互斥”，讨论如何在异步并发情景中做证明。

时间：时间的外部性、与任何事物无干。是自然，控制所有事情不同时发生的手段。

对并发计算的推理，几乎就是对时间的推理——事情是否会同时发生，不同时间间隔是否会重叠。
在我们的验证过程中，没有一个全局的时间戳，只有事件发生的“顺序序列”。我们用时间去解释这个“ordering”。
顺序并不表示“时间的数值”（我们也不会考虑时间的数值）。
可以理解为，there is a notion of time but it is local and not global. 像是广义相对论中的那样。

## 概念

### Events

线程A的事件$a_0$，是瞬间发生的，没有与其同时发生的其他事件。

线程A，形式上是一个事件的序列。（Trace model）. 顺序意味着order.

事件可能有什么：
1. 对局部变量/共享变量的赋值
2. 调用函数、从函数返回
3. ...

### Thread as State machine 

Thread State: PC, local variables.

System state: Object fields(shared variables), and Union of thread states.

Interleavings: 不同线程并不一定是独立的（对全局变量的访问）（通信）。

Interval $A_0 = (a_0, a_1)$: time between events $a_0$ and $a_1$. Intervals may overlap/disjoint, 有先后次序的区分.

Precedence: $A_0\rightarrow B_0$, 前者的end event在后者start event前面。
它是非自反的，反对称的，传递的。（interval上的序是偏序，两个interval可能无法比较）

events上的序是全序（任意两个event一定可以比较）。

### Lock

```
// In Java, these methods should be used in the following structured way.
mutex.lock();
try {
  ...            // body
  } finally {
     mutex.unlock();
  }

```

critical section, is a block of code that can be executed by only one thread at a time.

标记：$CS_i^k$：线程i的第k个critical section **execution**. 像是interval，但也是“event”.

#### Deadlock-Freedom

System as a whole makes progress.

如果一些线程call lock(), and never returns. Then other threads must complete lock() and unlock() calls infinitely often.


#### Starvation-Freedom

如果线程calls lock，It will eventually return. Individual threads make progress.

## Two thread Solutions

### LockOne

i是本线程，j是另一个线程。

```java
public void lock() {
  flag[i] = true;
  while (flag[j]) {}
}

public void unlock() {
  flag[i] = false;
}
```

这个方案是满足mutual exclusion的。（其实是overview中第一个使用flag的策略。）反证：假设有$CS_A^j$与$CS_B^k$彼此overlap，那么考虑在进入CS前，每个线程在lock()中的上一次读写，一定是flag[i]=0, flag[j]=1或恰相反，这不可能同时满足。

形式化的证明：

因为overlap，所以Wa, Ra, Wb, Rb可以连续的写在一起，中间没有其他的event。
为了让readA(flag[B])成功读出false，write[B]=true必须在它的后面，否则它只能读出A；读flag[A]亦然。由此，能从assumption得到这两个序。

```
//由代码
writeA(flag[A] = true) --> readA(flag[B] == false)
writeB(flag[B] = true) --> readB(flag[A] == false)
//由Assumption，这四个event可以连续的处于执行序列中。但我们有额外的两个event的顺序必须满足、因为写之后再读、只会读出相同的数：【操作间的依赖】
readA(flag[B] == false) --> writeB(flag[B] = true)
readB(flag[A] == false) --> writeA(flag[A] = true)
```

这四条规则构成了一个环。不再满足偏序。QeD.

这个算法不是deadlock free的.【有一个线程的时候，不会deadlock；多个线程的时候会。】

### LockTwo

用一个全局变量、每个线程让其他线程优先执行。

```
public class LockTwo implements Lock {
 private int victim;
 public void lock() {
  victim = i;
  while (victim == i) {}; 
 }

 public void unlock() {}
}
```

> 看起来好像会starvation...

证明mutual exclusion: 和之前类似的反证：因为overlap，所以WA RA WB RB可以连续排在一起。 为了保证read_A(victim == B), write_A(victim = A) 一定在 write_B(victim = B)，也就是有顺序` write_A(victim = A) -->  write_B(victim = B) --> read_A(victim = B)`.同理，也有一个关于read_B(victim=A)的式子。显然有环，得到矛盾。


看完two thread情况，用只有lock/unlock的语法，写出deadlock的定义。【一个线程的时候，会死锁；多个线程的时候，会有progress】。

我觉得就是会starvation啊QAQ... 没有空闲则进。【可能starvation的来源就是“让步”...】

[deadlock: 系统不再progress。如果只是系统的一部分永远不再progress呢..：TODO。]

DEADLOCK究竟如何定义：
1. 我觉得定义为“系统的局部（线程子集）没有执行完（不全是skip），且永远不可能进展”，是好的。但这又和starvation有什么区别呢。
2. starvation是一个线程在想做一件事时，一定会make progress，（应该需要假设其他线程不会永久占用同步区？）.【或者不会永远调度另一个线程... 感觉需要有前提啊。。】
3. 一个线程starvation，并不意味着deadlock. 可能是它不断在让步，调度器没有让它运行。
4. deadlock：应该就是定义在多个线程之上的，“所有请求资源的线程，拿不到资源，no progress”。因为资源分别被彼此占有。deadlock主要强调这个依赖关系。


### Peterson: combine LockOne and LockTwo

```java
public void lock() {
 flag[i] = true; // （个人）意向声明
 victim  = i; // 让步（victim=i表示i让出优先级） 
 while (flag[j] && victim == i) {}; // 当另一个人有意向且让步给了它时，等待
}
public void unlock() {
 flag[i] = false;  // 取消意向
}

```

Mutual exclusion: 假设都在critical region，线程A最后写victim field。因为他最后进入了critical region，所以必然会有read到另一个没有意向。由于假设A后写victim，B必然已经提出了意向，矛盾。

Deadlock Free: victim只有一个人，永远有进程可以progress. （我认为可以这样叙述：两个线程都请求（这个）资源，总有一个会拿到）。

Starvation Free: 假设一个人在等，另一个人重新进入，它会先让权，使等待的线程先运行。