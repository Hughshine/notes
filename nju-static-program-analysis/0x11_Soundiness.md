# 11. Soundness

Soundness要求去做保守估计，捕获所有程序行为，或者令分析结果建模所有可能的动态执行。

但实际使用时，我们总在做unsound analysis. 因为有许多语言特性对于静态分析是困难的。强行保证soundness，会导致imprecision.

* java: reflection, native code, dynamic class loading, etc.
* javascript: eval, DOM, etc.
* c/c++: 指针代数，函数指针，etc.

所以，我们对于大多数语言特性的处理是over-approximated，某一些是under-approximated. 实际使用中，若没有很好地处理这些困难的语言特性，会对分析结果造成很大影响。不过这一部分features没有被额外强调。

所以：很难定量解释分析算法的好坏（how sound/fast/precise）.

## Soundiness

soundy analysis: analysis is mostly sound, with **well-identified** unsound treatments to hard/specific language features.

## Hard Language features: Java Reflection & Native Code.

### Reflection

语言的动态特性，使得静态分析变得很困难。尤其是很重要的信息，是否是元信息很重要。比如class, method, field.

如何分析java reflection：

* string constant analysis + pointer analysis: 我们终究要得到源信息，它们有可能以字符串存储、可能存放于配置文件中.. 但可能就是无法在静态条件确定。
* type inference + string analysis + pointer analysis: 当我们并不能静态确定string arguments时，我们可以根据usage points进行推理。【会假设程序员使用的是对的】
* assisted by dynamic analysis

为什么是soundy：会在信息不足时对反射做unsound分析（而不是理解为sound，认为它会invoke所有方法）。

### Native Code

JNI: Java native interface. 接口最后会调用本机原生代码，一般是由c编译，平台相关。

![](./pics/11-01.png)

（230 JNI functions）

JNI functions 可以去创建对象，access fields，invoke methods等in native code. 知晓底层代码做了什么是困难的。

目前处理native code的方式：manually models the critical native code.比如有些函数为了高效，使用c去写，但实际功能可由java表达（模拟）。

Recent Work: identifying java calls in native code via Binary Scanning. (ISSTA 2020)

要看：http://soundiness.org/ 很可能会游泳。
