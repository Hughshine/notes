# 6. Pointer Analysis

> 一下子复杂好多好多QAQ！

## Motivation

virtual call 有多个candidate，有的时候的确每一种都是可能的，
但有可能一定只选一个。我们追踪变量到class的points-to关系，
看一看“是不是常量”。（是不是有些类似variable class propagation呢...）但这里是指针，有传递关系，所以更复杂。

## Introduction to Pointer Analysis

regraded as a may-analysis.

第一篇：popl 1980, William, interprocedural data flow analysis in the presence of pointers, procedure variables, and label variables. 

Pointer analysis and Alias analysis: 后者可以由前者得到

Applications: 

* 更好的call graph, aliases...
* 可以做编译器优化：virtual call inlining...
* bug detection: null pointer detection...
* security analysis: information flow analysis...
* ...

总之非常重要QwQ...

## Key factors of pointer analysis

![](./pics/06-01.png)

### heap abstraction

静态分析要处理有限的对象（保证termination），内存模型可以无穷大，需要进行抽象。

store based/storeless/hybrid. 

allocation-site abstraction 属于 store based，目前最常用。

> one abstract object per allocation site.

### Context sensitivity

* context-sensitive: 区分分析不同context的相同method。很有用。
* context-insensitive: 只分析每个method一次。lose imprecision.

### (control) flow sensitivity

* flow-sensitive: 考察控制流，maintain a map of points--to relations at each program location. （所有data-flow analyses都是流敏感的）【在Java中，似乎没有“证据”说明flow sensitive更好】
* flow-insensitive: 忽视控制流，只维持一个points-to relations for the whole program. (相当于over-approx一下，只向任何执行点可能指向的对象)。【这节课用flow-insensitive】

> 是否context-sensitive已经涵盖了flow-sensitive一部分信息？

### analysis scope

分析程序的哪些部分：
1. 所有指针的指向关系【选择这种】
2. 需求驱动，只分析想要考察的指针... 【是也可能考察许多其他指针的... 多次运行，效率可能会低】

## Concerned statement

我们只关注pointer-affecting statements.

> local variable; static field(有时当作global variable); instance field; array element(array[i]).
> 
> 数组会忽略index——建模成只有一个single field的object【也就是当作instance field】.
>
> 所以只考察local variable和instance field即可。

* new: `x = new T()`
* assign: `x = y`
* store: `x.f = y`
* load: `y = x.f`
* call: `r = x.k(a, ...)`

> 三地址码会为复杂的memory-accesses引入临时变量
> 
> call只研究virtual call，因为最复杂。


*** 

https://www.bilibili.com/video/BV1gg4y1z78p/?spm_id_from=333.788.recommend_more_video.0

https://spa-book.pblo.gq/ch3/pointer-analysis/03-01-pointer-analysis-spa