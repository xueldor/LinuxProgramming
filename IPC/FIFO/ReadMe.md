#有名管道
FIFO即有名管道（named pipe），它提供一个路径名与之关联，因此能用与不存在亲缘关系的进程之间相互通信。  
FIFO严格遵循先进先出（first in first out），对管道及FIFO的读总是从开始处返回数据，对它们的写则把数据添加到末尾。它们不支持诸如lseek()等文件定位操作。  
一般文件的I/O函数都可以用于FIFO，如close、read、write等等。
  
创建
----
```c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char * pathname, mode_t mode)
```
