/**************
* readtest.c *
**************/
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main()
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[100];
    char w_buf[4];
    int r_num;
    int cmd;

    memset(r_buf,0,sizeof(r_buf));
    memset(w_buf,0,sizeof(r_buf));
    if(pipe(pipe_fd)<0)
    {
        printf("pipe create error\n");
        return -1;
    }

    if((pid=fork())==0)//子进程
    {
        printf("\n");
        close(pipe_fd[1]);//关闭子进程的写端

        printf("read from pipe_fd[0],while no data in pipe\n");
        r_num=read(pipe_fd[0],r_buf,100);
        printf( "read num is %d the data read from the pipe is %d\n",r_num,atoi(r_buf));

        sleep(2);

        printf("read from pipe_fd[0] again\n");
        memset(r_buf,'3',sizeof(r_buf));
        r_num=read(pipe_fd[0],r_buf,100);
        printf( "read num is %d the data read from the pipe is %s\n",r_num,r_buf);

        sleep(2);
        r_num=read(pipe_fd[0],r_buf,100);//这时管道写端已经关闭，r_num是0,r_buf没有改变还是前一次的值
        printf( "XXX read num is %d ,r_buf is still %d\n",r_num,atoi(r_buf));

        close(pipe_fd[0]);

        exit(0);
    }
    else if(pid>0)
    {
        close(pipe_fd[0]);//关闭read端

        sleep(5);
        strcpy(w_buf,"111");
        if(write(pipe_fd[1],w_buf,4)!=-1)
            printf("parent write over\n");

        sleep(6);
        strcpy(w_buf,"2222222");
        if(write(pipe_fd[1],w_buf,4)!=-1)
            printf("parent write over\n");

        close(pipe_fd[1]);//write
        printf("parent close fd[1] over\n");

        //在通过clion运行时发现，父进程结束退出，子进程变成僵尸进程（defunct），同时控制台也就结束了，看不到子进程的输出，所以我加了wait
        wait(NULL);
    }
}
/**************************************************
* 过程：
× 先从管道读数据，几秒后另一端往管道中写
* 结论：
* 管道中没有数据时，read方法阻塞;管道中没有数据，且写端已经close时，read方法立即返回0
 * 传送字符串时记得末尾的\0.或者读端提前将buffer清0
****************************************************/