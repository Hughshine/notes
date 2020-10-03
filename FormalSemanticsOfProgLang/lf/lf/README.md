# Basics

Inductive, 定义数据类型

Definition (function), Fixpoint (recursive function).

```
match m with
    | xxx => ...
    | xxx => ... 
    end.
```


Notation.

Check. (检查类型)

Theorem, Example

Compute xxx. 直接输出结果.

> Module, 祈祷命名空间的作用

## Proof

> 注释很重要，形式化的符号推理，很难被人理解。尽量写出证明思路。

intros.

simpl.

reflexivity.(都能做什么，和simpl.的区别)

rewrite [<|>-] H.

destruct.

induction x as [|...].

> ```
> - destruct
>   { destruct
>       - xxx
>       - xxx    
>   }
> ```

apply. (和rewrite的区别)

remember.