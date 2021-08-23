# Mutual Exclusion

形式化“互斥”，讨论如何在异步并发情景中做证明。

> 有一些经典的互斥算法（一些协议），我们在这一节研究它们，去看一些微妙的性质，以小见大。



（目录）互斥问题：
1. 形式化定义
2. 2线程解决方案
3. n线程解决方案
4. 解决方案
5. 固有代价（inherent costs）

## 概念

在讨论并发程序时，我们需要确定一个语言，并提供描述时间与并发的方式。

时间：时间的外部性、与任何事物无干。是自然，控制所有事情不同时发生的手段。

**对并发计算的推理，几乎就是对时间的推理——事情是否会同时发生，不同时间间隔是否会重叠。**
在我们的验证过程中，没有一个全局的时间戳，只有事件发生的“顺序序列”。我们用时间去解释这个“ordering”。
顺序并不表示“时间的数值”（我们也不会考虑时间的数值）。
可以理解为，there is a notion of time but it is local and not global. 像是广义相对论中的那样。


### Events

线程A的事件$a_0$，是瞬间发生的，没有与其同时发生的其他事件。

线程A，形式上可以看作是一个事件序列。（Trace model）. 顺序意味着order.

事件可能有什么：
1. 对局部变量/共享变量的赋值
2. 调用函数、从函数返回
3. ...

每个事件也可以看作是状态机上的一条边。

### Thread as State machine 


Thread State: PC, local variables.

System state: Object fields(shared variables), and Union of thread states.（shared + local）

Interleavings: 不同线程并不一定是独立的（对全局变量的访问）（通信）。（并不是独立的，意味着，不是任意两个线程的seq随便interleave都可以）

Interval $A_0 = (a_0, a_1)$: time between events $a_0$ and $a_1$. Intervals may overlap/disjoint, （如果disjoint）有先后次序的区分，有“happen-before”关系. interval实际是某一线程状态的持续区间。

Precedence: $A_0\rightarrow B_0$, 前者的end event在后者start event前面。
它是非自反的，反对称的，传递的。（interval上的序是偏序，两个interval可以无法比较）

events上的序是全序（任意两个event一定可以比较）。

一些事件，是重复出现的，比如在迭代语句中。我们用上标记录它。

### Lock

```
// In Java, these methods should be used in the following structured way.
mutex.lock();
try {
  ...            // body, critical region
  } finally {
     mutex.unlock();
  }

```

critical section, is a block of code that can be executed by only one thread at a time.

标记：$CS_i^k$：线程i的第k个critical section **execution**. 像是interval，但也是“event”，因为会被原子执行。

#### Deadlock-Freedom

System as a whole makes progress. Even if individuals starve.【progress globally / local progress,exists】

如果一些线程call lock(), and never returns. Then other threads must complete lock() and unlock() calls infinitely often.

#### Starvation-Freedom

如果线程calls lock，It will eventually return. Individual threads make progress.【local progress，forall】

## Two thread Solutions

### LockOne

i是本线程，j是另一个线程。每个人都声明要去抢锁，但是没有规划在这些人中如何分配锁。

```java
public void lock() {
  flag[i] = true;
  while (flag[j]) {}
}

public void unlock() {
  flag[i] = false;
}
```

这个方案是满足mutual exclusion的。（其实是overview中第一个使用flag的策略。）反证：假设有$CS_A^j$与$CS_B^k$彼此overlap，那么考虑在进入CS前，每个线程在lock()中的上一次读，A一定读到flag[j]=0，B一定读到flag[i]=0；但两者之前都有写1，SC执行，不可能满足。

形式化的证明：

因为overlap，所以Wa, Ra, Wb, Rb可以连续的写在一起，并且我们知道Ra，Rb都读到了0，中间没有其他的event。若不满足ME，则两者overlap。
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

--- TODO
### LockTwo

用一个全局变量、每个线程让其他线程优先执行。但每个线程并不知道其他线程是否要执行！

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

> sequential execution deadlock（另一个线程干别的时，本线程动不了），感觉就是starvation。两个线程无休止地交替进行，是没有问题的。

证明mutual exclusion: 和之前类似的反证：因为overlap，所以WA RA WB RB可以连续排在一起。 为了保证read_A(victim == B), write_A(victim = A) 一定在 write_B(victim = B)，也就是有顺序` write_A(victim = A) -->  write_B(victim = B) --> read_A(victim = B)`.同理，也有一个关于read_B(victim=A)的式子。显然有环，得到矛盾。

