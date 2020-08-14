/**
 *演示简单的文件读写方法
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int createFileTest()
{
    int fd;
    //如果文件存在,文件内容清空
    //如果文件不存在，新建文件
    //相当于open函数flag指定为O WRONLY | O CREAT | O TRUNC
    fd = creat("/home/xue/test/testcreat.txt",0755);//creat,没有e
    if(fd != -1) {
        printf("file created\n");
    } else {
        printf("file can not create");
    }
    close(fd);
    close(fd);

    return 0;
}
//打开文件并将内容打印出来
int readFileTest()
{
    int fd;
    //只读模式返回一个文件描述符
    fd = open("/home/xue/test/testread.txt",O_RDONLY);
    if(fd != -1) {
        printf("file can open\n");
        ssize_t ret;//每次实际读取的字节,ssize_t是有符号的size_t,负数用来表示错误
        const int len = 3;//每次读三个字节
        char buf[len + 1];//放到数组里
        memset(buf,0,len + 1);
        while(len != 0 && (ret = read(fd,buf,len)) != 0){//等于0表示EOF，即end of file
        	if(ret == -1){//-1表示错误
        		if(errno == EINTR){//EINTR表示读入字节之前收到了一个信号，可以重新进行调用
        			continue;
        		}
        		perror("read");//打印错误消息
        		break;
        	}
        	//print buf
        	buf[ret] = '\0';
        	printf("%s",buf);
        	printf("---next\n");
        }
    } else {
        printf("file can not open");
    }
    if(close(fd) == -1){
    	perror("close");
    }
    return 0;
}

int testErrno()
{
    int fd;
    //shell中识别～，代码中不行，故返回-1
    fd = creat("~/test/a.txt",0755);
	if (fd == -1) {
		perror("create");//打印错误信息
		printf("errno:%d,errstr:%s\n", errno,strerror(errno));//获取错误码以及描述
	}
	close(fd);
    return 0;
}
int writeFileTest()
{
    int fd;
    //只读模式返回一个文件描述符
    fd = open("/home/xue/test/testwrite.txt",O_WRONLY
//    		|O_APPEND   //不加append，只是文件前面的内容覆盖了，后面的内容还在。加append，追加到末尾
    		|O_SYNC     //相当于在每个write后面隐式地执行fsync
			);
    if(fd != -1) {
    	const char *buf = "My ship is solid!";
        ssize_t nr;
        printf("Now write %s to file\n",buf);
        nr = write(fd,buf,strlen(buf));
        //------三个同步方法
        fsync(fd);//同步到磁盘
        fdatasync(fd);//仅同步数据到磁盘，不同步元数据
        sync();//同步内核缓冲区所有数据
        if(nr == -1){
        	perror("create");
        }else if(nr != strlen(buf)){
        	//部分写,这种情况’errno’没有设置。对于普通文件，意味着发生了一个错误。对于套接字，需要一个循环来保证全部写入
            printf("not complete write");
        }
        //close file
        if(close(fd) == -1){
        	perror("close");
        }
    } else {
        printf("file can not open");
    }
    return 0;
}

int main_fileio(int argc, char **argv)
{
    createFileTest();
    readFileTest();
    testErrno();
    writeFileTest();
    return 0;
}

