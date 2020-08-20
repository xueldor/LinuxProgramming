#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wait.h>

using namespace std;

int main()
{
    struct epoll_event ev;                     //事件临时变量
    const int MAXEVENTS = 1024;                //最大事件数
    struct epoll_event events[MAXEVENTS];      //监听事件数组
    int ret,pid;
    int pipe_fd[2];
    if((ret=pipe(pipe_fd))<0)
    {
        cout<<"create pipe fail:"<<ret<< ",errno:" << errno <<endl;
        return -1;
    }
    ev.data.fd = pipe_fd[0];        //设置监听文件描述符
    ev.events = EPOLLIN|EPOLLET;    //设置要处理的事件类型和触发方式

    int epfd=epoll_create(MAXEVENTS);//参数是预估的fd数量，不是最大数。传递一个适当的近似值会带来性能的提升
    if (epfd <0 )
        perror("epoll_create");

    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,pipe_fd[0],&ev);
    if (ret != 0)
    {
        cout << "epoll_ctl fail:" << ret << ",errno:" << errno << endl;
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(epfd);
        return -1;
    }
    if((pid=fork())>0)
    {
        int count=epoll_wait(epfd,events,MAXEVENTS,5000);
        char r_buf[100];
        for(int i=0;i<count;i++)
        {
            if((events[i].data.fd==pipe_fd[0])&&(events[0].events&EPOLLIN))
            {
                int r_num=read(pipe_fd[0],r_buf,100);
                printf("read num is %d bytes data from the pipe,value is %d \n",r_num,atoi(r_buf));
            }
        }
        close(pipe_fd[1]);
        close(pipe_fd[0]);
        close(epfd);
        cout<<"parent close read fd[0],wirte fd[1] and epfd over"<<endl;

        wait(NULL);
    }
    else if(pid==0) {
        close(pipe_fd[0]);//read
        cout << "sub close read fd[0] over,sub does't read" << endl;
        char w_buf[100];
        int i = 100;
        while (i--) {
            strcpy(w_buf, "1234");
            if (write(pipe_fd[1], w_buf, 5) != -1) {//you can remove this line for learn
                printf("sub write over \n");
            }
        }
        close(pipe_fd[1]);//write
        printf("sub close write fd[1] over \n");

    }

    return 0;
}
/****************************************
 * 通过epoll实现新数据到达的监听
 * 读端r_buf是1234\01234\01234\0...............
 * 接收端还是一次性把所有字符串都读了出来放在buffer里面，分不清发送端是怎样发的。
 * 假设这种场景：发送方发送信号，可能是1, 2 ， 12
 * 如果接收方收到byte[0]=1,byte[1]=2,那么无法判断发送方发送的是1和2两个信号，还是一个12信号;
 * 应该制定一些额外的规则，比如信号之间插入分界符。
 * 对于这个demo程序，因为是字符串，所以可以用\0作为分界符
 *
 * 后面演示的一些其他IPC方式将会解决这个问题
 */