#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

//执行 sysctl -a | grep kernel.shm，保证SIZE在系统定义的最大和最小值之间.比如我当前是：
//kernel.shmmax = 18446744073692774399
//kernel.shmmni = 4096
#define SIZE 0x40000

//共享内存读取的一方
//先执行sw,再执行本程序
int main(int argc, char **argv)
{
    //1. 用ftok函数创建KEY
    key_t key = ftok("/dev/shm/myshm_test", 1);
    if(key == -1){
        perror("ftok");
        return 0;
    }

    //2. shmget获取共享内存的id
    // 参数2是共享内存的size，PAGE_SIZE的整数倍。
    // 参数3是shm的flag,取值有IPC_CREAT、IPC_EXCL、SHM_HUGETLB、SHM_NORESERVE,也可以是访问权限
    //失败返回-1
    int shm_id = shmget(key, SIZE, IPC_CREAT|0666);//八进制，共享内存的访问权限是0666
    if(shm_id == -1){
        perror("shmget");
        return 0;
    }

    //3. 对共享内存进行映射
    //参数2 shmaddr是NULL，系统会自动选择一个合适的虚拟内存地址空间去映射共享内存
    char *p = (char*) shmat(shm_id, NULL, 0);
    //<--------------到这里跟sw.cpp完全相同，都是得到指针p

    //4. 通过指针p读取这段内存
    printf("%c %c %c\n", p[0], p[1], p[2], p[3]);

    //5. 解除映射
    shmdt(p);

}
