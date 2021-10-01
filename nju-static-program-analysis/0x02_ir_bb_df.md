# Intermediate representation

> 静态分析在编译过程的位置，分析对象“IR”，CFG的构建。


编译器：

regular expression -> context-free grammar -> attribute grammar. 

chars -> tokens -> AST -> decorated AST -> IR -> machine code

sa work on IR.

AST 与 三地址码的区分：AST保存的是语法结构，high-level，用于类型检查.；只有较少的控制流信息、语义信息。
三地址吗 是 low-level的，包含控制流信息。

三地址码（操作数1/2，运算符，结果；没有更多元运算），每一条指令的右侧，最多只有一个运算符。（最多有3个“地址”，地址的含义是name/constant/temporary（大概可以理解为操作数？））

> 我觉得还是说，两种ir都可以含有一定层次的语法、数据流语义、控制流语义信息，三个维度，不同的表示主要呈现的信息维度不同。静态分析不考虑语法，而考虑数据流、控制流语义，所以使用三地址码形式（或者类似的图形式）。

![](./pics/02-01.png)

Soot, most popular static analysis framework for java. [IR: Jimple，typed 3AC]

> soot项目不直观简洁，课程不用。

理解soot生成的三地址码：函数签名，参数声明（类型），函数调用（method signature, \<classsignature: return type func(parameters type)\>(paramters)），函数调用是可以传入多个参数的！不是那么“三地址码”。Class初始化，多了一个clinit(static变量的初始化)，init（对象变量的初始化），最后会special invoke父类的init. (自动继承Object)

JVM中的方法调用：

invokespecial: call constructor, super class's methods, call private methods.

invokevirtual: instance methods call (会做virtual dispatch)

invokeinterface: 和前者基本一致，cannot optimize, checking interface inplementation

invokestatic: call static methods.

Java7: invoke dynamic -> Java static typing, 但想为dynamic language（如JS，python） runs on JVM提供一些方便.

Class的三地址码：
1. extends Object
2. 保留成员，init默认初始化函数，多了clinit（class init）进行静态成员初始化（field signature:(class, type, identifier)）。

***

Static single assignment: give each definiation a fresh name. control flow merge?

定义一个special merge function (phi-function).

Why SSA: 

1. 间接的融入flow information into unique variable names. (一些简单的分析算法，可以flow-insensitive的方式做到flow-sensitive analysis)

2. define-and-use pairs are explicit, 优化容易等.

Why not:

1. 引入太多的变量和phi-function

2. 多了一些copy操作，可能会更低效率

*** 

## Control Flow analysis

control flow graph: 结点和边

图中的节点：基本块（只能从第一个指令开始、最后一个指令结束，不会有控制流从中间进入或流出）

Algo: build CFG from 3AC. 找到leaders：第一条指令/所有跳转语句的target/跳转语句的下一句. leaders之间的语句，是一个basic block.

图中的边：

1. 一个块的最后一条语句是无条件跳转，那么它与跳转块有一条边
2. 一个块的第一条语句是有条件跳转，那么它与紧邻的块与跳转块各自有一条边。

（常常将jmp的参数改为basic block id（地址的含义消去了！丢掉了没用的信息。））

会额外加extry/exit node

感觉把三地址码称为intermediate language（IL）比较好，representation（IR）更倾向于一种数据结构。

对于IR，感觉语法、数据/控制流是IR的三个基本信息维度（都含有，否则就不能相互转换了），不同IR只是直接呈现的维度不同（层次也可以不同，比如一些信息被丢弃，或者增加了硬件信息），具体用哪个IR取决于算法目标/分析目标。
