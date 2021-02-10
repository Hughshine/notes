# Datalog-Based Program Analysis

Imperative Implementation: 命令式编程太复杂了！想用声明式编程写指针分析程序。

![](./pics/10-01.png)

## Introduction to Datalog

Datalog = Data + Logic
* No side effects
* No control flows
* No functions
* Not Turing-Complete

要素：
* `Predicates(relation)`: a set of statements, a table of data.
* `fact`: a particular tuple, belongs to a relation(table). —— 需要是正确的.
* `Atoms`: `P(X1, X2, ..., Xn)`. Represent predicates.
* `terms`: Variables, stand for any values; constants.

`P(X1, X2, ..., Xn)` 被称为relational atom，如果P的确含有tuple(X1, ..., Xn), Atom被evaluated为正确.

还有arithmetic atoms，如 `age >= 18`.

Datalog Rules: (Logic), head(consequent) is true if body(antecedent) is true. `,`可以理解为and. 

![](./pics/10-02.png)

程序对规则的解读：
1. 找到所有可能的变量值组合in the subgoal
2. 如果一个组合使得all subgoals true, 那么得到一个head atom with corresponding values
3. The head predicate包含所有正确的值.

我们区分两类Predicate:
1. EDB(extensional database): defined in a priori.
2. IDB(intensional database): established only by rules.

`Or`: 两种方式
1. Write multiple rules with the same head.
2. Use `;`. 它的优先级比`,`低.

`Negation`: `!`.

`Recursion`: 如果没有recursion，datalog就是sql.

![](./pics/10-03.png)

`Rule Safety`: 每一个变量都出现于至少一个non-negated relational atom中，一个规则才是safe的. 否则会出现infinite relation. 只有safe rules才被datalog允许。

`Recursion and Negation`: 不允许理发师悖论. 即不允许`A(x) <- B(x), !A(x)`式规则. diverge.

DataLog Engine: 

![](./pics/10-04.png)

## Pointer Analysis thru datalog

直接按照形式化的rule去写datalog的rule！

![](./pics/10-05.png)

Call:

![](./pics/10-06.png)

## Taint Analysis via datalog

![](./pics/10-07.png)

Datalog based Program Analysis缺点：有一些逻辑无法表达（可能我们有更复杂的分析目标、抽象手段），并不能准确控制performance.