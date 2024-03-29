#1、[管道概述及相关API应用](https://www.ibm.com/developerworks/cn/linux/l-ipc/part1/)
##1.1 管道相关的关键概念
管道是Linux支持的最初Unix IPC形式之一，具有以下特点：

* 管道是半双工的，数据只能向一个方向流动；需要双方通信时，需要建立起两个管道；
* 只能用于父子进程或者兄弟进程之间（具有亲缘关系的进程）；
* 单独构成一种独立的文件系统：管道对于管道两端的进程而言，就是一个文件，但它不是普通的文件，它不属于某种文件系统，而是自立门户，单独构成一种文件系统，并且只存在与内存中。
* 数据的读出和写入：一个进程向管道中写的内容被管道另一端的进程读出。写入的内容每次都添加在管道缓冲区的末尾，并且每次都是从缓冲区的头部读出数据。

##1.2管道的创建：
````c
#include <unistd.h>
int pipe(int fd[2])
````
&emsp;&emsp;该函数创建的管道的两端处于一个进程中间，在实际应用中没有太大意义，因此，一个进程在由pipe()创建管道后，一般再fork一个子进程，然后通过管道实现父子进程间的通信（因此也不难推出，只要两个进程中存在亲缘关系，这里的亲缘关系指的是具有共同的祖先，都可以采用管道方式来进行通信）。

##1.3管道的读写规则：
管道两端可分别用描述字fd[0]以及fd[1]来描述，需要注意的是，管道的两端是固定了任务的。  
> 管道读端：fd[0]  
> 管道写端：fd[1]

如果试图从管道写端读取数据，或者向管道读端写入数据都将导致错误发生。一般文件的I/O函数都可以用于管道，如close、read、write等等。

**从管道中读取数据的规则：**
* 如果管道的写端不存在，则认为已经读到了数据的末尾，读函数返回的读出字节数为0；
* 当管道的写端存在时
    * 如果请求的字节数目大于PIPE_BUF，则返回管道中现有的数据字节数;
    * 如果请求的字节数目不大于PIPE_BUF，则返回管道中现有数据字节数（此时，管道中数据量小于请求的数据量）；或者返回请求的字节数（此时，管道中数据量不小于请求的数据量）。  

注： 以上说法疑有误。limits.h中有这样一行：  
    #define PIPE_BUF        4096	/* # bytes in atomic write to a pipe */  
表明PIPE_BUF是一次原子写的大小
又内核源码中有：
````c
133 /* Differs from PIPE_BUF in that PIPE_SIZE is the length of the actual
         memory allocation, whereas PIPE_BUF makes atomicity guarantees.  */
````
表明PIPE_SIZE才是管道内存的大小。2.6.11之前，PIPE_SIZE为4k，之后内核中PIPE_SIZE为64k。

注：（PIPE_BUF在include/linux/limits.h中定义，不同的内核版本可能会有所不同。Posix.1要求PIPE_BUF至少为512字节，red hat 7.2中为4096）。

[验证读规则代码](readtest/readtest.c)

## 管道的原理
管道只是一种借助于文件结构的抽象，系统内核没有这种结构。借助了其文件系统的file结构和VFS的索引节点inode，通过将两个 file 结构指向同一个临时的 VFS 索引节点，而这个 VFS 索引节点又指向一个物理页面而实现的。

![pipe_inode](_img/pipe_inode.png)

## 同步机制
如果process2读的时候，process1正在将数据放入管道，process2就必须等待。系统通过对VFS 索引节点进行锁定，让读取数据的进程进入休眠等待队列。待process1写入操作完成，内存区被解锁同时process2被唤醒读取数据。同样，process2读取时，process1也必须进入等待唤醒写入。  
注意：读写进程是同步操作，进程向管道中写消息时，管道另一端的读进程必须打开，否则写进程就会阻塞（默认情况下），命名管道也要遵守这个原则。

## 缺点
通信效率低、不能传递格式化数据、需要进行同步读取。