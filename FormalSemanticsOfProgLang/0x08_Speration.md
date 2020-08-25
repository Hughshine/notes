# Seperation Logic

分离逻辑在Hoare Logic上进行了简单的拓展, 1. 增加heap操作 2. 增加*，-*逻辑运算 3. 修正inference rule.

Soundness 与 Completeness 都需要做证明，这个改动看似很易懂，**其实是非常大的改动**。

`*`主要在谓词逻辑之上引入了局部性的考虑。


When $s,h\models p$ holds for all states $s,h$ (such that the domain of s contains the free variables of $p$), we say that $p$ is **valid**.

When $s,h\models p$ holds for some state $s,h$, we say that $p$ is **satisfiable**. 这一点值得考虑，Hoare Logic中似乎没有强调这个性质。


## 四类Assertions

### Pure

for all stores s and all heaps h and h'. $s,h\models p \iff s,h'\models p.$

也就是只考虑s的assertion. 可以认为store就是全局性的。

不包含emp和两种arrow的assertion，是pure的。（sufficient syntactic criteria）

pure的式子，也有一些特殊的定理。这个时候，* 与 $\wedge$ ，$-*$与$\Rightarrow$基本是等价的。

### Strictly exact

assertion指定了唯一的堆：

for all stores s and all heaps h and h', $s,h\models p$ and $s,h'\models p$ implies $h = h'$.

值得注意的是，定义中只说明“当s确定时”，“h就是确定的”（h函数是唯一的）。那个h的定义域大小也就是确定的了。

对于一个strictly exact的assertion q，可以理解(q * true)的“准确定义了某一个堆的子堆”。

### Precise 

For all s and h, there is at most one $h'\in h$ such that $s,h'\models q$.

我理解为限制了堆的大小的assertion. 对于每个s，若存在h使得$s,h\models q$, h的定义域大小是唯一确定的。

Strictly Exact Assertions 一定是 Precise的.

### Intuitionistic 

An assertion i is intuitionistic iff, for all stores s and heaps h and h': $h\in h' and s,h\models i$ implies $s,h'\models i$. 

只谈局部属性的assertion，在某个s下，只要包含某一个子堆，就OK. 

Pure assertion 是 Intuitionistic的. p * i, p -* i, i -* p，等也是intuitionistic.

有许多好用的inference rules.

## Inference Rule

Frame Rules and its soundness: Safety Monotonicity + The Frame Property.

footprint: The set of variables and heap cells that may actually be used by a command (starting from a given state) is called its footprint. 

并不是所有编程语言都可以满足Frame Rule的soundness，这个语言的语义需要capture the idea that a program will only depend upon or change the part of the initial heap within its footprint, and will abort if any of that part of the heap is missing.


形式化的定义：（不是完全理解）


starting in the state s,h. no execution of a command c aborts, then c is **safe** at s,h. 如果terminates without aborting, then c must **terminate normally** at s,h.

> dispose emp算是abort

safety monotonicity: 在一个语言中，如果一个堆是safe的，那么比它大的堆也是safe的；如果must terminate normally, 比它大的堆也同样保持.


The Frame Property: 在s与某个h下执行某个指令c，与将h去掉一部分没有的h^相比（保证执行仍然是safe的），终态s'一致，堆相差h'. 


> 讲了每一个inference rules，主要区分global/local，mutation处区分是否是FV。额外的，讲了几个简单的验证例程。对inference rule的具体理解，认为还需要用coq做练习，仅看一遍总感觉大概懂了的，或者总觉得不是完全正确；之前写的链表插入的验证到现在也不确定对不对。后面用些时间读Software Foundation. 其他想做的事：那个Lambda-Calculus Interpreter的项目感觉还不错; 过一遍SICP课程; 读TAPL. 有时间过一遍老师的论文。 