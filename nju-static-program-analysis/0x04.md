# Dataflow Analysis - Foundations

几个问题：

1. 算法一定会终止吗？（reach a fixed point）
2. 有唯一解吗？
3. 要算多久？

看待迭代算法的新视角：不断更新有限个节点的有限个位的IN/OUT（假设就是OUT）。我们将每个节点的OUT列在一起，标记为$V^k$，我们想证明每一次迭代（看作函数$f: V^k\rightarrow V^k$），具有“单调性”——$V^k$和$f$构成偏序集——由于$V^k$是有限的，算法就一定会终止。

我们的算法最后找到的解，就是$X = F(X)$的解。它被称为不动点（fixed point）. 它是唯一的吗？

## Math Preliminaries

### 偏序集，Poset
**偏序集（poset）**：$P, \sqsubseteq$，其中$\sqsubseteq$是满足自反，反对称，传递三个性质的关系. （注意，自己和自己是被包含在关系中的）。

> 偏序，意味着可能无法比较，partial -> incomparable

upper bound/lower bound of subset $S\subseteq P$: $P$中的比每个S中元素都大或小的元素。（应该可能不存在）

least upper bound(lub/join) of S: 最小的那个upper bound，$\sqcup S$. for every upper bound of $S$, say $u$, $\sqcup S\sqsubseteq u$. 

greatest lower bound(glb/meet) of S: 最大的lower bound，$\sqcap S$.

若S只有两个元素，我们一般写成二元运算的形式。

一个**偏序集**的lub/glb 可能不存在，若存在，则唯一（反对称性）。

### 格，Lattice

如果偏序集中任意的两个元素，都具有lub/glb，那么我们称这个偏序集为格。

semilattice， 半格：join semilattice, 任意两个元素都有lub; meet semilattice, 任意两个元素都有glb.

complete lattice, 任意的子集（包含全集、任意无穷子集！），都存在lub/glb. 

every complete lattice has a greatest element, called top, $\top = \sqcup P$
and a least element, called bottom, $\bot = \sqcap P$. **每一个有限格，都是complete lattice.** 这是我们主要探讨的控制流分析的模型。

Product Lattice，我们可以用有限个格，构造一个新的格。就类似笛卡尔积啦... 这个新的格保持构造格的共同性质。

## Data Flow Analysis Framework via Lattice

Dataflow analysis framework: $(D,L,F)$ consists of

1. D: direction of dataflow, forwards or backwards.
2. L: a lattice including domain of the values $V$ and a meet $\sqcap$ or a join $\sqcup$. （根据是forwards or backwards，我们需要定义如何整合多个输入或多个输出）
3. F: a faimly of transfer functions from V to V. (每一个BB都有一个transfer function)

回归问题：

1. 算法一定会终止吗？考察单调性。

    * 我们说一个格上的函数是单调的，$\forall x,y\in L, x\sqsubseteq y\Rightarrow f(x)\sqsubseteq f(y)$.
    * Fixpoint Theorem: 对于一个有限全格L，和一个单调函数f，我们通过迭代找到least fixed point或另一个方向是greatest fixed point.
    ![](./pics/04-01.png)
    * 证明存在fixpoint: 因为是有限格，还是单调的，早晚会停。
    * 证明是least fixed point: 直觉上就是没有办法“绕过最小的fixpoint”
    ![](./pics/04-02.png)

2. 有唯一解吗？如果有多个解，是最好的解吗？考察fixpoint的个数、以及算法找到的是哪个fixpoint.
    * 是的，见fixed point theorem. 最好的解，也就是least fixed point/greatest fixed point.


套用到具体算法时，我们需要证明abstraction + transfer function + algo符合Dataflow analysis framework需求的性质。主要证迭代函数满足单调性。

![](./pics/04-03.png)

3. 要算多久？
    * 格的高度：Top至Bottom的最长路径。
    * 最坏时间：格的高度h，CFG的节点数量k。worst cases: h * k次迭代.

## May/Must analysis, a lattice view

这张图说明算法在全格上做了什么。

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

它是不满足分配律的。（但仍然保证monotonic）

## 优化：Worklist algorithm

没有变的block，可以不用在下一次迭代去算~

![](./pics/04-08.png)