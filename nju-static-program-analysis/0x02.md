# Intermediate representation

编译器：

regular expression -> context-free grammar -> attribute grammar. 

chars -> tokens -> AST -> decorated AST -> IR -> machine code

sa work on IR.

AST 与 IR的区分：AST保存的是语法结构，high-level，用于类型检查.；只有较少的控制流信息、语义信息。
IR 是 low-level的，包含控制流信息。

IR: 三地址码，每一条指令的右侧，最多只有一个运算符。（最多有3个“地址”，地址的含义是name/constant/temporary（大概可以理解为操作数？））

![](./pics/02-01.png)

Soot, most popular static analysis framework for java. [IR: Jimple]

理解soot生成的三地址码：函数签名，参数声明（类型），函数调用（v.\<classsignature: return type func(parameters type)\>(paramters)），函数调用是可以传入多个参数的！不是那么“三地址码”。Class初始化，多了一个clinit(static变量的初始化)，init（对象变量的初始化），最后会special invoke父类的init. (自动继承Object)

JVM中的方法调用：

invokespecial: call constructor, super methods, call private.

invokevirtual: instance methods call (会做virtual dispatch)

invokeinterface: cannot optimize, checking interface inplementation

invokestatic: call static methods.

Java7: invoke dynamic -> Java static typing, dynamic language runs on JVM，提供一些方便.

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

图中的节点：基本块（只能从第一个指令开始、最后一个指令结束）

Algo: build CFG from 3AC. 找到leaders. 第一条指令/所有跳转语句的target/跳转语句的下一句. leaders之间的语句，是一个basic block.

图中的边：

1. 一个块的最后一条语句是无条件跳转，那么它与跳转块有一条边
2. 一个块的第一条语句是有条件跳转，那么它与紧邻的块与跳转块各自有一条边。

（常常将jmp的参数改为basic block id（地址的含义消去了！丢掉了没用的信息。））

会额外加extry/exit node