# Lambda Calculus

1. functions of rules, 关注 how the function is to be calculated. 此时一般不需要了解函数的domain/codomain. 很多时候，我们也更关注这部分的中间过程。
2. untyped lambda-calculi 由于完全无类型，有些运算无意义，如无限递归，$\omega = \lambda x.x(x)$
3. 与turing machine / general recursive functions 在可计算性上等价；是simplest possible PL that is Turing complete.
4. Lambda Calculus 与 建构主义逻辑 紧密相连，Proofs as Programs. 建构主义指，一个证明必须是一个`construction`, 也就是一个program. 
5. 将lambda calculus 应用于数学模型之上，可以更好理解其意义（如代数，偏序集、拓扑学、范畴学...）

## Untyped Lambda

### overview 

1. definition: variables / applications / lambda abstractions
2. 括号太多，所以有括号convention
3. free \& bound variables, $\alpha$-equivalence. 替换规则的第一步（rename）. rename的递归定义；许多数学领域都有free/bound variables的概念，如微积分符号，lim，$\sum$，c程序的函数.
4. 替换（substitution），注意有时需要rename. 
5. $\beta$-reduction. A lambda term without any $\beta$-redexes(reductive expression, $(\lambda x.M)N$) is said to be in $\beta$-normal form. 要替换. 概念：evaluates to (normal form). Not every term evaluates to somthing(因为无线递归结构). 注意，TODO，课件上的reduction与evaluation有区别. $\beta$-equivalence.

### Programming in untyped lambda calculus

1. 表示Bool与基本运算/自然数/Pair/Tuple/List/trees.
2. 基于List之上可以表示新的自然数，比直接基于lambda-calculi的更有效（$\beta$-reduction次数更小）
3. 解Recursive Function：Fixpoints. *In the untyped lambda calculus, every term F has a fixpoint*, 所以可以解递归方程.

课件上：

Reduction Strategy & Evalutation Stratygy

1. Reduction Strategies: normal-order, 最左侧，最外侧的redex先reduce. 这个方式一定会找到normal form, if exists. (走必须经过的路径) ；Applicative-order reduction, 最左侧，最内部的redex优先，此时是可能陷入死循环的.
2. Evalutation Stratygies: 什么时候 evaluate the arguments of a function call, and what kind of value to pass to the function. 

> 课件上强调reduction与evaluation的区别，此处没有理解

20.8.11记：Reduction 与 Evaluation的区别应该就是：reduction是一直reduce到normal form为止，normal form相当于原先函数的最简化，allow optimizations, 可能会遇到无限的场景，最后可能得到一个最简单的lambda abstraction（所谓的“函数”）, 可能只是一个变量（不再有“栈”）。Evaluation是到一个lambda abstraction($\lambda x.M$)就停下（感觉可以理解为，运行时最终还是在栈上...），only evaluate closed terms(no free variables) ，因为嵌套地进行evaluate的是“被固定的参数”。

几个练习：Find lambda terms then represent each of the following functions.

1. f(n) = iseven(). 但是无类型， = 也没有定义，似乎不对. 希望找到一个函数F, F(x) = y, F(y) = x. 事实证明，思维被iszero固化了. iszero $\lambda n\;x\;y.\;n\;F\;T$ 的形式，输入自然数后效果是将F应用m次，由于iszero太简单，所以直接在x,y bound的情况下，设计F就可以. 如果iseven也以这种形式设计，必然需要做等于判断，因为x是bound的. 但显然，我们只需要不断翻转true/false就可以. 很轻松就可以跳出这个思维框架.

$$

\lambda n\,x\,y.\, n \; (\lambda z. \;if\_then\_else\; (z = x)\; y\; x)\; x (不好)\\
iseven := \lambda n.\;n\;(not)\;T
$$

2. $f(n) = (n+3)^2$, 只需要把add/mult写进去就好了，最多可以reduce，但会不够直观.

