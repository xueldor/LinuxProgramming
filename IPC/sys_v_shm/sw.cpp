#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//执行 sysctl -a | grep kernel.shm，保证SIZE在系统定义的最大和最小值之间.比如我当前是：
//kernel.shmmax = 18446744073692774399
//kernel.shmmni = 4096
#define SIZE 0x40000

//共享内存写的一方
int main(int argc, char **argv)
{
    //1. 先touch /dev/shm/myshm_test, 创建文件
    //然后 用ftok函数创建KEY
    key_t key = ftok("/dev/shm/myshm_test", 1);
    if(key == -1){
        perror("ftok");
        return 0;
    }

    //2. shmget获取共享内存的id
    // 参数2是共享内存的size，PAGE_SIZE的整数倍。
    // 参数3是shm的flag,取值有IPC_CREAT、IPC_EXCL、SHM_HUGETLB、SHM_NORESERVE,也可以是访问权限
    //失败返回-1
    int shm_id = shmget(key, SIZE, IPC_CREAT|0666);//八进制，共享内存的访问权限是0666。如果不添加flag IPC_CREAT，第一次执行shmget会失败

    if(shm_id == -1){
        printf("shmget fail\n");
        perror("");
        return 0;
    }

    //3. 对共享内存进行映射
    //参数2 shmaddr是NULL，系统会自动选择一个合适的虚拟内存地址空间去映射共享内存
    char *p = (char*) shmat(shm_id, NULL, 0);
    //4. 操作这段内存，通过指针p
    memset(p, 'A', SIZE);

    //5. 解除映射
    shmdt(p);

}
