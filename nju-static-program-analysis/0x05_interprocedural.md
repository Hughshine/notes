# Interprocedural analysis

## motivation

不同函数有自己的函数内控制流图，函数之间会相互调用。
被调用函数的形参近以靠对该函数的分析，是得不到任何运行时信息的，
因而当代码存在函数调用，只能将其当作黑箱，丧失所有准确率。

我们希望进行interprocedural analysis. 为了可以做到它，我们要建立call graph,
—— 增加 call edges 和 return edges.

## call graph construction(CHA)

call graph: a set of call edges from call-sites to their target methods (callees)

> call graph 是十分重要的程序信息，用于程序优化、理解、调式、测试、...

四种call graph construction（对于面向对象语言的，OOPL），从上到下更精确、效率更低:
1. **class hierarchy analysis (CHA)**
2. rapid type analysis (RTA)
3. variable type analysis (VTA)
4. **pointer analysis (k-CFA)**

相关ir：
* `invokestatic`，确定的static methods
* `invokespecial`，contructors/private instance methods/superclass instance methods, 某一个
* `invokeinterface` & `invokevirtual`，其他的instance methods -- 因为多态，所以可能有多个target method. 这一项对于面向对象语言的静态分析，很重要。

*virtual* calls `o.foo(...)` 如何 dispatch？
* 根据object的类型（pointed by o）
* 函数签名（the identifier of a method: class type + method name + descriptor(return type + parameter types)）【要保持注意，我们是在OOPL语境下讨论的问题】【`<C: T foo(P,Q,R)>`】

定义 dispatch 过程：（注意，只关注m的name and descriptor，不关注m的“类”（这个类是静态的，dyn dispatch正是调整了它））

![](./pics/05-01.png)

### class hierarchy analysis

> ECOOP 1995, Jeffrey Dean, Optimization of Object-Oriented Programs Using Static Class Hierarchy Analysis

```java
A a = ...
a.foo()
```

resolve a virtual call based on the declared type of receiver variable of the call site —— assume the receiver variable `a` may point to objects of class `A` or all subclasses of `A` —— resolve by class hierarchy of class A.

注意，class heirarchy是一棵树。

![](./pics/05-02.png)

缺点：完全忽略了数据流、控制流信息，仅依赖inheritance hierarchy（是完全静态的），不够准。但是很快。

![](./pics/05-03.png)

### call graph construction

build call graph for whole program, via CHA: 
start from entry methods(`main`), 
for each reachable method m, resolve target methods for each call site in m via CHA..... 
repeat until no new method is discovered.

> 一次图遍历

![](./pics/05-04.png)

配一个example：

![](./pics/05-05.png)

## interprocedural control-flow graph (ICFG)

ICFG包含：每一个函数的CFG，和额外两类边，call edges & return edges.

![](./pics/05-06.png)

## interprocedural data-flow analysis

transfer functions: node transfer + edge transfer!

* call edge transfer: transfer data flow from call node to the entry node of callee
* return edge transfer: transfer data flow from return node of the callee to the return site
* node transfer: 额外删去LHS variable（kill!），它的值会通过return transfer返回

额外有一类call-to-return edge，用来transfer local data-flow. call/return edge transfer 只传参数和返回值，不传其他局部变量。

![](pics/05-07.png)

> 还是很好理解的！注意对于virtual method, 需要meet多条return edge.

edge 的指向是控制流，edge的值（转移函数）预示着数据流的分化。结点的转移表征程序的语义。在跨过程分析时，method call复制掉过程参数，并在调用返回时将承载返回值的变量数据流强行接入。

edge transfer 在其他地方也可有用，比如switch可以对每个case增加edge transfer，每个case只在对应分支为确定值（对数据流的约束）时过去，“restrictTo”。