# 复制与一致性

复制：保证可靠性（避免单点故障）与性能（数量上的可拓展性）；但需要维持复制透明性，一些一致性保证

## 一致性

一致性模型：讨论如何描述与推理全局状态的一致性（对共享状态的访问，进程可以期待什么）

数据为中心的一致性：strict/sequential consistency, linearizability

client为中心的一致性：eventual consistency

要考虑的主要问题：update propagation method

复制协议（replication protocols）：采取某种更新传播方法以维护一致性的算法

### strict consistency

read只能读到最近的写

依赖绝对的全局时间

### sequential consistency
