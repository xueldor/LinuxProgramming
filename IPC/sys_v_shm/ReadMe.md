基于传统SYS V的共享内存, 年代久远，对应内核代码linux/ipc/shm.c。
当你编译内核的时候不选择CONFIG_SYSVIPC，则不再具备此能力。

(仅作了解，实际开发尽量用POSIX共享内存)

对应的函数是shmget、shmat、shmdt、shmctl四个函数

Linux敲ipcs命令看到的share memory就是这种共享内存.

本示例先执行sw，在执行sr，观察输出结果。

共享内存的原理见代码注释