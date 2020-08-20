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
    char r_buf[1024];
    char w_buf[100];
    int writenum;
    int rnum;
    memset(r_buf, 0, sizeof(r_buf));
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }

    if ((pid = fork()) == 0) {
        close(pipe_fd[1]);
        int i = 100;
        while (i--) {
            rnum = read(pipe_fd[0], r_buf, 1024);
            printf("child: readnum is %d,r_buf is %s\n", rnum,r_buf);
        }
        close(pipe_fd[0]);

    } else if (pid > 0) {
        close(pipe_fd[0]);//write
        int i = 0;
        while (i++ < 100) {
            sprintf(w_buf,"%s%d","abc",i);
            writenum = write(pipe_fd[1], w_buf, strlen(w_buf) +1);//记得末尾有个'\0'，所以len+1
            printf("the bytes write to pipe is %d \n", writenum);
        }

        close(pipe_fd[1]);
        wait(NULL);
    }
}

/****************************
输出(片段):
.....................
the bytes write to pipe is 6
the bytes write to pipe is 6
the bytes write to pipe is 6
the bytes write to pipe is 6
child: readnum is 99,r_buf is abc1
the bytes write to pipe is 6
child: readnum is 84,r_buf is abc19
the bytes write to pipe is 6
child: readnum is 6,r_buf is abc33
the bytes write to pipe is 6
child: readnum is 6,r_buf is abc34
...................

注意readnum is 99,r_buf is abc1，r_buf打印不全，因为遇到了'\0'.
write每次写一个string。对管道来说，只是二进制数据流而已。
读取时，会把许多次write写入的数据一次性全部读取出来，也可能一次write的数据分两次读出来。
读和写同时进行时，read出来的数据大小更是难以确定，是写一个byte读一个byte，还是写几个读几个，似乎有随机性。
那么如果我的需求是write多少次，read端也调用多少次，第n次read的数据与第n次write的数据完全一样，需要读写双方约定好规则。

对于这个例子，可以扫描读到的数组，根据\0符号做分割。

另外，有没有通知机制？即：写一次，读端收到通知读一次，不要一直阻塞？研究一下epoll+pipe能不能达到我的目的。

*/