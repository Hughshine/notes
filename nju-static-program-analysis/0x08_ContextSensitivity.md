# Context Sensitivity

一个函数可能在不同的上下文中被调用，不同上下文可能有一些数据流的特征。不考虑调用的上下文，我们是以全局的角度看待每个函数，causing spurious data flows. 考虑上下文，可以增加精确性。

一种context的抽象策略是，使用call-site sensitivity，represents each context of a method as a chain of call sitees(abstract call stack).

函数、变量、堆（objects），都根据上下文被clone、被细化。

> 对于函数，它有有限个被调用的代码位置，我们为每一个位置创建该函数静态分析的副本。（函数不同位置调用，形参、局部变量被区分）
> 
> 堆中对象的context，最常用的是继承所在函数的context（对象所在函数的调用点）。（函数中动态创建的对象，被区分）

尽管可以只为函数附加context，但是如果不同时做heap-sensitive，准确率不会有很大提升，要做就一起做嘛。C.S heap是依赖C.S的。

## Context Sensitive Pointer Analysis: rules

> 引入Context，所有rule也被细化，variable/object都增加了context

![](./pics/08-01.png)

![](./pics/08-02.png)

new `i: x = new T()`: context下被调用的函数中的变量x，可以指向在i行声明的object i:oi. 

assign `x=y`: 首先，动态执行时，进行赋值的x, y，一定位于相同context；但每个函数可能有任意有限个context。对于每一个可能的context，我们复制该context下右侧变量的可能指针集给相同context下的左侧变量. from src to target. 注意，每个指针指向的对象，不一定和指针在同一个context.

store `x.f = y`: 
1. 对于每一对位于相同context(c)的x, y
2. 对于x可能指向的每一个某context的对象c':oi
3. 它的指针f（`c':oi.f`）
4. 可以指向(c:)y能指向的所有对象

load `y = x.f`:
1. 对于每一对位于相同context(c)的x, y
2. 对于x可能指向的每一个某context下的对象oi
3. 其指针f所能指向的所有对象
4. 都可以被y指向

> store/load有两重量词，对应集合的集合：o.f对应指针集，x对应指针集的集合

call `l: r=x.k(a1, ..., an)`: 
1. 对于x可能指向的每一个某个context下的对象oi
2. 我们得到此时对应的函数（virtual dispatch）
3. 并根据call site的上下文信息，得到这个函数调用的上下文c^t
4. `this`指针的对象集：`c^t:oi`
5. 传递参数：c下的实参aj所指向的指针集，赋予c^t下的形参pj指向的指针集.
6. 送回返回值：c^t下的返回值所指向的指针集，添加至c下的`r`指向的指针集.

> 语句所在context为c，被调用函数函数体context为c^t

![](./pics/08-03.png)

## Context-Sensitive Pointer Analysis

算法：与insensitive相比，sensitive算法只是额外考虑context信息。算法整体结构没有变化，仍然保持维护PFG与points-to information，两者mutually dependent. 

可以注意到，此时我们的指针，是context-sensitive pointer.

细节去看算法，我们需要区分不同context下的函数调用（函数的形参和局部变量）与在堆创建的对象。算法本质上没有差别，只是额外地纳入了context信息。就像是... LR(0)与LR(1)算法的区别。

![](./pics/08-04.png)

## Context Sensitivity Variants

我们可以设计多种Select函数，这决定了context的构成。`Select(c, l, c':oi)`

* call-site sensitivity
* object sensitivity
* type sensitivity
* ...

### k-CFA

就call-site sensitivity来说，我们可以维护call chain作为context. 被称为`call-string sensitivity`，or `k-CFA`. 为了保证可以终止与空间效率，一般设置call-string长度上限，最后k个. 实践中，k一般小一点，小于等于3是常见的。函数与堆可以用不一样的k.

> 若这个chain可以无限大，是不是就接近了动态执行...

### object sensitivity("allocation-site sensitivity")

call-site sensitivity的层数k，限定了算法的区分能力。当代码设计时具有进行了多层封装抽象、导致实际有效的函数栈很深，这会要求k变高，才能达到好的分析效果。

object sensitivity利用了OO“所有函数调用都是某个对象在调用”这一特性，它可以较好地处理call-site sensitivity的缺点：函数栈很深无所谓、我们只关注对象栈。

![](./pics/08-05.png)

在理论上，两者的precision是难以比较的（TODO）；但实际使用中，object sensitivity效果在OO上更好。(甚至效率也更好)

### Type Sensitivity

Type Sensitivity是Object Sensitivity的更粗略抽象。将具体的对象用更抽象的type代替。type不是该对象的类型，而是该对象声明时所在的函数的类型。

一些测试集上，效率高一些（显著地高），准确率低一些（比c.s.要好，多数不比o.s.差很多）。（应该都指OO上）