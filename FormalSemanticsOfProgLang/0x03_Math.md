# 0x03 一点数学

## Symbol

一些符号的latex标识: 自然数，整数，bool

$$

\mathbb{N}\;\mathbb{Z}\;\mathbb{B}.

$$

## Sets

Generalized集合符号，Generalized Unions/Intersections of Sets.

不会证明呀.. $\exists$要怎么建立和$or$的关系. 是否可以destruct $\{A, B\}$


$$
\bigcup_{i=m}^{n} S(i)  \\
$$

注意$\bigcap \emptyset$无意义，set of everything. “x属于空集中的每个集合”对于任意元素x都成立。

$$
\bigcap S = \{x\;|\;\forall T \in S. \; x \in T.\}
$$

## Relations

1. 定义1，笛卡尔积，Projections over pairs $\pi_{0/1}(x,y)$.
2. $\rho$ is a relation from A to B if $\rho \sube A \times B$, or written as $\rho \sube \mathcal{P}(A \times B)$
3. $\rho$ is a relation on S if $\rho \sube S \times S$
4. $x\;\rho\;y$
5. $rho$ is an identity relation: $\forall (x,y) \in \rho.\; x=y$. 没有要求作用域值域.  

1. identity of S
2. domain, range 两者都是“全”集，不大不小 
3. composition of $\rho$ and $\rho'$ : $\rho' \circ \rho = \{(x,z)\;|\;\exists y.(x, y) \in \rho\;\wedge\;(y,z)\in \rho')\}$. 符号是“从右向左”的，第一个输入在最右侧.. 和 $x\;\rho\;y$的不完全一致.（和写函数的方式类似）
4. inverse

> [math font I can use](https://tex.stackexchange.com/questions/58098/what-are-all-the-font-styles-i-can-use-in-math-mode) 

属性：
1. composition的结合律
2. id是composition的左右“幺元”
3. composition与逆的关系
4. 与空关系compose，得到空
5. $dom(\rho) = \emptyset \Leftrightarrow \rho = \emptyset$. 问题：$dom$ 换成 $ran$ 也成立吗？
   
自反 + 对称 + 传递 = 等价

## 函数

是特殊的关系：同一个自变量，因变量唯一

$(g\circ f)\,x = g(f\;x)$

单射函数的反函数也是函数

functions denoted by typed lambda expressions: $\lambda x \in S.E$ denotes the function f with domain S such that $f(x) = E$ for all $x \in S$. 匿名函数.

variation of a functions: 让函数在输入x下的值变为n，x可能不在原本f的定义域下(此时相当于定义域扩大了一项).

$$

f\{x \rightsquigarrow n\} 

$$

Function Types, right associative $A \rightarrow B \rightarrow C = A \rightarrow (B \rightarrow C)$.

Currying.

## Products