$$

\lambda n.\;mult\;(add\;n\;3)\;(add\;n\;3)

$$

3. $exp(n,m) = n^m$ 只需要将(mult n)重复m次嘛。

$$
 
\lambda n,m.\; m (mult\;n)\;\overline{1}.

$$

4. $pred(n) = n - 1$ reduction rule只允许我们将一个term替换为另一个term，不过term不能为空. 所以我们必须用全新的思路构造pred: 生成的思路. 我们从(0, 0)开始找，fst为前面的数，snd为当前的数，一旦当前的数等于n，我们返回fst. (当我们记不住某个英文字符的前一个是什么，就要重头背一遍...). 我们需要一个结构存储两个数，并且做n次转移.

> [参考](http://gettingsharper.de/2012/08/30/lambda-calculus-subtraction-is-hard/)

$$

Pred := \lambda. (n\; Trans\;(MkPair\;\overline{0}\;\overline{0})) Fst\\
Trans := \lambda p.\;MkPair\;(p\;Snd)\;(Succ(p\;Snd))\\
MkPair := \lambda ab.\;\lambda s.\;(sab)\\
Fst := \lambda ab. a\\
Snd := \lambda ab. b

$$

基于此，我们可以定义减法。

### fixpoint

当我们将递归方程写成fixpoint equation形式时，我们就已经把约束全部融入这个等式$f = F\;f$之中了. 我们要求的解，就是F的fixpoint，且任何一个都可以. 也就是说，我们用任何一个combinator求解，也都可以.

解方程时，完全绕过了fixpoint的真实表达式，就是在不断反复用F规约. 每次单独遇到fixpoint，都根据$\Theta F\twoheadrightarrow F(\Theta F)$替换.即，我们无法直接reduce fixpoint到底（因为没有底），我们只能一步一步解析输入参数、并用上述等式替换，直到到达递归终点（如果函数被正确定义了的话）.

> $\Theta = AA\\A = (\lambda xy.y(xxy))$ .

### 其他数据结构：Pair/Tuple/List/Tree

#### Pairs

Pairs: $<M, N>$ => $\lambda z. z M N.$ 有一个参数，是“选择子”.

Left: $\lambda p. p(\lambda x y.x).$

Right: $\lambda p. p(\lambda x y.y).$
 
#### Tuples -- Extension of Pairs

$\langle M_1, ...,M_n\rangle$ = $\lambda z. zM_1...M_n$.

$\pi_i^n = \lambda p.p(\lambda x_1...x_n.x_i)$.  是Left/Right的拓展.

#### List

$nil = \lambda xy.y$

$H::T = \lambda xy.xHT$.注意，这里的H/T不是参数，就是一个连接链表的语法糖.

熟悉一下list的（递归）方程$addlist\; l=l(\lambda h\;t.add\;h(addlist\;t))(\overline{0})$. 它很像foldr...，l接受的第一个参数是如何accumulate H 与 T的解，l的第二个参数是到达nil了，我们的递归终点，此时返回值是什么.

#### Trees

$leaf(n) = \lambda xy.xn$  
$node(L,R) = \lambda xy.yLR$

这里的定义，内部节点没有value.

熟悉一个函数：$addtree\;t=t(\lambda n.n)(\lambda l\;r.add(addtree\;l)(addtree\;r)).$ leaf和node都接受两个参数，但是，leaf只使用第一个，node只使用第二个.


#### 练习

> 都需要用一般的fixpoint处理方式简单处理一下

1. $len\_of\_list\;l = l(\lambda ht.\;succ(len\_of\_list\;t))\;\overline{0}$
2. $depth\_of\_tree \;t=t(\lambda n.\overline{0})(\lambda lr.succ\;(\;max\;(depth\_of\_tree\;l)\;(depth\_of\_tree\;r))$

> 差两个，头脑迟钝，以后再想.（max a b，sort l）

## The Church-Rosser Theorem