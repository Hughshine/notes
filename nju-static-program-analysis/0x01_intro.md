# 绪论

程序语言进一步的分支 - 理论/运行时环境/应用

理论：语言设计、类型系统、语义与逻辑...

环境：编译器、运行时...

应用：程序分析、验证、合成

（相对来说，语言没怎么变，包括theory，environment）程序变得越来越大、越来越复杂，如何保证reliability, security, promises of large-scale / complex programs，越来越重要，编程语言应用相关部分被强调. 静态分析便作为一处应用。

> memory相关/安全相关/编译器优化（dead code elimination, code motion）/程序理解（butterfly, call hierarchy, type indication...）

`static`的含义：在**运行**程序P之前，分析其行为、确定它是否满足某些属性（一般是分析动态行为）。

静态分析是有的局限性：Rice's Theorem. 

> Any non-trivial property of the behavior of programs in a r.e. language is undecidable. (r.e. -> recursively enumerable -> recognizable by a Turing-machine) (No perfect static analysis)
>
> Non-trivial properties -> interesting properties -> the properties related with run-time behaviors of programs.

尽管我们得不到truth，也就是绝对的正确性，但我们可以适当的降低精度 —— 使得结果足够有用。

> Truth = Sound(overapproximate, 含有 false positives) + complete(underapproximate，含有 false negatives))
>
> 从positive的视角看，sound指分析出来结果的含义一定包含Truth，但可能有额外的不准确性；complete分析出来的一定是Truth，但并不包含全部的truth
>
> sound/complete关键取决于问题的truth的定义，以及答案的准确性的定义. 不必过分纠结

Sound but not fully-precise static analysis is promising. Soundness意味着全面性，尽管得到的property相比truth，更weak. Completeness意味着“可靠性”，得到的property更强。静态分析里前者多些。【还是具体情况具体分析】。

> 找bug是一个unsound的问题，但也可以用sound algo去找，思路有些类似类型系统，预先将坏的可能性去掉（不好的写法）。

> soundness <--> precision + speed 作 trade off

## 对于多数的static analysis(may analysis): Abstraction + over-approximation

静态分析算法，根据研究问题构建抽象域，只涉及关注的属性（的简化版本）。算法本身over-approx的来源有两处，transfer function 和 control flow meet两个处理。

Transfer function 是定义在抽象域上的，常常会丧失准确性（UAC）. 

每一次动态执行会有一条执行路径，静态分析就是在研究所有的执行路径共有的信息。一条路径中，可能会通过cfg某一条边无数次；同时，也可能有无数种执行。准确研究每一条路径的信息是不可能的，我们也不会在cfg的边上记录每一条执行路径每一次到达这里得到的结果。对于前者，我们会在控制流交汇时meet数据流（也就是以指令或基本块为分析单元，而非路径）；对于后者，我们会用更不准确的结果覆盖原有结果。
