# 三、关于互摹拟的进一步探讨

互模拟关系，并不能直接与modal equivalence关联。需要限制模型类的范围。

Hennessy-Milner classes: 具有$M,w\;\underline{\leftrightarrow}\;N,v\Leftrightarrow M,w\equiv_{ML}N,v$ 性质的所有模型类. 我们想要找到最大的H-M class.

## Image-finite models

A model $M$ is image-finite iff for each state $w\in W_M$ and each relation $R_a^M$, the set $\{v\in W_M\;|\;(w,v)\in R_a^M\}$ is finite.

每一个可能世界的后继数量都是有限的！

### On image-finite models, 两个点模型互模拟 <=> 两个点模型模态等价

只需要证右至左的方向。

Let $Z\subseteq W_M\times W_N$ be defined as $\{(w,v)|M, w\equiv_{\texttt{ML}}N, v\}$. 我们要证明Z是一个bisimulation.

1. 赋值维持。很容易被证明。
2. 关系维持（左至右和右至左的证明一致）。已知$(w\equiv_{\texttt{ML}}v)$，假设$(w, w')\in R_W$，我们要证，存在$v'$，$(v, v')\in R_V$，且$(w'\equiv_{\texttt{ML}}v')$. 我们借助反证法，假设不存在这样的v'，那么v的每个后继v'都有一个公式$\varphi_{w',v'}$在(M, w')满足，在(N, v')不满足. 我们收集所有这些公式，做一个finite conjunction（因为是image-finite的），可以得到新的公式$\varphi$（如果v没有后继，让$\varphi:=T$）.那么$\Diamond\varphi$在$M,w$成立，在$N,v$不成立，这与我们的前提（w, v是模态等值的）矛盾。因而命题得证。



## m-saturated models

image finite model 是 m-saturated models的子集。

一些概念：

1. 公式集可满足：对于一个模型$M$，X是其论域的子集，我们说一个模态公式集$\Sigma$是X可满足的，当且仅当X中的某一个世界中，$\Sigma$的每个公式都成立（iff there is $w\in X$ such that $M, w\vDash \varphi$ for all $\varphi \in \Sigma$）。【这意味着，如果$\Sigma$不是在X中可满足的，那么对于每一个$w\in X$，there is a formula $\varphi$使得$M, w\not\vDash\varphi$.】
2. 公式集有限可满足：公式集的每个有限子集是X可满足的。

模态饱和模型（类）：对于任意的模态公式集合$\Sigma$，对于任意w的后继集合X，(*)如果$\Sigma$在X是有限可满足的、那么X是可满足的。【条件也可以写作：如果$\Sigma$在某个w的后继集合X是不可满足的，那么有一个$\Sigma$的有限集合是X不可满足的。】

m-sat也是H-M model class. 【是最大的H-M models吗？】【是不是不是... 好像没有说。】

证明：和image-finite模型类上的证明类似，我们想要证明在m-sat模型类上，模态等价关系可以推出bisimulation. 对于模态等值的(w, v)，赋值维持仍然是好证的，证明关系维持（如果w -> w'，There is a v' 使得 v -> v'，且w' 与 v'模态等值），我们仍然反证，假设不能维持，收集v的每一个后继中特殊的$\varphi_{w', v'}$，它在M, w'上为真、在N, v'上为假。收集到$\Sigma$中。

因为N是m-sat的，我们可以找到一个$\Sigma$的有限子集$\Sigma'$，在v上的任意后继都不满足。此时，$\Diamond\wedge\Sigma'$在M, w为真、在N, v为假，这与两者模态等值的前提矛盾。

## what logic exactly corresponding to bisimilarity?

支持无限交的模态逻辑！如果模态逻辑可以无限与，逻辑的区分能力更强，便可以与bisimilarity匹配了。可能现在的模态逻辑的语义中，后继是可以任意的无限的，与运算却是有限的，会有不匹配。

