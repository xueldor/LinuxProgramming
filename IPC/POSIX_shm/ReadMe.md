POSIX共享内存, 与传统SYS V的共享内存类似。

相关函数是shm_open()、mmap ()

内存的拷贝(memcpy)是占据内存带宽、CPU利用率的消耗大户,共享内存免去了拷贝，是Linux进程间通信、计算机系统里各个不同硬件组件通信的最高效方法。

原理说白了，无非就是把一块物理内存映射到多个进程的逻辑地址。

posix的共享内存是通过用户空间挂在的tmpfs文件系统实现的，而system V的共享内存是由内核本身的tmpfs实现的。可以看出，二者其实是用同一种机制实现的。
posix的共享内存严格依赖用户空间tmpfs的挂载。
其实自己在ramdisk某个路径建个文件，通过文件实现跨进程，也就和共享内存差不多。

## NOTE
shell执行`man shm_open`, 有一行“Link with -lrt.”，
意思编译的时候要加-lrt:
```shell
gcc psw.cpp -o psw -lrt
```
CMakeLists中这样写：
```cmake
add_executable(psw psw.cpp)
add_executable(psr psr.cpp)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(psw ${LIBRT})
    target_link_libraries(psr ${LIBRT})
endif()
```