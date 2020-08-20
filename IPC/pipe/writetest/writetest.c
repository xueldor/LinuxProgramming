#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>

static void sigint_handler(int signo){
    if(signo == SIGPIPE){
        perror("SIGPIPE");
    }
}

//验证所有的读端都已经被关闭时，往管道写数据的行为
int main() {
//    signal(SIGPIPE,sigint_handler);

    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    char *w_buf;
    int writenum;
    int cmd;

    memset(r_buf, 0, sizeof(r_buf));
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }

    if ((pid = fork()) == 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        sleep(10);
        exit(0);
    } else if (pid > 0) {
        sleep(3); //等待子进程完成关闭读端的操作
        close(pipe_fd[0]);//read
        w_buf = "111";
        if ((writenum = write(pipe_fd[1], w_buf, 4)) == -1)
            printf("write to pipe error\n");
        else
            printf("the bytes write to pipe is %d \n", writenum);

        close(pipe_fd[1]);
    }
}
/****************************
如果不注册sigint_handler，在write时，管道断裂,进程收到了SIGPIPE信号，按照默认动作进程终止。
同时控制台输出  Signal: SIGPIPE (Broken pipe)
经实测，debug运行可以看到此输出。run看不到

如果注册了sigint_handler，在sigint_handler里收到SIGPIPE信号。然后write返回-1。
****************************/