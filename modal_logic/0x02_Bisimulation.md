# 0x02 Bisimulation

模态语言是描述点模型的，一个自然的问题是，模态语言可以区分何种点模型？不能区分哪些？

但确定点模型时，需要先确定模型。

## 同构，isomorphism

如果两个模型同构（借助函数定义），模态逻辑无法区分两者中对应的两个点（由函数关联）模型（即两个点模型的模态逻辑等值）（$M,w\equiv_{\texttt{ML}}N, v: for\;each\;\varphi\in\texttt{ML}，M, w\vDash\varphi\Leftrightarrow N, v\vDash\varphi$）。

定义：存在两个模型论域之间的一个双射（映射W），这个函数可以保持对应点的赋值一致、可及关系也被保持（保证R与V）。

对这个双射函数$f$性质的形式化描述：

赋值（V）：For any proposition letter $p \in P:$ $p\in V_M(w)\iff p\in V_N(f(w))$

可及关系（R）：For all w, w' $\in$ W_M: (w, w') $\in$ R_M $\iff$ $(f(w), f(w'))\in R_N$

那么，如果对应的点模型满足这个式子：f(w) = v，那么$M, w \cong N, v$.

### 证明：结构归纳. 

对于w和v = f(w)，根据同态的赋值定义，V(w) = V(v).

> 啊啊啊啊啊归纳假设是啥？？？
> **对于所有同构的点模型，子公式模态等值。**（或者说：如果两个点模型有同构关系，则其上的任何子公式模态等值）
> 相当于$If\;M,w\cong N, v\;then\;M,w\equiv_{ML}N,v$
> 
> 相当于证明 $\forall\varphi\in\texttt{ML}, (M,w\cong N,v) \rightarrow (M,w\vDash\varphi\Leftrightarrow N,v\vDash\varphi)$ 量词的位置倒是无所谓的，我们把它提到前面，然后便可以做induction。
>
> 我们是对$\varphi$做induction。


对于其他式子不断使用归纳假设（假设子表达式（比如$\varphi, \psi$）已经满足$M,w\vDash\varphi\Leftrightarrow N,v\vDash\varphi$）. $M, w\vDash\neg\varphi\Leftrightarrow M,w\not\vDash\varphi（根据语义将\neg提到\vDash之外）\Leftrightarrow N,v\not\vDash\varphi（归纳假设）\Leftrightarrow N,v\vDash\neg\varphi（再根据语义转换回来）$. 也就是，将语法转换至语义、整理出归纳假设（是子表达式语义的等价性！）、然后归约回语法。前几个都很简单，直接就能得到双向的，不用分成两个方向单独证明。

$M,w\vDash\Diamond\varphi$ （要证两个方向（主要是因为$\Diamond$要考虑$R$，$R$的形式化限定用到函数，函数是有方向的（$w, f(w)$），而没有（$f^{-1}(v), v$），不得不分开。），先是左）根据语义，存在$w'$，$\langle w,w'\rangle\in R_M$满足 
$M,w'\vDash\varphi$, 根据归纳假设$v' = (f(w'))$满足
$N,v'\vDash\varphi$，根据f对可及关系的要求，v=f(w)与v'=f(w')一定有可及关系，即$\exists v', \langle v, v'\rangle\in R_N，满足\varphi$，即
$N, v\vDash\Diamond\varphi$.（然后证从右向左，需要用到满射的性质（得到：每个v都能找到一个w，f(w) = v））。【需要用到满射，本质是因为函数是有方向的。】





> 一个逻辑$L$在某结构关系$\rightarrow_S$下被保持（是不变的, invariant），当$M,w\rightarrow_S N,v$ implies $M, w \equiv_L N, v$.
> 
> "L is invariant under $\rightarrow_S$."
> 
> 模态逻辑在同构关系下是不变的。

## 满射强同态

> 满射强同态可以做相同子结构的消除。

只是把同构中可及关系的双射改为了满射而已。（左至右的）单射的性质不重要。

## 有界态射

赋值被保持：$p\in V_{M}(w)\Leftrightarrow p\in V_N(f(w))$.


可及关系（左至右）：For all $w, w'\in W_M$: if $(w, w')\in R_M$ then $(f(w), f(w'))\in R_N$.

【对满射进一步弱化】弱化了什么：可及条件约束，是希望通过v'找到f^{-1}(v') = w'. 但并不是所有v'都需要，比如没有前驱的结点就不必映射。【因而不需要满射】

可及关系（右至左）：For all $w\in W_M$: if $(f(w), v')\in R_N$ for some $v' \in W_N$ then there is a $w' \in W_M$ such that $f(w') = v'$ and $(w, w')\in R_M$.

值得注意的是，这里我们讨论的是“模态逻辑在点模型上的区分能力”，而映射是对完整论域的映射——很多和这个点模型完全没关系的世界，其实想怎么映射都可以的！有界态射便是在做这样一个尝试，把视角收回到点世界本身。

右至左的可及关系，就是为了从$f(w)=v$，且$(v, v')\in R_N$，得到存在$(w, w')\in R_M$.

（我目前的看法）“在两个宇宙中，两个有关系的（被函数f联系的）点世界能看到的可能世界是一样的。”，可以忽略处于W世界中的点的后面的世界。——与bisimulation有什么区别（不能杂糅“单射”和“双射”）？

## Bisimulation

使用relation定义。relation中的每个二元组$(w, v)$，满足：

赋值一致：For any $p\in P$: $p\in V_M(w)\Leftrightarrow p\in V_N(v).$

可及关系（左至右）：if $w R_M w'$ then there exists a $v'\in W_N$ such that $v R_N v'$ and $w'Zv'$.

可及关系（右至左）：if $v R_N v'$ then there exists a $w'\in W_M$ such that $wR_M w'$ and $w'Zv'$.

作为关系，也就是说一些世界、relation可以不被映射（但是如果要是映射了，就要把相关的都映射过去），或者（将某一个世界、关系）映射为很多个。

Bisimilar: $(M, w)\underline{\leftrightarrow}(N, v)$: 两个模型间存在bisimulation Z，且$(w, v)\in Z$.（可以“杂糅单射和双射”。）

> a bisimulation Z is total: 某个模型中的每个世界都在Z中出现。

### bisimilarity的一些特点

1. 某个模型和其自身的bisimulation，不是等价关系（或者说是“划分”更好？【不清楚】）（不能将一些可能世界根据某种等价性联系到一起）。它甚至可以考察某个模型的某个，和这个模型另一个局部的关系，完全和等价关系不沾边。

2. （某两个模型间的）bisimulation对并（union）封闭，所有bisimulations的union，是greatest bisimulation.【相当于把有关联的分支不断叠加起来】（注意，这一点是很重要的：我们探讨两个模型的bisimulation时，是探讨最大bisimulation的一个子集）

> 交集，Intersection

3. Bisimilarity 是点模型间的等价关系。【自反，对称，传递】

4. Bisimilarity within models $\{(w,v)\;|\;M,w \underline{\leftrightarrow} N,v\}$ 可以被看作最大bisimulation（任意两个点世界若可以有bisimilarity关系，就放入这个集合中）. 如果（$N = M$），它可以被视为M上的等价关系。

In practice，我们经常用bisimulation talk about bisimilarity.

两个模型间具有bisimulation，不意味着任意模型中任意可能世界，在另一个模型中都有对应的点。（对点世界的操作，保持“bisimularity”，意味着之前有关系、之后还是有，之前没有、之后也没有）【TODO】
