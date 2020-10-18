# Modal Logic I, Introduction

逻辑研究推理的形式（forms of inferences），then how to characterize valid forms of inferences
1. 确定formal language, 定义形式上合法的基本命题
2. 定义语义，建立推理系统。所有语义正确的（为“真的”）命题 都能用某 推理系统证明，称为completeness；若有用推理系统证明的语句，都是语义正确的的，这称为soundness.

这与“设计一个程序逻辑”，实际是一样的，因为都是“逻辑”。

## Propositional Logic，谓词逻辑

### 语法：

$$

\phi ::= p | \neg \phi | (\phi \wedge \phi), where\;p\in P.

$$

p指原子命题，它属于一个预定义的非空原子命题集合P。这个语言被标记为$PL_P$.

我们只利用与非定义True（$\neg \prep$），False（$\prep = p\wedge\neg p$）, 或，蕴含，等价。

注意，False直接用排中律定义。

语言中每一个句子，都是一个formula（公式）.

### 语义：

相对于程序逻辑，谓词逻辑的语义是简单的：它就是“真”或“假”，它没有“操作”的含义，也就是它的语义不是“多元的”。当然，一个命题的真假受决定于原子命题的取值——原子命题的取值可以理解为“状态”。

（需要恒真吗，还是在某个赋值下为真？？）

Assignment：function $A: P\mapsto\{0,1\}$. 即是命题的“状态”。

Satisfaction: $\models_{PL}\subseteq 2^P\times\mathbb{L}^P_{PL}$. 一个命题，在某个赋值下其值等于真，意味着它被满足。

Logical consequence（逻辑蕴含）：一个公式（$\varphi$）是一个公式集合（$\Gamma$）的逻辑蕴含，记为$\Gamma\models\varphi$，当且仅当 $A\models_{PL}\Gamma$ implies $A\models_{PL}\varphi$，for any assignment A.

【这里有一个问题：$A\models_{PL}\Gamma$没有过定义！意思应该是在赋值A下，$\Gamma$里面的每一个formula都真吧】【应该是对的】

如果$\varphi$ is valid(是恒真式(tautology)), 如果$\emptyset\models_{PL}\varphi$, 或写作$\models_{PL}\varphi$.

### 谓词逻辑的公理系统$\mathbb{S}$

包含：
1. Axiom schemas
2. Inference rules

如：
![](pics/0x01-1.png)

一个公式$\varphi$是在$\mathbb{S}$、与公式集合$\Gamma$上 可证明的（provable, or derivable），denoted $\Gamma\vdash_{S}\varphi$, 如果存在一个公式序列$\varphi_1 ...\varphi_n$，such that $\varphi_n = \varphi$ and that, for each $\varphi_i$ where 1 <= i <= n, 三选一：

1. $\varphi_i\in\Gamma$;
2. $\varphi_i is an axiom in \mathbb{S}$;
3. $\varphi_i can be infered from previous using a rule of inference in \mathbb{S}$.

### Soundness and Completeness

Soundness: If $\Gamma\vdash_{\mathbb{S}}\varphi$ then $\Gamma\models_{PL}\varphi$. 如果在proof system + $\Gamma$上，$\varphi$是可被推理得到的，那么它在其上也是符合语义的（satisfied）。

Strong Completeness: If $\Gamma\models_{PL}\varphi$ then $\Gamma\vdash_{\mathbb{S}}\varphi$. 在proof system上，额外加一些“公理”！公理模式被拓展。

(Weak) Completeness: $\models_{PL}\varphi$ then $\vdash_{\mathbb{S}}\varphi$不额外增加公理！

## Modal Logic

> Modal Logic is a family of logics that study of the deductive behavior of modality. 研究引入模态词的一系列逻辑。模态词是对真值的修饰。

模态逻辑，就是在谓词逻辑之上增加若干个模态词，我们一般考虑一元的模态词。

$$

\varphi ::= p\;|\;\neg\varphi\;|\;(\varphi\wedge\varphi)\;|\;\square\varphi

$$

And we define $\diamond$ as $\neg\square\neg\varphi$.

1. 在基本模态逻辑中，分别为必然、可能
2. 时序逻辑中，为“总是”，偶然
3. 道义逻辑中，为“应该”，“可以”
4. 认知逻辑中，为“已知”，（和“可知”？）

多个模态词可以同时存在。

![](./pics/0x01-2.png)

模态词可以是任意元的：General modal language 可以用modal similarity type表示。

### 一些基本定理

对于这一类模态逻辑（含有若干个一元模态词）：

![](./pics/0x01-4.png)

有以下定理。

1. P, A是可数的，则ML(P,A)是可数的。

> 语言的符号表$\Sigma$是有限的，语言是符号表的n次笛卡尔积的子集，（可数集上）有限次笛卡尔积运算维持可数性（笛卡尔积可以理解为有限次可数集上的并运算）。

2. ML(P,A) is uniquely readable. 即无二义性。

### 结构归纳法

1. 证明所有公式（formulas）都有特定的属性（比如：所有公式有相同数量的左右括号）。
2. 定义一个用于所有公式的函数。

### 克里普克语义：Kripke frame. 注意，不是可能世界语义。 

Kripke frame定义：$<W, \{R_a\;|\;a\in A\}>$，W是非空的可能世界集合，第二项是W上的二元关系。二元关系的性质决定frame的性质。【可以拓展到若干个二元关系】


Kripke model for ML(P, A): is a pair <F, V>

1. F is a frame for ML.
2. $V: W \rightarrow 2^P$ is an assignment. 【每个可能世界，都有自己的原子变量的取值】

对于每个可能世界，都单独构成一个点模型（pointed model）. (M, w)


Kripke Semantics for ML: 对于某个点模型$(M, s)$上的公式$\varphi$，我们说$\varphi$在模型$M$的w世界上被满足（is true），denoted $M,s\vDash\varphi$, 满足以下定义：

1. $M, w\vDash p\iff p\in V(w)$. 原子命题在该世界是正确的。
2. $M, w\vDash \neg\varphi \iff M,w\not\vDash\varphi$. 
3. $M, w\vDash(\varphi\wedge\phi)\iff M,w\vDash\varphi\;and\;M,w\vDash\phi$. 谓词逻辑还是直观的意思
4. $M, w\vDash\square_a\varphi\iff \forall v.\;(w,v)\in R_a\rightarrow M,v\vDash\varphi$. “必然”意味着由关系决定的可能世界的后继，都要满足$\varphi$.

简记：Let $\Gamma$ be a set of formulas. We use $M, w\vDash\Gamma$ to denote $M, w\vDash\varphi$ for each $\varphi\in\Gamma$.

公式是公式集合的逻辑蕴含：$\Gamma\vDash\varphi$, iff the following condition is satisfied: for each $(M, w)$, if $M, w\vDash\Gamma$ then $M, w\vDash\varphi$.【和谓词逻辑的logical consequence是一个意思】

我们称一个公式$\varphi$ is valid iff $\emptyset\vDash\varphi$, 或写作 $\vDash\varphi$. 也就是，$\varphi$在任何一个点模型上都为真。


谓词逻辑是模态逻辑的子集，语法意义上与语义意义上都是。

注意：Frame 与 Model的区别。Model额外有$V: W\rightarrow 2^P$的赋值。

true/valid 分别在model和Frame上谈。

也注意模态词带来的local/global的区分：true at a state / globally true in a model; valid at a state / valid in a frame.

### Relational Structure 的性质 与逻辑的性质

如果F是自反的，那么$F\vDash \square p\rightarrow p$. 非自反，则不满足。