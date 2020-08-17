# [Intro](https://cs.nju.edu.cn/hongjin/teaching/semantics/)

刚刚开始看梁老师的课程。第一节课的Intro中，指向一篇[blog](http://www.pl-enthusiast.net/2015/05/27/what-is-pl-research-and-how-is-it-useful/)，配合老师的课件，看的我热血沸腾...

在信科夏令营的时候，我瞎想说可以将形式化方法应用于软件工程——建构一种新的设计模式，将某一类问题抽象化、形式化，形成一个形式化“设计模式”的社区，其中是对于各类general problem的模型建构，最终可以设计一种DSL。其实这就是PL community在做的事呀... 文章里提到的Adapton、概率编程，都是对一类问题背后的数学思考后，设计了语言完成这一类通用的问题。

PL最重要的思想是：developing general abstractions, or building blocks, for solving problems, or classes of problems. 同时也用于对软件行为进行评估。

PL + X，PL的思想与某些其他领域结合，is great. 第一生产力。

PL与安全性，bugs-free,crash-proof code.

梁老师后面提出一些优势... 软件可靠性、安全性很重要，对于语言的深入理解有助于写出更好的程序。我想最重要的是“abstract”。

这门课会讲：
1. Coq与一些数学基础
2. lambda演算、类型系统、操作/注释语义、Hoare Logic、separation logic
3. concurrency logic
4. 梁老师好像在做 编译器验证的工作；还会讲os 验证。
5. other advanced topics.

## Coq overview

普通的程序验证：太无聊了！！！没人会做。。而且基本上就是会出错。

Machine-checkable: proof checking is as simple as type checking. [Through Curry-Howard isomorphism]

> Only need to trust the proof checker... Can be reviewed by human experts...

have a lasting influence, 获得了Software System Award

编译指令

### Installation

Coq 似乎很容易安装；还要装Proof General[A generic Emacs interface], Emacs

vscoq应该是可以用的（windows platform, 添加好coq的环境变量即可）。

```shell
coqc -Q . LF xx.v  # `.` => Logical Folder
```

### Software Foundation

Software Foundation 这本书似乎很新。里面提到：FP的pure使得并发、并行很方便；同时，它连接了logic and computer science. Proofs are programs.

书中还提到，Logic在计算机领域扮演十分essential的角色。两者互相促进。