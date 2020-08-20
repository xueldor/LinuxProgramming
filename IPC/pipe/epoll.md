1、创建一个新的epoll实例
---
使用epoll_create()或者epoll_cerate1()创建一个epoll上下文。这里epoll_cerate1()是epoll_cerate()的扩展版本。  
````c
#include <sys/epoll.h>
int epoll_create (int size)
````
调用成功后,epoll_create()创建一个epoll实例，返回与该实例关联的文件描述符(**不是真正的文件的文件描述符，和真正的文件没有关系，仅仅是为了后续调用使用epoll而创建的**)  
size参数告诉内核需要监听的文件描述符数目，但**不是最大值**。传递一个适当的近似值会带来性能的提升，但不需要给出确切的数字。出错时，返回-1，设置errno为下列值之一：  
> **EINVAL**   size不是正数  
**ENFILE**    系统达到打开文件数的上限  
**ENOMEN**    没有足够内存完成该次操作。

标准调用如下：
````c
int epfd;
epfd = epoll_create (100); 
if (epfd <0 )
    perror("epoll_create");
````
   
epoll_create返回的文件描述符**需要用close()关闭**。

2、控制 epoll
---
epoll_ctl 可以向指定的epoll上下文加入或删除文件描述符：
````c
#include <sys/epoll.h>
int epoll_ctl (int epfd, int op, int fd, struct epoll_event *event);
````
epoll_ctl()成功调用将关联epfd和指定的fd。  
参数op指定对参数fd要进行的操作，以下是op的有效值：  
>**EPOLL_CTL_ADD** 把fd指定的文件添加到epfd指定的epoll实例监听集中，监听event中定义的事件。  
**EPOLL_CTL_DEL** 把fd指定的文件从epfd指定的epoll监听集中删除。  
**EPOLL_CTL_MOD** 使用event改变在已有fd上的监听行为。  

struct epoll_event *event参数描述epoll更具体的行为,头文件<sys/epoll.h>中定义了epoll event结构体:  
````c
struct epoll_event {
    _u32 events;
    union {
    void * ptr;
    int fd;
    _u32 u32;
    _u64 u64;
    }data;
};
````
第一个成员_u32 events出了在给定文件描述符上监听的事件。多个事件可以使用位或运算同时指定。以下是部分有效值：
>**EPOLLERR**  文件出错。即使不设置这个标志，这个事件也是被监听的。  
**EPOLLET** 使用边沿触发。默认是水平触发。  
**EPOLLHUP** 文件被挂起。即使不设置这个标志，这个事件也是被监听的。  
**EPOLLIN** 文件未阻塞，可读。  
**EPOLLONESHOT** 在一次事件产生被处理之后，文件不在被监听。必须不再被监听。必须使用EPOLL_CTL_MOD指定新的事件，以便重新监听文件。  
**EPOLLOUT** 文件未阻塞，可写。  
**EPOLLPRI** 高优先级的带外数据可读。

event_poll中的**data字段由用户使用**。**通常将event.data.fd设定为fd，这样就可以知道那个文件描述符触发事件**。  

成功后，epoll_ctl()返回0.失败返回-1，并设置errno为下列值：
>EBADF epfd不是一个有效的epoll实例，或者fd不是有效文件描述符。  
EEXIST op为EPOLL_CTL_ADD，但是fd已经与epfd关联。  
EINVAL epfd不是一个epoll实例，epfd和fd相同，或者op无效。  
ENOENT op是EPOLL_CTL_MOD或者是EPOLL_CTL_DEL，但是fd没有与epfd关联。  
ENOMEN 没有足够内存完成进程的请求。  
EPERM fd不支持epoll。  

在epfd实例中加入一个fd指定的监听文件，使用如下代码：
````c
struct epoll_event event;
int ret;
event.data.fd = fd;/*当event触发时，用户可以根据fd知道是谁触发的*/
event.events = EPOLLIN|EPOLLOUT  ;
ret = epoll_ctl (epfd,EPOLL_CTL_MOD,fd,&event);
if (ret)
    perror ("epoll_ctl");
````

修改epfd实例中的fd上的一个监听事件,可以使用如下代码:
````c
struct epoll_event event;
int ret;
event.data.fd = fd;/*return the fd to us later*/
event.events = EPOLLIN ;
ret = epoll_ctl (epfd,EPOLL_CTL_MOD,fd,&event);
if (ret)
    perror ("epoll_ctl");
````

删除一个fd监听事件,可以使用如下代码:
````c
struct epoll_event event;
int ret;
event.data.fd = fd;/*return the fd to us later*/
event.events = EPOLLIN ;
ret = epoll_ctl (epfd,EPOLL_CTL_DEL,fd,&event);
if (ret)
    perror ("epoll_ctl");
````

3、等待Epoll事件
-------
epoll_wait()等待给定epoll实例关联的文件描述符上的事件：
````c
#include <sys/epoll.h>
int epoll_wait (int epfd, struct epoll_event * 
    * events, int maxevents, int timeout);
````
成功返回，将事件放入struct epoll_event *events指向的内存，且最多可以有maxevents个事件。如果前面设置了event.data.fd = fd，那么就可以根据events数组中每个event结构体的的data.fd来确定哪些fd触发了事件，从而做出相应的处理。  
成功返回，返回值是事件数;出错，返回-1，并将errno设置为以下值
>**EBADF** epfd是无效文件描述符  
**EFAULT** 进程对events指向的内存无写权限  
**EINTR** 系统调用在完成前被信号中断  
**EINVAL** epfd不是有效的epoll实例，或者maxevents小于等于0

timeout指定等待时限为timeout毫秒。如果:  
    1) timeout 为0  即使没有事件发生，调用也立即发生，此时调用返回0.  
    2) 如果timeout为-1，调用将阻塞一直等待到有事件发生。  
    3) 如果timeout > 0,在timeout毫秒之内等待事件发生，并在事件发生后返回;如果没有事件发生，超时后返回。  
    
当调用epoll_wait()返回，epoll_event结构体中的events数组描述了**一次等待发生的事件**，最多返回maxevents个事件。每次epoll_wait()返回，events数组里面都是不同的对象，但event.data字段总是用户自己设置的值，比如文件的句柄，用来区分那个文件所发生的事件。  
一个完整的epoll_wait()例子如下：
````c
#define MAX_EVENTS   64
struct epoll_event * events = NULL;
int nr_events, i, epfd;
events = malloc (sizeof(struct epoll_event) * MAX_EVENTS);
if (! events ){
    perror("malloc");
    exit(-1);
}
nr_events = epoll_wait (epfd,events,MAX_EVENTS,-1);
if (nr_events < 0){
    perror("epoll_wait");
    free(events);
    exit (-1);
}
for (int i=0; i<nr_eventsl i++)
    printf("event = %d on fd = %d \n",
        events[i].events,events[i].data.fd);
````

4、边沿触发时间和水平触发事件
----
EPOLL事件有两种模型 Level Triggered (LT) 和 Edge Triggered (ET)：

LT(level triggered，水平触发模式)是缺省的工作方式，并且同时支持 block 和 non-block socket。在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的，所以，这种模式编程出错误可能性要小一点。

ET(edge-triggered，边缘触发模式)是高速工作方式，只支持no-block socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，等到下次有新的数据进来的时候才会再次出发就绪事件。

监听文件可读，使用边沿触发方法:
````c
ev.events = EPOLLIN|EPOLLET;
````
