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

https://www.bilibili.com/video/BV1gg4y1z78p/?spm_id_from=333.788.recommend_more_video.0

https://spa-book.pblo.gq/ch3/pointer-analysis/03-01-pointer-analysis-spa