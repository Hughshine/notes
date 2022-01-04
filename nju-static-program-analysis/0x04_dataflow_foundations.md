# Dataflow Analysis - Foundations

几个问题：

1. 算法一定会终止吗？（reach a fixed point）
2. 有唯一解吗？
3. 要算多久？

看待迭代算法的新视角：不断更新有限个节点的有限个位的IN/OUT（假设就是OUT）。我们将每个节点的OUT列在一起，标记为$V^k$，我们想证明每一次迭代（看作函数$f: V^k\rightarrow V^k$），具有“单调性”——$V^k$和$f$构成偏序集——由于$V^k$是有限的，算法就一定会终止。

> $f$ 在遍历顺序确定的情况下，is deterministic

我们的算法最后找到的解，就是$X = F(X)$的解。它被称为不动点（fixed point）. 它是唯一的吗？

> 将所有结点的结果放在一起，看作是格的一个实例。（虽然我们同时考虑了In/Out，但实际上只跟踪一个，forward跟踪out，inward跟踪in）

## Math Preliminaries

### 偏序集，Poset

**偏序集（poset）**：$P, \sqsubseteq$，其中$\sqsubseteq$是满足自反，反对称，传递三个性质的关系. （注意，**自己和自己是被包含在关系中的**）。

> 偏序，意味着可能无法比较，partial -> incomparable

upper bound/lower bound of subset $S\subseteq P$: $P$中的比每个S中元素都大或小的元素。（应该可能不存在）

least upper bound(lub/join) of S: 最小的那个upper bound，$\sqcup S$. for every upper bound of $S$, say $u$, $\sqcup S\sqsubseteq u$.

greatest lower bound(glb/meet) of S: 最大的lower bound，$\sqcap S$.

若S只有两个元素，我们一般写成二元运算的形式。

一个**偏序集**的lub/glb 可能不存在，若存在，则唯一（反对称性）。

### 格，Lattice

如果偏序集中任意的两个元素，都具有lub/glb，那么我们称这个偏序集为格。

> 什么样子的（有限）偏序集不是格？大概就是有一个顶端和底端

semilattice， 半格：join semilattice, 任意两个元素都有lub; meet semilattice, 任意两个元素都有glb.

complete lattice, 任意的子集（包含全集、任意无穷子集！），都存在lub/glb.

every complete lattice has a greatest element, called top, $\top = \sqcup P$
and a least element, called bottom, $\bot = \sqcap P$. **每一个有限格，都是complete lattice.** 这是我们主要探讨的控制流分析的模型。

Product Lattice，我们可以用有限个格，构造一个新的格。就类似笛卡尔积啦... 这个新的格保持构造格的共同性质。

## Data Flow Analysis Framework via Lattice

Dataflow analysis framework: $(D,L,F)$ consists of

1. D: direction of dataflow, forwards or backwards.
2. L: a lattice including domain of the values $V$ and a meet $\sqcap$ or a join $\sqcup$. （根据是forwards or backwards，我们需要定义如何整合多个输入或多个输出）
3. F: a faimly of transfer functions from V to V. (每一个BB都有一个transfer function，如果每一个tfunc都是单调的，整体就是单调的)

> 有限格上的单调函数的迭代应用（自bot/top开始），最终会到达一个不动点，该不动点是（least/greatest fixed point）

回归问题：

1. 算法一定会终止吗？考察单调性。

   * 我们说一个格上的函数是单调的，$\forall x,y\in L, x\sqsubseteq y\Rightarrow f(x)\sqsubseteq f(y)$.
   * Fixpoint Theorem: 对于一个有限全格L，和一个单调函数f，我们通过迭代找到least fixed point或另一个方向是greatest fixed point.
     ![img](./pics/04-01.png)
   * 证明存在fixpoint: 因为是有限格，还是单调的，早晚会停。
   * 证明是least fixed point: 直觉上就是没有办法“绕过最小的fixpoint”（主要是，“单调性”这个概念比较特殊，它不是说x < f(x)，而只是说x < y => f(x) < f(y)；保证迭代算法是沿着一条路径向上走的，是bot < f(bot)这个前提，（格的bot一定小于格中所有其他点））
     > 单调的f的跳跃不影响数据流分析中找fixpoint的过程的不跳跃，这里的跳跃指，x 与 f(x) 不可比 或者 x > f(x)，（这个>不包括自反，前面的>包括）
     > ![img](./pics/04-02.png)
     >