```
由程序：
write(v=B) -> read(v=A)
Write(v=A) -> read(v=B)
根据assumption，两者都进入临界区。并发，两个线程会interleave在一起。先看第一个线程，为了read(v=A)
要求有一个子序列 write(v=B) -> write(v=A) -> read(v=A)
同理B要求 write(v=A) -> write(v=B) -> read(v=B)
两个写操作之间的先后依赖形成了环，不可。
```

### Peterson: combine LockOne and LockTwo

只考虑参与者。参与者中，后让步的，让步成功。【抢锁的人，排队拿锁】

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

Mutual exclusion: 假设都在critical region，线程A最后写victim field。因为他最后进入了critical region，所以必然会有read到另一个没有意向。由于假设A后写victim，B必然已经提出了意向，矛盾。（不可能都让步了，却都能进来。）

Deadlock Free: victim只有一个人，永远有进程可以progress. （我认为可以这样叙述：两个线程都请求（这个）资源，总有一个会拿到）。证明：一个线程若blocked，只有它是victim，且在循环处；两个线程都block，意味着victim有两个取值，不可能。

Starvation Free: 要思考一个线程是否有可能不断放锁取锁成功。假设一个人已取锁，另一个人在等，第一个人放锁后重新取锁，看见了另一个人在等，会让它先做，使等待的线程先运行，所以没有starvation。【让步策略是后面的让前面的——来到的人默认等待，使等待者先行。】

同时需要一个排队策略（每个线程知道其他线程的意图，有意图的人在一个集合中），和一个好的访问策略（控制有意图的人的访问顺序）。


## n-threads mutual exclusion protocols

1. filter generalization: Peterson lock 的泛化
2. Bakery lock: simplest and best known.

### Filter Lock

Filter lock 是直接的 paterson lock 直接的拓展，对于n个线程，它设定了一个n层的等待队列，每一层可以最多锁一个线程（有一个victim）。每有一个后来者，都会将已有线程向前推进（已有线程会发现自己不是victime）。当且前面（每一层）没有其他线程，本线程主动前进。【最后这一点，我认为可以改为只看前面一层没有别的线程，就向前进】


对于n个线程

```
class Filter implements Lock {
  public void lock(){
    for (int L = 1; L < n; L++) {
      level[i]  = L;
      victim[L] = i;
      while ((\exists k != i level[k] >= L) &&
             victim[L] == i ); 
    }} 
  public void unlock() {
    level[i] = 0;
  }}
```

Mutual exclusion 的证明：（归纳式）Level L 最多有n-L个线程通过。
basic: trivial
induction step: 归纳前提，l-1 层有最多 n-l+1线程通过；
假设都进入了，会有一个线程A，最后写了victim[L]，我们想说明它的停等条件一定是true（也就是一定存在其他线程level[k]>=L）。
```
每个线程都先写level=L，再写victim；A最后写victim，也就意味着其他线程都写完了level=L；写完victim后，A才读停等条件，意味着A一定读到了其他线程写的level=L。所以停等条件一定为真。
【形式化证明用三个 trace】
```

no starvation: 类似peterson的证明，应用到每一层；refer textbook, TODO. 但是并不能保证fairness（how long does it take a thread to enter CS），排队在前面的可能被overtake。这里需要对waiting也做形式化：bounded waiting.

```
starvation-free证明：
（倒序）归纳。任意一个到达n层的线程，可以最终到达临界区。
归纳step：对于到达了n+1层的线程，都最终进入了临界区。
假设线程在n层stuck，那么它前面最终会没有线程
【TODO，这里还是有些疑惑】
```

#### Bounded Waiting

对 waiting 进行形式化，想要描述fairness，我们可以关注线程进入临界区的次数。
我们希望是，首先调用`lock()`的线程，会更先进入临界区，但我们目前无法清楚这一件事（victim信息不包含这一点）。我们找一种替代的方法去确定这件事。

对于`lock()`，我们将它分为两部分，doorway（$D_A^k$，A线程第k次进入doorway）与waiting（$W_A^k$），前者会在有限步停止，用于设置锁相关信息；后者可能有无限步，线程根据某个条件保持等待。W结束后，会进入CS。

我们以完成D的先后，为调用`lock()`的先后。如果一个程序是fair的，也就是First come first serve，等同于 forall A、B、j、k，if D_A^j -> D_B^k，then CS_A^j -> CS_B^k. 

对于r-Bounded Waiting：forall A、B、j、k. if D_A^j -> D_B^k，then CS_A^j -> CS_B^{k+r}. 也就是任意两个线程进入若干次临界区（对于各自的第j，k次），如果一个先结束了doorway，那么它进入临界区的先后一定在第二个线程的k+r次进入临界区的前面。这从某个角度描述了“不会等待很久”。

0-Bounded Waiting就是fairness。

对于filter lock，它不会starve（前证），但对于任意r，它都不是r-bounded。
【TODO】