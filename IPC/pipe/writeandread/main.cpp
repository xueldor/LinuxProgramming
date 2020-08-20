#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include <wait.h>
#include <linux/limits.h>

int main(int argc, char **argv) {
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4096];
    char w_buf[4096 * 20];
    int writenum;
    int rnum;
    memset(r_buf, 0, sizeof(r_buf));
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }

    if ((pid = fork()) == 0) {
        close(pipe_fd[1]);
        int i = 15;
        while (i--) {
            sleep(3);
            rnum = read(pipe_fd[0], r_buf, 5000);
            printf("child: readnum is %d\n", rnum);
        }
        close(pipe_fd[0]);

    } else if (pid > 0) {
        close(pipe_fd[0]);//write
        if ((writenum = write(pipe_fd[1], w_buf, 1024)) == -1)
            printf("write to pipe error\n");
        else
            printf("the bytes write to pipe is %d \n", writenum);

        writenum = write(pipe_fd[1], w_buf, 4096*20);
        printf("the bytes write to pipe is %d \n", writenum);
        close(pipe_fd[1]);
        wait(NULL);
    }
}
/***************
 * 输出结果：
the bytes write to pipe is 1024
child: readnum is 5000
child: readnum is 5000
child: readnum is 5000
child: readnum is 5000
the bytes write to pipe is 81920
child: readnum is 5000
 ...........
 *
 *表明写数据端当写满缓冲区的空闲空间后，会一直阻塞，直到读端读取了一些数据
 *
 */