2. 有唯一解吗？如果有多个解，是最好的解吗？考察fixpoint的个数、以及算法找到的是哪个fixpoint.

   * 是的，见fixed point theorem. 最好的解，也就是least fixed point/greatest fixed point.

套用到具体算法时，我们需要证明abstraction + transfer function + algo符合Dataflow analysis framework需求的性质。主要证迭代函数满足单调性。

![](./pics/04-03.png)

3. 要算多久？
   * 格的高度：Top至Bottom的最长路径。
   * 最坏时间：格的高度h，CFG的节点数量k。worst cases: h * k次迭代.

## May/Must analysis, a lattice view

这张图说明算法在全格上做了什么。最开始是“unsafe result”，逐渐向“safe result”走。correctness是如何保证的，为什么最终停止时达到了safe：transfer function和meet function是否设计“对”了 -> 需要某种证明。

may/must 初始化为bottom/top 的原因是各自的“unsafe”定义不同。

> 最小/最大不动点的原因，是因为meet只做了最少应该做的（比如没有直接变成1111），而transfer function也是在做最基本做的事。实际上，最终还是看transfer function和meet的定义的正确性。

![](./pics/04-04.png)

how precise is our solution? 与Meet-Over-All-Paths Solution(MOP) 对比。（precision metric）

结论：如果我们的转移函数是可分配的，那么和MOP一样precise（gen/kill problems都符合）. 对于某些分析，是不符合可分配律的。

MOP[S_i]是每一条以S_i为结尾的所有路径的meet/join. => some paths may be not executable. not fully precise.  路径可能是无限的，无法枚举的.

我们的算法和MOP的差异：

![](./pics/04-05.png)

MOP是least upper bound, 算法是一个上界，所以 Ours is less precise than MOP. 当算法的迭代函数是可分配的，那么就一样准。

![](./pics/04-06.png)

## 练习：Constant propagation

常量propagation：程序某一点的某个变量，值是否一定是某个常量？不再用bit表示：pairs<x, v>. variable - value.

D: forward.

L: Domain, UNDEFINED -> CONSTANT -> NON-ACC; Meet, NAC $\sqcap$ v = NAC, UNDEF $\sqcap$ v = v(我们不关注uninitialized variables).

F: transfer function. $OUT[s] = gen\cup (IN[s]-\{(x, \_)\})$

![](./pics/04-07.png)

它是不满足分配律的（两个变量可能取不同的值，但是两者之间有约束，导致和它们相关的某个变量是常量（比如分支1中a=1;b=9;分支2中a=9;b=1;汇合点c=a+b;NAC+NAC得不到Constant.））。（但仍然保证monotonic）

> cp可以反向算(可以用may；不考虑了，感觉不直观)；f(x,y) = undefined 情景必须这样设置的原因，否则不保证单调性的；kill/gen和分配律的关系（为什么有关系），CP和kill/gen问题的关系。
>
> 提到如何不是undefined+constant = undefined, 会出现函数不单调的问题，细节不记得了。这里在写作业时还是需要想一下。undefined意味着那个块还没走，如果以后一定会升上来，那不会影响结果；如果永远undefined，它也就应该设置成undefined，...。要再想一下。
>
> gen/kill意味着bit vector问题，CP的kill不是基本的kill

## 优化：Worklist algorithm

没有变的block，可以不用在下一次迭代去算

![](./pics/04-08.png)
