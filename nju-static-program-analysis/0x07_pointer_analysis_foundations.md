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