execl方法
========
##1、execl("../toTestExecl/PrintArgs","../toTestExecl/PrintArgs","firstParam","secondParam",NULL)
  1)第一个args[0]是用程序本身的路径.实际上随便传一个string都没问题，但是linux是这样约定的。  
  做个实验。打开shell，运行toTestExecl下面的PrintArgs程序。如果执行:  
  $ ./PrintArgs  
 **输出**: PrintArgs args[0]:./PrintArgs  
 
 **执行**: $ ../toTestExecl/./PrintArgs dddd  
**输出**：PrintArgs args[0]:../toTestExecl/./PrintArgs  
  PrintArgs args[1]:dddd  
  
  所以当我们需要用execl执行一个指令时，第一个参数是指令本身，从第二个开始才是需要的传过去的参数  
  2）args的最后一个参数必须以NULL结束  
  3）如果执行成功则函数不会返回,而是新的程序替换当前进程的正文/数据/堆栈，执行失败则直接返回-1，失败原因存于errno中  
    一个进程一旦成功调用exec类函数，它本身就"死亡"了，系统把代码段替换成新的程序的代码，废弃原有的数据段和堆栈段，并为新程序分配新的数据段与堆栈段，唯一留下的，就是进程号，也就是说，对系统而言，还是同一个进程，不过已经是另一个程序了。（不过exec类函数中有的还允许继承环境变量之类的信息。）  
那么如果我的程序想启动另一程序的执行但自己仍想继续运行的话，怎么办呢？那就是结合fork与execl的使用。  
所以昨天我在程序A里用binder调用另外一个进程，被调用的方法里有execl方法，然后程序A提示binder died。就是这个原因。

_______________________

 #pid = fork();
 fork返回值大于0时，返回的是子进程的id;返回值为0,表示在子进程中。  
 所以，在fork后面，要紧跟一组if-else，分别执行父进程和子进程的内容。  
 