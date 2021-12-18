# Seperation Logic

分离逻辑在Hoare Logic上进行了简单的拓展, 1. 增加heap操作 2. 增加*，-*逻辑运算 3. 修正inference rule.

Soundness 与 Completeness 都需要做证明，这个改动看似很易懂，**其实是非常大的改动**。

`*`主要在谓词逻辑之上引入了局部性的考虑。

When $s,h\models p$ holds for all states $s,h$ (such that the domain of s contains the free variables of $p$), we say that $p$ is **valid**.

When $s,h\models p$ holds for some state $s,h$, we say that $p$ is **satisfiable**. 这一点值得考虑，Hoare Logic中似乎没有强调这个性质。

## Assertion

magic wand：p1 -* p2, 表达的，1是p1如果提供一些额外的关于堆和栈的信息，p2会成立；2是p1不可能提供影响p2的信息。两点都很重要。

## Assertion's inference rules

对于分离逻辑，尤其要考察它的assertion logic是如何elaborate，引入了指针运算的。

几个单向inference rule说明：

1. $(p_0\wedge p_1) * q \Rightarrow (p_0 * q) \wedge (p_1 * q)$, 不支持右向左，是因为q并不一定是确定性的（precise；描述确定的内存位置的信息），比如，$q:\;(x\hookrightarrow 0\vee y\hookrightarrow 1),\;p_0:\;x\hookrightarrow 0,\;p_1:\;y\hookrightarrow 1$
2. $(\forall x.p_0) * p_1 \Rightarrow \forall x.(p_0 * p_1)$, when x not free in $p_1$. (TODO，不太确定，需要理解下x not free in $p_1$究竟意味着什么).

## Assertions的四种分类

### Pure

for all stores s and all heaps h and h'. $s,h\models p \iff s,h'\models p.$

也就是只考虑s的assertion. 可以认为store就是全局性的。

不包含emp和两种arrow的assertion，是pure的。（sufficient syntactic criteria）. 当然，语法中含有这些的也有可能pure，因为它们可能不起作用。

pure的式子，也有一些特殊的定理。这个时候，* 与 $\wedge$，$-*$与$\Rightarrow$是基本等价的。

* $p_0\wedge p_1\Rightarrow p_0*p_1$，当 p0 或 p1 is pure. 因为可以直接给pure p分一个空heap，另一个还保有使它成立的全部heap
* $p_0*p_1\Rightarrow p_0\wedge p_1$，当 p0 和 p1 都 pure. 从定义出发，这个公理成立，需要可以将s, h1|= p0, s, h2 |= p1 都可以拓展到s,h|=p0与s,h|=p1。要求p0与p1都pure，式子显然成立。但如果只有一个为pure，无法保证它为了使得p0*p1成立而分得的hi是什么（虽然hi=emp一定可以使它成立，但会使得另一个取完整的h，这不一定使得其也成立）。因而不可。
* $((p\wedge q)*r\Leftrightarrow(p*r)\wedge q)$, if q is pure. q是关于非heap信息的描述，所以可以从内层*中拿出去。
* $(p_0\mathrel{-\mkern-6mu*} p_1) \Rightarrow(p_0\Rightarrow p_1) $, if p0 is pure. p1不需要关于heap的补充，所以可以换成普通的蕴含。
* $(p_0\Rightarrow p_1)\Rightarrow(p_0 \mathrel{-\mkern-6mu*} p_1)$，如果p0、p1都pure. 为什么只当p0非pure时无法成立：对于-*，p0是pure，意味着它对应任意的额外heap都可以使得p1成立。蕴含式没有包含这个forall，它对p1的约束更强，p1只对应此时此刻的堆h，无法拓展。

### Strictly exact

assertion指定了唯一的堆：作用域与值都确定的（assertion为false则另算）

for all stores s and all heaps h and h', $s,h\models p$ and $s,h'\models p$ implies $h = h'$.

值得注意的是，定义中只说明“当s确定时”，“h就是确定的”（h函数是唯一的）。那个h的定义域大小也就是确定的了。

对于一个strictly exact的assertion q，可以理解(q * true)的“准确定义了某一个堆的子堆”。可以借助magic wand把该子堆拿出来。

### Precise

For all s and h, there is at most one $h'\subseteq h$ such that $s,h'\models q$.

我理解为限制了堆的*大小*的assertion. “关注一块确定的内存空间，只是数据的内容不确定”。对于每个s，若存在h使得$s,h\models q$, h的定义域的domain是唯一确定的，即多任意个位置也不行、少任意个位置也不行、换任意个位置也不行。

1. Strictly Exact Assertions 一定是 Precise的.
2. $e\mapsto-$ is precise, 显然关注确定的内存位置
3. $p*q$ is precise, if p and q are precise. 显然，两个关注确定内存位置的不相交的子堆，合起来关注的还是确定的内存位置.
4. $p\wedge q$ is precise, when p or q is precise. 只要一个就可以，因为p、q都是在完整的h下要被满足的，p确定了内存位置，也就是给q确定了（q可以*true，去描述这些内存位置子集的信息），如果q指定其他内存位置的信息，那就一定得到false，false一定是precise。

precise使得两个单向的inference rule的反向也成立。

### Intuitionistic

An assertion i is intuitionistic iff, for all stores s and heaps h and h': $h\in h' and s,h\models i$ implies $s,h'\models i$.

只谈局部属性的assertion，在某个s下，只要包含某一个子堆，就OK. （重点在于，对于使其成立的堆，可以任意扩展，仍然成立）

Pure assertion 是 Intuitionistic的. p * i, p -* i, i -* p，等也是intuitionistic.

有许多好用的inference rules.

## Inference Rule

和Hoare Logic相比，三元组的语义额外要求了no aborts。 assertion语言如前，**同时**描述store与heap，有很多影响。

> Hoare logic 中的 constant rule 不再成立了（可以在前后condition各自与上一个相同条件；因为这个条件可能表达了堆的信息；而assertion对于堆的作用域是敏感的）. 这个rule很重要，是可以在验证中忽略无关条件的。“local reasoning”

在seperation logic 中 frame rule 代替了 constant rule. “只去推理footprint上的操作”

Frame Rules and its soundness: Safety Monotonicity + The Frame Property.

footprint: The set of variables and heap cells that may actually be used by a command (starting from a given state) is called its footprint.

并不是所有编程语言都可以满足Frame Rule的soundness，这个语言的语义需要capture the idea that a program will only depend upon or change the part of the initial heap within its footprint, and will abort if any of that part of the heap is missing. 

形式化的定义：对于某个command c，它在某个状态s, h上可以正常终止到某个s', h'，且它的一个堆的子堆可以safely执行，那么它和该子堆的差$h^*$是h'的子集，且从子堆执行的结果，可以执行到s', h'-h*.

starting in the state s,h. no execution of a command c aborts, then c is **safe** at s,h. 如果terminates without aborting, then c must **terminate normally** at s,h.

> dispose emp算是abort

safety monotonicity: 在一个语言中，如果一个堆是safe的，那么比它大的堆也是safe的；如果must terminate normally, 比它大的堆也同样保持.

The Frame Property: 在s与某个h下执行某个指令c，与将h去掉一部分没有的h^相比（保证执行仍然是safe的），终态s'一致，堆相差h'.
