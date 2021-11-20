# 6. Operational Semantics

这一节讲如何形式化地描述一门程序语言的语义。

语言有两个组成，语法和语义，语法要先设计好才能给出语义定义。

定义好语法后，我们已知道语法树每一种可能的父子结构。给出语义定义，就是给出每一种父子结构的分析的方案。注意，语义和语法的二义性无关，形式语义是在描述如何在一棵已经确定的语法树（CST）上做分析。至于语法树是怎么来的，形式语义并不考虑。

可以这样想：我们的程序已经被解析了一棵树，我们要“运行这棵树”，运行之前我们有某初态$\sigma$。我们的运行过程就是这棵树递归的遍历过程，不断转换我们的状态。遍历规则按照我们的语义定义工作。

所以语义规则要考虑：(1)某个结点的类型(2)其孩子们的pattern. 以决定下一个动作: (1)选择某个孩子结点递归(2)返回上一层，意味着抵达了叶子或者短路. 返回后，结点类型和返回值也改变了. 孩子们是否符合多个pattern，决定着determinism(语法树允许了多种执行顺序)(但这不意味着结果一定是多义的).

这棵树某种意义上是“动态的”，因为reduce会改变类型，while更是会“展开”。这里就是我们会关注的可中止性(最终会到达normal form). 

> 语义就是：我们如何根据某个短语（Expression）/语句（Statement）更改状态。
> 
> Expression一般代表简单运算，比如+/-.

下面都从“分析语法树的方式”角度理解语义定义。感觉不同的描述方式没什么差异。

> 我们考虑包含 四则运算，bool运算，skip, assignment, if-else，; , while，local variable，动态分配的简单语言. 并且支持abort.

## Small-Step Semantics

1. 词法与语义要做区分，并引入了语义符号$\lfloor\;\rfloor$
2. 首先要定义终结符的语义。
3. 用什么表示状态 $\sigma \in Var \rightarrow Values$ 中的Values：语义符号 / 语法符号，都可以. 选择用语义符号.
4. configurations: $(e,\sigma)$, $(b, \sigma)$, $(c, \sigma)$.第一项我理解为待分析的子树的pattern，第二项理解为此时的状态. 注意，此时的node一般表示某个运算符, 当第一项是常量时，也就意味着有一个孩子，它是运算符的值.

### Small-step structural operational semantics (SOS)

用子结构语义表示父结构语义. 这意味着每棵子树的语义不会相互耦合，兄弟节点没有干扰。

这样理解下图（Expression）：横线上左侧config指示了下一步要分析哪个子结构. 横线下左侧代表子树此时的pattern. 右侧代表子结构语义与父结构语义的关系. 可以注意到，这是规定了分析顺序的. 当相同pattern出现了多次，也就有了多种语义分析方案.

![](./pics/0x06-01.png)


对于command，若执行结束，是不需要携带返回值的. 也就是没有子节点. 此时忽略第一项.

While 的语义 借助 if 递归定义.

* normalizing: transition是否会终止.

### 一种改写：用统一的configuration表示状态. (x , $\sigma$)

* 增加了 $\llbracket\;[e|b]\;\rrbracket_{intexp|boolexp}\sigma=[n|bo]$ 符号, 表示在状态为$\sigma$下，表达式e/b语义为n.

和普通SOS是等价的，就是符号统一一点.

> 2021.11: 注意到，将（无副作用的）表达式的语义视为单步，if/while的语义都有所变化（其实是变的更简单直接了）. if 的三条规则自然变成了两条，同时while的规则不再借助if

### 引入 Going wrong

就是增加额外的一个configuration: abort. 并且(1)增加产生abort的基本子句状态改变，比如0/0会导致config变为abort. (2) 每一个rule 要考虑子树是否可能是坏的. 

> 2021.11: 注意，go wrong 和 get stuck 的差异，前者是程序的一种行为，后者是一种执行状态（没有语义可以apply了，比如skip）. 一般会给出错程序abort语义, 而不是让它直接get stuck在那里.

### 引入 Local Variable Declaration

```
c ::= ... | newvar x := e in c
```

> 错误的语义设计：`(newvar x := e in c, \sigma) --> (x := e; c; x := n(旧值), \sigma)`

newvar在并行语义中会很集中考虑. 语义一定要体现局部性.

![](./pics/0x06-02.png)

> 2021.11: 为了增加“局部”语义，必须考虑在configure和语义的设计中实现它，成为真的局部语义。否则该语义难以拓展。
> 
> 在这个语义中，局部的值被记录在了语法中；`\sigma`设计时就是全局的memory.

