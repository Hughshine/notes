# 7. Pointer-Analysis: Foundation


## Pointer-Analysis: Rules

Domain: variable, field, object, instance fields, pointers, points-to relations

![](./pics/07-01.png)

Rules: (注意，o的下标是“行号”. allocation site的标识.)

![](./pics/07-02.png)

对于store：x.f = y —— x所有可能的o的f field，可以指向y的任何指向对象。

对于load：y = x.f, x的所有o的f field的可能对象，都加入y.

## How to implement pointer analysis?

- 我们实际要做的，是propagate points-to information among pointers(variables & fields). （solving a system of inclusion constraints for pointers, Anderseb-style analysis）
- 如何实现：当`pt(x)`改变，向所有有关指针传播更改的部分。我们需要一个图（Pointer Flow Graph, PFG），去连结有关指针，在`pt(x)`改变时，传播更改至x的后继。【有向图】

图的节点是Pointer（变量+域），边是指针到指针的may-flow-to关系。—— 计算PFG的传递闭包。

build point flow graph 与 propagate points-to information on PFG，是相互依赖的。

## Pointer Analysis: Algorithm

AddEdge(s, t): `t = s -- AddEdge(s, t)`, 增加一条右部指向左部的边。PFG中的边的方向，表示数据可能的流向：`e is reachable from b on the PFG, which means that objects pointed by b, may flow to and also be pointed by e.` PFG中被指向的指针，可能指向更多的objects。

> 加入worklist中的entry<target, pt(s)>, 是等待被广播的项。

Propagate(n, pts): 是沿着指针图propagate 指针指向的objects集合的信息. 迭代过程，维护pt(n)是闭包（所有可能指向的**对象**）。注意，PFG中的边，不是扁平化的。【每次迭代同时维护pt(s)和PFG！！】

每一次propagate之后，针对store/load，更新PFG。

> 要注意，两种指针，分别是变量和object.field，object是确定的（指内存中那片确定的数据）！obj.field是不可能在代码中出现的。obj只能用变量替代。

在主函数中，`pts-pt(n)`，含义是：已经可能流向n的，不需要被再次传播啦，for effiency。【除此之外，对于stores/loads/method calls的效率，它也很重要，TODO】

## Pointer Analysis with Methods calls

pointer analysis就是为了做更准确的call graph的. 构建过程是on-the-fly.

也就是，对于call statement，我们也有一个rule.

它做这样的事情：设置函数形参、返回对象和`this`的可能对象集合，增加需求的PFG Edge(实参->形参，ret->r). 注意的是，没有边指向`this`, 因为没有this=x这种隐藏语义，我们会通过dispatch(x.k)找到相应函数的this，父级类，不会有孩子的类的可能对象。

![](./pics/07-03.png)

指针分析与call graph construction也是彼此依赖的，指针分析只分析call graph关于main的连通子图。

此时，原有的指针分析算法有被拓展：需要逐步分析更多的method（stmts），增量式. 

`ProcessCall(x, oi)`做的事情：传入的参数x多了一个可能指向的对象oi，所有的关于x的调用，为了重建call graph，关于x的方法调用都要update、增量分析一次。
1. 首先找到实际调用的函数`m`
2. 根据call rule设计算法：处理m_this（放入worklist等待propagate），指针图中，增加实参到形参的边，返回值的边。CG中增加cs到该函数的边（只这一部分关于CG）。同时被调用的函数要考虑是否要被继续分析，加入worklist.

![](./pics/07-04.png)

> 真正的深入理解，需要做好实验..