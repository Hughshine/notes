# [Intro](https://cs.nju.edu.cn/hongjin/teaching/semantics/)

Computer science is no more about computers than astronomy is about telescopes.  
计算机只是一个工具，类似天文望远镜，很朴素的事物，但是通过它呈现给人类的（computer science）却绚烂、深刻、丰富，正如基于望远镜研究的天文学。

那么值得进一步探讨的，程序语言研究与计算机科学的关系：通过语言看待各类计算、各类软件行为，常常是从寻找一种更普遍的通用的本质的视角看待它们（通过developing general abstractions），从复杂（的应用表象）中提取本质。这种视角仍然保持数学的严格性，PL 研究者常常研究程序所保证的性质，这是很有价值的。PL作为一种视角、方法，与其他领域计算机子领域结合，是好的。[blog](http://www.pl-enthusiast.net/2015/05/27/what-is-pl-research-and-how-is-it-useful/)

形式语义这门课，是一门数学语言课，这个数学语言用于讨论“语言”、“编程”，在一些有通用性的抽象层次上研究它们的性质（都是数学描述了的，形式化了的）。要建立对这门数学语言的读、写能力，数学语言为了严格，由symbols构成，读者需要能从这些symbols中理解起背后的general abstraction/intuition；在自己研究某些程序性质时，也需要自己有能力去写。

可以研究程序的哪些问题？含义、性质、推理方式、程序间行为比较，如何设计一个语言。

理解形式语义，可以更好的把握软件可靠性、安全性，它们变得越来越重要（生命、财产安全）。并发、嵌入式（有限资源）、分布式、IoT都为安全性带来更大挑战，软件系统变得愈来愈复杂 --> 新的挑战，新的机遇。

## 可靠性、安全性

造成重大损失的原因，常常是因为simple problems in the details: 在细节处，对于语言的语义细节没有清楚把握。这意味着，我们不需要完整地定义一个程序的全部维度的正确性并证明，一些小的定理证明就是有用的。语义更清楚的语言，对于程序推理（reasoning）是有帮助的，因为提供了更明确的语义接口。

### 与测试的比较

测试是局限的（遍历所有输入），尤其在并发等场景（难以控制的不确定性）。它简单，容易自动化。


这门课会讲：
1. Coq与一些数学基础
2. lambda演算（有无类型）、类型系统、操作/指称语义、公理语义（Hoare Logic、separation logic），concurrent language and **process calculi**.
3. concurrency logic
4. 编译器验证的工作；还会讲os 验证。
5. other advanced topics.

## Coq overview

Coq 支持的是高阶逻辑（Higher Order Logic）！就是指可以对Predicate(...)进行遍历（forall/exists）.

定义inductive datatype, Machine-checkable: proof checking is as simple as type checking. [Through Curry-Howard isomorphism]

> 使用Coq做证明（比如程序证明），Reduce TCB(trusted computing base): Only need to trust the proof checker... Can be reviewed by human experts...

have a lasting influence, 获得了Software System Award

### Installation

vscoq应该是可以用的（windows platform, 添加好coq的环境变量即可）。

```shell
coqc -Q . LF xx.v  # `.` => Logical Folder
```

### TODO

关于 Coq的高阶这部分（Prop）这部分，还有稍微的不融贯，`./code/CoqFilesNew/`下面的几个再整理一遍。Software Foundation还要有时间继续写。