### 引入 动态data

此时状态要增加一个Heap. 之前只有Store.

Heap是一个partial mapping，相同指针可能指向同一处（共享内存）. Values也增加`Loc`（地址）类型. Config也就进化为(c, (s, h)). 分离逻辑中集中考虑这一部分.

> 2021.11: heap 强调它是有限的；它含有的是已allocate的内存.

四个新statement: allocation / lookup(load) / mutation(store) / deallocation.

> 2021.11: 注意：我们是在statement级增加的heap操作，而非表达式级别，如此表达式求值时不必考虑heap location这个东西.
> allocation 不会出错；free可能出错；新的内存地址一定是没分配过的，释放的内存地址（以及store/load）一定是分配过的.

### 进一步简化表示方法：Contextual Semantics

> 2021.11: SOS的定义时，用子结构的SOS（放在前提中），构建更大的具体结构的语义. 但我们观察到每次修改的只是“redex”（比如对于表达式，我们只会规约 `n op n`，从来不会规约 `e op n`）. 语法树的其他部分都可以归为context. 
> 
> "redex" 是语义只可能会规约的叶节点..

$\mathcal{E}$. $[\;]$暗示下一个要被分析的子节点. 

Redex + Evaluation contexts. 虽然说是分别考虑了local / global rule，我觉得没啥变化. 还是用“在某状态下，根据某节点和其孩子们的pattern确定下一步的执行动作”.

> 2021.11: 虽然还是模式匹配，但是对context和redex两个概念进行了划分，使得语义清晰了 —— 配合原始的语法，定义了一个新的语法，可以将一个式子写为`E[r]`的形式（找到这棵树的E部分和r部分）.
> 
> redex 就是自身就可以reduce的规则，我们需要直接为它去设计local reduction rule. 对于context，它只具有一个reduction rule（如果local reduction rule与context无关），但是context的语法定义，决定了evaluation strategy.
> 
> 当语义和context有关时，也就需要另设计reduction rule. 实习的时候，Maciej定义的rule就是这一种，记得邮件里有讨论它的抽象机... 有些不记得了... 唔。什么时候再回顾下这个工作。

就是写起来更简单了，更符合我们的理解方式.

和SOS中progress与determinism的关联：
1. exists redex => progress
2. unique redex => determinism

> 2021.11: bool计算短路与否的例子. 调整了context和redex的定义.
> 
> redex: program counter 
> 
> 优点：rule更简洁了；缺点：直接去实现它的虚拟机是比较低效的，因为每一步需要完整分解command去寻找redex（我觉得这是小步语义的通病，都需要从**完整的树**开始找到实际reduce那部分，做一个微小的reduce，停下；正常执行二进制程序时，看到的就是一堆线性的程序，不需要看完整的程序去找pc）
## Big-Step Semantics

2021.11: 看似Big Step没有规定表达式的求值顺序，其实是因为没有必要（因为没有副作用）. 顺序还是可以由前提中每个子结果的状态的联系确定. 不过这个执行顺序不对用户呈现。定义出了一个abstract interpreter.

> Determinism: 一个表达式如果求出来了两个值，两个值一样
> 
> Totality: 每一个表达式，在任何状态下，它都有一个值

> 对于while loop，只能表达可终止的；没有办法区分无法终止、get stuck

bigstep下的newvar规则，我觉得它是使用了最朴素的方式，大步语义看起来很不适合表达并发，这种程序不同片段会产生关联，导致大量不确定性的程序要素

Small Step 是关于树的线性序列，Big Step就是一个单一树结构的执行

> 2021.11: 小步语义就是很基本地，每一次只考虑粒度最小的一步。大步语义认为一棵子树的结果，就只依赖于它的子树，并且预设了终止，所以不能建模很多复杂特性。（有点类似Inductive和CoInductive）。BigStep证明东西有时更简单，因为它的规则更少.

## 操作语义Sum up

* 是对动态语义的具体规范
* 抽象，只纳入想要讨论的东西（当不想考虑寄存器时，语义便不必包含它）
* 常常不是compositional（while）
* 是一种重要的语义，是对语言证明、推理的基础

## 以Untyped Lambda Calculus为例

若直接按照ULC语法写语义，得到的rules就是non-deterministic的. 因为对于 $M,N :== M N$ 这一项，想先执行那个子树都不影响结果. 此处也能看出函数式编程的no side-effect特性，完全依赖reduce/map，没有全局状态.

> 问题：什么时候可以说“Big-Step”与“Small-Step” Semantic是equivalent的？比如，对于一个term，在大步下得到的term，在小步走若干步也能走到.