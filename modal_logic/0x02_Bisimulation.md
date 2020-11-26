# 0x02 Bisimulation

模态语言是描述点模型的，一个自然的问题是，模态语言可以区分何种点模型？不能区分哪些？

但确定点模型时，需要先确定模型。

## 同构，isomorphism

如果两个模型同构，模态逻辑无法区分两者中对应的两个点模型（$M,w\equiv_{\texttt{ML}}N, v: for\;each\;\varphi\in\texttt{ML}，M, w\vDash\varphi\Leftrightarrow N, v\vDash\varphi$）。

定义：存在两个模型论域之间的一个双射，使得对应点的赋值一致、可及关系也被保持。

赋值：For any proposition letter $p \in P:$ $p\in V_M(w)\iff p\in V_N((f(w)))$

可及关系：For all w, w' $\in$ W_M: (w, w') $\in$ R_M $\iff$ $(f(w), f(w'))\in R_N$

点模型对应：f(w) = v

以上三者记为 $M, w \cong N, v$.

### 证明：结构归纳. 

对于w和v = f(w)，根据同态的赋值定义，V(w) = V(v).

> 啊啊啊啊啊归纳假设是啥？？？
> **对于所有同构的子模型，子公式模态等值。**
> 相当于$If\;M,w\cong N, v\;then\;M,w\equiv_{ML}N,v$
> 
> 相当于证明 $\forall\varphi\in\texttt{ML}, M,w\cong N,v \rightarrow M,w\vDash\varphi\Leftrightarrow N,v\vDash\varphi$
>
> 对$\varphi$做induction。

对于其他式子不断使用归纳假设（假设子表达式（$\varphi, \psi$）已经满足）. $M, w\vDash\neg\varphi\Leftrightarrow M,w\not\vDash\varphi（\texttt{语义}）\Leftrightarrow N,v\not\vDash\varphi（归纳假设）\Leftrightarrow N,v\not\vDash\varphi（语义）$.

$M,w\vDash\Diamond\varphi$ （要证两个方向，先是左）根据语义，存在$w'$，$\langle w,w'\rangle\in R_M$满足 
$M,w'\vDash\varphi$, 根据归纳假设$v' = (f(w'))$满足
$N,v'\vDash\varphi$，根据f对可及关系的要求，v=f(w)与v'=f(w')一定有可及关系，即$\exists v', \langle v, v'\rangle\in R_N，满足\varphi$，即
$N, v\vDash\Diamond\varphi$.（然后证从右向左，需要用到满射的性质）。【需要用到满射，本质是因为函数是有方向的。】





> 一个逻辑$L$在某结构关系$\rightarrow_S$下被保持（是不变的, invariant），当$M,w\rightarrow_S N,v$ implies $M, w \equiv_L N, v$.
> 
> "L is invariant under $\rightarrow_S$."
> 
> 模态逻辑在同构关系下是不变的。

## 满射强同态

只是把同态的双射改为了满射而已。

