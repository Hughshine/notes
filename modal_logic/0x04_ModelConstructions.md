# 四、Model Constructions

已知ML的区分能力，我们可以定义维护模态逻辑真值的操作(truth preserving operations)、可以进一步思考不能用模态逻辑区分的（点）模型的“性质”。（模型类($\{(M, w)|\dots\}$)：什么样的模型类能被模态逻辑定义？）（A property is not definable by the basic modal logic if there are two bisimilar models such that one satisfies the property and one does not.）

## Bisimulation contraction

对模型$M$做bisimulation contraction，做什么：

论域：$W' = \{|w|\;|\;w\in W_M\}$ where $|w|=\{v\;|\;w\simeq v\}$.【暂使用$\simeq$表示互摹拟关系】.【相当于把所有互摹拟的点压缩为一个】

可及关系：$|w|\rightarrow'|v|$ iff $\exists w'\in|w|,v'\in|v|.\;w'\rightarrow v' in\;M$.等价类到等价类若具有某个可及关系，只要其中某个世界和另一个等价类的某个世界有可及关系。


赋值：被压缩的点所有的赋值都是一样的，保持这个赋值。$V'(|w|)=V(w)$.