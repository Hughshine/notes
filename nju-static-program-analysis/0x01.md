# 绪论

程序语言进一步的分支 - 理论/环境/应用

理论：语言设计、类型系统、语义与逻辑...

环境：编译器、运行时...

应用：程序分析、验证、合成

程序变得越来越大、越来越复杂，如何保证reliability, security, promises of large-scale / complex programs.

- memory相关/安全相关/编译器优化（dead code elimination, code motion）/程序理解（butterfly, call hierarchy, type indication...）

`static`的含义：在**运行**程序P之前，分析其行为、确定它是否满足某些属性。

局限性：Rice's Theorem. Any non-trivial property of the behavior of programs in a r.e. language is undecidable. (r.e. -> recursively enumerable -> recognizable by a Turing-machine) (No perfect static analysis)

Non-trivial properties -> interesting properties -> the properties related with run-time behaviors of programs.

Truth = Sound(overapproximate(false positives)) + complete(underapproximate(false negatives))

Sound but not fully-precise static analysis is promising. Soundness意味着全面性，尽管得到的property相比precise，更weak. Completeness意味着“可靠性”，得到的property更强。静态分析里前者多些。

soundness <--> precision + speed 作 trade off

## 对于多数的static analysis(may analysis): Abstraction + over-approximation

Abstraction: Concrete field state -> Anstract field state. 

Over-approximation: Transfer Functions. Concrete state transition -> Abstract state transition. Define how to evaluate different program statements on abstract values. 【undefined/unknown】. （当然会丢失信息啦）

Over-approximation: Control Flow.

