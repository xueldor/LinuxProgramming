#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#include "Consts.h"

//共享内存读取的一方
//先执行psw,再执行本程序
int main(int argc, char **argv)
{
    int fd = shm_open(TEST_FILE_NAME, O_RDONLY, 0666);
    ftruncate(fd, SIZE);

    char *p = (char*)mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);

    printf("%c %c %c\n", p[0],p[2],p[13]);

    munmap(p, SIZE);

}
