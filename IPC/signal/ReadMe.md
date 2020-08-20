##信号本质
信号是在软件层次上对中断机制的一种模拟，在原理上，一个进程收到一个信号与处理器收到一个中断请求可以说是一样的  
信号是进程间通信机制中唯一的异步通信机制，可以看作是异步通知  

##信号来源
信号事件的发生有两个来源：**硬件来源**(比如我们按下了键盘或者其它硬件故障)；**软件来源**，最常用发送信号的系统函数是kill, raise, alarm和setitimer以及sigqueue函数，软件来源还包括一些非法运算等操作。  

##信号的种类
####"不可靠信号"
Linux信号机制基本上是从Unix系统中继承过来的。早期Unix系统中的信号机制比较简单和原始，后来在实践中暴露出一些问题，因此，把那些建立在早期机制上的信号叫做"不可靠信号"，信号值小于SIGRTMIN.  
Linux对不可靠信号机制做了改进：在调用完信号处理函数后，不必重新调用该信号的安装函数（信号安装函数是在可靠机制上的实现）。因此，Linux下的不可靠信号问题主要指的是信号可能丢失。
####"可靠信号"
原来定义的信号已有许多应用，不好再做改动，最终只好又新增加了一些信号，并在一开始就把它们定义为可靠信号，这些信号支持排队，不会丢失.  
同时，信号的发送和安装也出现了新版本：信号发送函数sigqueue()及信号安装函数sigaction()  
信号值位于SIGRTMIN和SIGRTMAX之间的信号都是可靠信号.

####实时信号与非实时信号
非实时信号都不支持排队，都是不可靠信号；实时信号都支持排队，都是可靠信号。

##进程对信号的响应
三种方式来响应一个信号：  
>（1）忽略信号，即对信号不做任何处理，其中，有两个信号不能忽略：SIGKILL及SIGSTOP；  
>（2）捕捉信号。定义信号处理函数，当信号发生时，执行相应的处理函数；  
>（3）执行缺省操作，Linux对每种信号都规定了默认操作，注意，进程对实时信号的缺省反应是进程终止。

##信号的发送
主要函数有：kill()、raise()、 sigqueue()、alarm()、setitimer()以及abort()。  

##设置信号关联动作
linux主要有两个函数实现信号的安装：signal()、sigaction()  

1、signal()
```c
#include <signal.h>
void (*signal(int signum, void (*handler))(int)))(int);
```

2、sigaction()
```c
#include <signal.h>
int sigaction(int signum,const struct sigaction *act,struct sigaction *oldact));
```

sigaction函数用于改变进程接收到特定信号后的行为。该函数的第一个参数为信号的值，可以为除SIGKILL及SIGSTOP外的任何一个特定有效的信号（为这两个信号定义自己的处理函数，将导致信号安装错误）。第二个参数是指向结构sigaction的一个实例的指针，在结构sigaction的实例中，指定了对特定信号的处理，可以为空，进程会以缺省方式对信号处理；第三个参数oldact指向的对象用来保存原来对相应信号的处理，可指定oldact为NULL。如果把第二、第三个参数都设为NULL，那么该函数可用于检查信号的有效性。

第二个参数最为重要，其中包含了对指定信号的处理、信号所传递的信息、信号处理函数执行过程中应屏蔽掉哪些函数等等。

##信号阻塞与信号未决:
```c
#include <signal.h>
int  sigprocmask(int  how,  const  sigset_t *set, sigset_t *oldset))；
int sigpending(sigset_t *set));
int sigsuspend(const sigset_t *mask))；
```

演示signal的一个简单demo见[这里](./../../TestSignal)
