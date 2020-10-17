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

【这里有一个问题：$A\models_{PL}\Gamma$没有过定义！意思应该是$\Gamma$里面的每一个formula都恒真吧】【TODO】

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