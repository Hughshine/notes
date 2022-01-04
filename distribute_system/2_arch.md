# Architecture

需要分布式系统的软件，往往在其本质上就是分散的，由多个组件组合在一起。
软件系统的问题结构与分布式系统的架构是相互配合、协调的。

1. layered style
2. object-based style

时间与空间解耦与否的分类：
1. publish/subscribe, 在空间上解耦
2. shared dataspace, 在时空上解耦

organization:
1. 中心化
    1. client-server model - publish/subscribe - multi client / single server 
        * server bottleneck / single point of failure / difficult to scale
        * 即性能、拓展性瓶颈，容灾能力弱  
        * `=>` multi servers. servers之间需要信息同步
          *  proxy server
2. 去中心化（overlay network? data is routed over connections setup between the nodes, cf. application-level multicasting... TODO ）
   1. structured P2P (base on id)
   2. unstructured P2P: nodes randomly select neighbors
        * random overlay. 不能准确查找信息了，需要flooding(limited/probablistic)/random walk(works well with replicated data)
   3. hybrid P2P
      1. super peers
3. 混合:
   1. client用类似P2P的方式与server建立关系
   2. edge-server architectures

layers:
1. user interface
2. processing 
3. data 

multitiered architectures

thin clients: client基本只做gui，应用实际在远端执行，两者通信

中间件：
* 分布式系统常常是基于某种特定的架构style，这种架构可能不会再所有情境下都最优，于是需要动态地调整（中间件）的行为. （在中间件之上，建立分布式系统；中间件灵活，可拓展（interceptor））
* Interceptor: 拦截控制流（无感地；插入新的运算步骤）

Self-Management: `self-*`, autonomic computing. Feedback control model.
1. configuration
2. managing
3. healing
4. optimizing

