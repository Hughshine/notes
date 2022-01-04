# CFL-Reachability and IFDS

CFG中并不是所有路径都是实际可达的、一定对应某个executions.（可能某个分支在目前的代码（输入范围）中不会被动态执行！）

infeasible paths: paths in CFG that do that correspond to actual executions.

我们希望程序分析的结果，尽量不受infeasiable paths的污染。但是，判断一个路径是否feasible，是undecidable.

Realizable Paths: CFG中的一个path，必须满足“调用点”与“函数返回点”匹配，才可能是可执行的。我们希望设计算法，识别realizable paths.

![](./pics/12-01.png)

## CFL-Reachablity

我们利用上下文无关文法描述可达性（其实是描述Realizable Paths）：路径如果符合文法，则可达。

![](./pics/12-02.png)

## IFDS: A Program Analysis Framework via Graph Reachability

(Interprocedural, Finite, Distributive, Subset Problem)

IFDS: is for interprocedural data flow analysis with distributive flow functions over finite domains.

Provide meet-over-all-realizable-paths(MRP) solution.

之前我们讨论了“MOP”（Meet Over All Paths），类似的定义，我们讨论其子集MRP。

### Overview

Given a program P, and a dataflow-analysis problem Q

1. Build **supergraph** G* for program P, define flow functions for edges in G* based on Q.
2. Build **exploded supergraph** G# for P by transforming flow functions to representation relations(graphs)
3. Q问题可以解决(finding out MRP solutions)，via applying Tabulation algorithm an G#.

#### SuperGraph G*

G*有三类与函数掉用有关的边：

1. intraprocedural的call->ret边
2. interprocedural的call->start边
3. interprocedural的exit->ret边

Flow functions: 我觉得就是transfer function. 用匿名函数写出。【我们的算法最终希望得到：以每个程序点为终点的所有MRP的over approximation（？TODO）】

meet？这里是以realizable path为单位，不必考虑meet.

> * 第76页 `a = a - g`的意思，应该是如果a和g有一个可能是未初始化的，那么这一次赋值会使a也可能是未初始化的（或者说成行为未定义更合适？）。
> * 第78页，call-to-return-site edge的flow function是在处理局部变量吗？好像不是，在第5课讲interprocedural的时候，call->ret的边要去掉LHS，因为会在return时被返回，（call2start edge只传局部变量）。这里也是一样的，注意，g定义为全局变量。call->ret将全局变量g删除，因为g之后是否是未定义的，只取决于被调用函数）
> * 第79页，exit-to-return-site edge，是在清除局部变量.

#### Exploded SuperGraph G#

exploded supergraph看起来就是将无副作用的flow function写成可视化的的图的形式。（似乎要求域是有限的？）

【理解Exploded SuperGraph】每个函数转换成它的representation relation. 多出来的0节点用来处理无关输入的输出。0->0是glue edge, "永远可以有无关输入的输出"。当有了0->x后，不需要有y->x.

【TODO：计算representation relation的算法】

我们的flow function是一个D（是一个超集） -> D的函数，D是有限的。它是由算数定义的，我们把它转化（也在某种程度上拓展）成等价的集合的形式。

1. 0->0 始终有边【“永远都可以‘无条件’”，glue edge】
2. 0->y，f的输入是空集时可以得到的y（无条件fact）
3. x->y，f的输入是{x}时得到的y，且y不是无条件的（有条件fact）

> * 133页的路径不行，因为绿色的exit2ret边没有与之匹配的call2start边。

然后可以在这张exploded supergraph上，应用Tabulation Algorithm to identify MRP solutions. —— 我们可以得到exploded supergraph上的每个node，是否有MRP可达。问题被转化为图上的可达性判断问题！

#### Tabulation Algorithm: Core Working Mechanism

在我们得到的exploded supergraph上，我们要找realizable paths可达的点。

复杂度O(ED^3). 做的事情：括号匹配 + 路径探索；“处理调用边、返回边、总结边，将间接可达的两节点直接连结起来”。用X_n存储从开始节点，到达节点n的所有data fact.

是怎么在算法过程排除掉unrealizable path的？... 先关注某一个data fact：

* 处理括号匹配：每次到达返回点ep'时，开始匹配，找到所有调用点和相应的返回点；
* 处理总结边，动态规划，节省时间（避免重复分析调用同一个函数的情况——context？若考虑context，应该只不过是动态的不确定次数的调用，被抽象成一个还是多个的问题。）：<Call, dm> to <Ret, dn>, 经过函数会将dm性质变为dn.

算法优点：属于动态规划，比传统的worklist在面对interprocedure问题时，更精确更省时间。

### Understanding the Distributivity of IFDS

IDFS一次只处理一个变量的传播，这种传播以分配律为特征。注意，这里的分配律和dataflow analysis中说的分配律不一样，是指“subset”（IFDS的S）操作的分配律。

constant propagation不行；pointer analysis不行（？ppt171. flow function's fact 没有alias information，我理解是需要的信息很多（points-to relation和图）等等... （而且我们学的也是flow-insensitive的？）就是看起来就不合适QAQ.. D无法是指针，因为需要传播points-to information，还需要额外的数据结构）。
