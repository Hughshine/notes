分析一个（具体的）执行路径，去得到有用的结果。与静态分析相反（互补），complete but unsound.

debugger is useful.

GDB: 还提供了reverse-step（record full?）,`rsi`.

> https://stackoverflow.com/questions/1470434/how-does-reverse-debugging-work 

如何处理空间爆炸的问题：TODO

how to implement GDB: how to implement a single step: patch a instruction

动态分析最主要的技术，就在于“gdb”. 找到问题，找到采样方式，找到研究方法，是动态分析研究的主要过程。