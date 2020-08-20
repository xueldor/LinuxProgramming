#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#define FIFO_SERVER "/tmp/fifoserver"

int main(int argc, char **argv)
//参数为即将写入的字节数
{
    int fd;
    char w_buf[4096 * 2];
    int real_wnum;
    memset(w_buf, 0, 4096 * 2);
    if ((mkfifo(FIFO_SERVER, O_CREAT | O_EXCL) < 0) && (errno != EEXIST)) {//EEXIST表示文件已存在。忽略这个错误
        perror("mkfifo");
        return -1;
    }

    //设置非阻塞标志
    fd = open(FIFO_SERVER, O_WRONLY | O_NONBLOCK, 0);
    if (fd == -1) {
        perror("open");
        if (errno == ENXIO) {
            fprintf(stderr,"open error; no reading process\n");
        }
    }


    //设置阻塞标志
    fd=open(FIFO_SERVER,O_WRONLY,0);
    if (fd == -1) {
        perror("open");
        if (errno == ENXIO) {
            fprintf(stderr,"open error; no reading process\n");
        }
    }
    printf("End of block");

    real_wnum = write(fd, w_buf, 2048);
    if (real_wnum == -1) {
        perror("write_w_buf");
        if (errno == EAGAIN)
            printf("write to fifo error; try later\n");
    } else
        printf("real write num is %d\n", real_wnum);
    real_wnum = write(fd, w_buf, 5000);
    //5000用于测试写入字节大于4096时的非原子性
    //real_wnum=write(fd,w_buf,4096);
    //4096用于测试写入字节不大于4096时的原子性

    if (real_wnum == -1)
        if (errno == EAGAIN)
            printf("try later\n");
}

/**
 * 先执行test1_A,非阻塞open,返回-1并且errno == ENXIO
 * 然后阻塞的方式open，会阻塞在open调用。
 * 然后执行test1_B,open成功后，test1_A的open调用从阻塞返回。
 *
 * 结论：
 * 对于命名管道，只有读端(flag设置O_RDONLY)已经打开了，写端(flag设置O_WRONLY)才能正常打开。所以写端尽量用阻塞方式打开。
 * 不管只读端还是只写端，如果阻塞打开(没有设置O_NONBLOCK)，都要等对方打开。(当然，不会出现互相等待的情况，两个都阻塞方式打开，也是可以的，将在同时返回)
 * 但是读端非阻塞打开不依赖写端。
 *
*/