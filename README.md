用Clion打开。如果不是打开里面的某一个demo，而是LinuxProgramming整个目录，那么所有demo都在project视图里。  
这时，当你需要执行里面的某个demo时，选中该demo的CMakeLists.txt,然后右击，点击“Load CMake Project”.

FileIO: 演示文件读写有关的函数，包括Linux里面十分重要的epoll机制

ProcessExec： 演示了与进程有关的函数：fork新的进程、调用其他程序

TestSignal: 演示两个进程之间信号量的发送、注册


调试：  
当系统调用没有按预期返回时，可以在后面加一行perror("xxxx")来打印异常信息
