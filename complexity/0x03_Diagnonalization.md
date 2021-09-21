# 0x03 对角化

对角化技术是有一种常用的证明技术，它最初被用来反证自然数与有理数势的不同

对角化技术在复杂度理论领域有一些成果，这一节举例进行了介绍。
1. 存在不可计算的函数
2. 证明hierarchy theorems（提供更多资源，可以做更多事情）
3. 证明Ladner theorem：若 p != NP, 那么有不是P也不是NPC的问题.

对角化也有局限，比如确定P和NP的关系。这种局限也用了对角化技术本身证明了。

> 是很重要的基础证明技术。

在复杂度理论中，对角化依赖配合这些事实：（*）
1. 图灵机可以用字符串表示
2. 用图灵机去模拟图灵机，overhead很小

***

构造不可计算的函数UC：
1. 一张表，横纵为01串的字典序，无限长度，所有的图灵机都在其中了。
2. 每一个格子(a, x)的含义是M_a(x)的值，当a不表示图灵机或者not halt时填入*。利用该表格的对角线可构造一个新的函数UC，UC在输入x时，取值为flip(M_x(x))。可知UC与每一个图灵机所对应的函数都在一个输入上不同。所以UC便不可表示为图灵机。

*** 

Time Hierarchy Theorem: 两个（time-construcible）函数f, g，满足f(n)log(f(n))=o(g(n))，那么Time(f(n))$\subset$Time(g(n)).

> time-constructible functions，时间复杂度不超过自己的函数。排除一些特殊的函数。
> 用log是因为模拟的界

证明思路：
1. 构造一个图灵机D，它用g(n)的时间模拟所有的f(n)时间的图灵机（D(x) = flip(M_x(x))）
2. D在Time(g(n))而不在Time(f(n))

***

Ladner's Theorem:如果$NP\not=P$，那么存在一个NP语言，不是P，也不是NPC.

> padding: 通过为输入增加无用的更高数量长度，使得其相对复杂度降低


为SAT构造一个padding（有些复杂），证明SAT_H不在P也不在NPC中。

> TODO: 证明细节

***

oracle turing machine: 将一种问题（oracle）的复杂度视作一步的机器。

cook reduction: $L < L' \Leftrightarrow L \in P^{L'}$. 另一种形式的reduction定义：将更复杂的问题L'当作oracle，如果此时可以在多项式时间计算L，那么我们说L is cook reduction to L'.

> 一些示例和证明

***

不能用对角线原则确定P与NP的关系：我们讨论的大多数证明，都依赖于*中两个事实。基于这两个事实得到的结论，我们成为relativizing results. P与NP的关系问题，不是relativizing的。Oracle turing machine也基于这两个事实。

Theorem：对于某个oracle A，它可能使$P^A = NP^A$，也可能使$P^B \not= NP^B$.

> Proof: TODO...