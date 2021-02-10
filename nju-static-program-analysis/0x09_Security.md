# 10. Security


## Information flow security(Read protection)

安全性很重要，我们可以用静态分析计数关注数据流。

> Prevent unwanted information flow to protect information security.

考量数据安全性的两点：
* Access-Control，正确的权限系统
* Information flow security，与预期相符的数据流

信息流：变量x的信息如果可以迁移到变量y，那么我们有信息流x->y. 类似指针分析.

分析之前，我们要确定业务的安全预期：
1. 设定安全类别，将变量归类
2. 设定安全类别间数据流动的协议（操作权限（多种操作）；可能不是线性序，是一个偏序）

协议：
1. Non-interference
    ![](./pics/09-01.png)
   
## Confidentiality and Integrity(Write protection)

类似的其实.

## Explicit Flows and Covert Channels

前面讨论的read protection，是显式的。但私密信息还可能影响控制流！（类似meltdown...信息由控制变化产生的未清除的side effect泄露）

![](./pics/09-02.png)

Channel: 传递计算系统信息的机制，被称为channel（Mechanisms for signalling information through a computing system are known as channels）
Covert Channels: 当一个Channel的主要功能不是做信息传输时，它被称为隐藏信道。

implicit flow(控制流呈现的信息) / termination / timing / exception...

implicit flow传递的信息是少的。所以我们先不关注（

## Taint Analysis(Algorithm)

污点分析：检测污点数据（tainted data）的数据流、并判断它们是否流向我们关注的位置（sinks, usually some sensitive methods）. 创建污点数据的地方，称为source.

比如下图中，检测用户输入的数据是否可能作为指令参与执行（应对注入攻击）。

![](./pics/09-03.png)

Taint Analysis和指针分析是对应的。
* tainted data as objects
* sources as allocation sites

算法的输出是`a set of pairs of tainted data and sink methods`. TaintFlows. 实际上是在做指针分析的同时，顺便维护TaintFlows.

Rules:

* Handles sources. 创建tainted data的函数，特殊处理，视为“new”.
* Handles sinks: 如果有指向tainted data的指针，作为参数传入某个sink function，记录.