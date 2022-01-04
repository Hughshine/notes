[toc]

## Intermediate representation

IR: 静态分析的分析对象

> 编译器：
>
> regular expression -> context-free grammar -> attribute grammar.
>
> chars -> tokens -> AST -> decorated AST -> (M)IR -> machine code
>
> static analysis work on IR. 静态分析在编译器的中端起作用。

AST 与 三地址码的区分：AST保存的是语法结构，high-level，用于类型检查.；只有较少的控制流信息、语义信息。
三地址吗 是 low-level的，包含控制流信息。

三地址码（操作数1/2，运算符，结果；没有更多元运算），每一条指令的右侧，最多只有一个运算符。（最多有3个“地址”，地址的含义是name/constant/temporary）

> 我觉得还是说，两种ir都可以含有一定层次的语法、数据流语义、控制流语义信息，三个维度，不同的表示主要呈现的信息维度不同。静态分析不考虑语法，而考虑数据流、控制流语义，所以使用三地址码形式（或者类似的图形式）。

![](./pics/02-01.png)

> Soot, most popular static analysis framework for java. [IR: Jimple，typed 3AC]
>
> soot的静态分析框架，不直观简洁，课程不用。会用它的前端。

理解soot生成的三地址码：函数签名，参数声明（类型），函数调用（method signature, \<classsignature: return type func(parameters type)\>(paramters)），函数调用是可以传入多个参数的！不是那么“三地址码”。Class初始化，多了一个clinit(static变量的初始化)，init（对象变量的初始化），最后会special invoke父类的init. (自动继承Object)

> JVM中的方法调用：
>
> 1. invokespecial: call constructor, super class's methods, call private methods.
> 2. invokevirtual: instance methods call (会做virtual dispatch)
> 3. invokeinterface: 和前者基本一致，cannot optimize, checking interface inplementation
> 4. invokestatic: call static methods.
>
> * Java7: invoke dynamic -> Java is static typing, 但想为dynamic language（如JS，python） runs on JVM提供一些方便.

> Class的三地址码：
>
> 1. extends Object
> 2. 保留成员，init默认初始化函数，多了clinit（class init）进行静态成员初始化（field signature:(class, type, identifier)）。

---

Static single assignment: give each definiation a fresh name. control flow merge?

定义一个special merge function (phi-function), 以保证在控制流汇聚时，仍然只有一个定义。`x_3 = phi(x_1, x_2)`，其语义是根据抵达的分支进行选择。

Why SSA:

1. 间接的融入flow information into unique variable names. (一些简单的分析算法，可以flow-insensitive的方式做到flow-sensitive analysis)
2. define-and-use pairs are explicit, 优化容易等.

Why not:

1. 引入太多的变量和phi-function
2. 多了一些copy操作，可能会更低效率

## Control Flow analysis

control flow graph: 结点和边

图中的节点：基本块（只能从第一个指令开始、最后一个指令结束，不会有控制流从中间进入或流出）【单指令就可以是节点】

从线性的三地址码，建立成（力度最粗的）CFG：

1. 寻找每个基本块的leader（每个基本块内部的代码，都是连续的，所以找到leader就找到了结点）
   1. 第一个指令
   2. 任意跳转指令的目标指令
   3. 任意跳转指令的下一条指令
2. 按照每个基本块最后一个指令，建立边（根据条件/非条件跳转/非跳转）

会额外加extry/exit node

> 感觉把三地址码称为intermediate language（IL）比较好，representation（IR）更倾向于一种数据结构。

> 对于IR，感觉语法、数据/控制流是IR的三个基本信息维度（都含有，否则就不能相互转换了），不同IR只是直接呈现的维度不同（层次也可以不同，比如一些信息被丢弃，或者增加了硬件信息），具体用哪个IR取决于算法目标/分析目标。
