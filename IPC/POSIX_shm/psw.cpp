#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>


#include "Consts.h"

//posix共享内存写的一方
int main(int argc, char **argv)
{
    int fd = shm_open(TEST_FILE_NAME, O_CREAT| O_RDWR, 0666);
    ftruncate(fd, SIZE);

    char* p = (char*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(p, 'B', SIZE);

    munmap(p, SIZE);
}
//执行完后，/dev/shm/目录可以看到myposixshm_test文件,也可以cat myposixshm_test