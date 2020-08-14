/**
 *演示IO多路复用
 */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/poll.h>


#define TIMEOUT 5
#define BUF_LEN 1024

/**
 * 可移植性:select > poll > epoll,尤其epoll是linux特有;
 * 不考虑移植性，epoll优于poll优于select
 *
 * 调用后select函数会阻塞,直到有描述符就绪(有数据 可读、可写、或者有except),或者超时（timeout指定等待时间，如果立即返回设为null即可），函数返回
 * 当select函数返回后，可以通过遍历fdset，来找到就绪的描述符
 * select的一个缺点在于“单个进程”能够监视的“文件描述符”的数量存在最大限制，在Linux上一般为1024
 */
int testSelect() {
	int file_fd = open("/home/xue/test/testread.txt", O_RDONLY);

	struct timeval tv;
	fd_set readfds;//fd_set是一个位掩码集合
	int ret;
	//移除readfds中的所有文件描述符
	FD_ZERO(&readfds);
	//STDIN_FILENO加入到readfds中
	FD_SET(STDIN_FILENO, &readfds); //STDIN_FILENO:标准输入设备(一般是键盘)的文件描述符
	FD_SET(file_fd, &readfds);
	//设置等待超时
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	//第一个参数取监视的描述符数的最大值+1，表示0,1,2,...max(fds)会被监视，在 Linux 上最大值一般为1024
	ret = select(file_fd + 1, &readfds, NULL, NULL, &tv); //现在readfds中有两个fd,如果其中有一个可读，则返回
	printf("finish select\n");

	if (ret == -1) { //发生错误
		perror("select");
		return 1;
	} else if (!ret) { //ret返回值为0，表示超时
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}
	//select返回后，readfds会清空，仅保留可读的fd
	if (FD_ISSET(STDIN_FILENO, &readfds)) { //检查STDIN_FILENO是否在readfds中
		char buf[BUF_LEN + 1];
		int len;
		/* guaranteed to not block */
		len = read(STDIN_FILENO, buf, BUF_LEN);
		if (len == -1) {
			perror("read");
			return 1;
		}
		if (len) {
			buf[len] = '\0';
			printf("read: %s\n", buf);
		}
		return 0;
	} else {
		fprintf(stderr, "stdin cannot read!\n");
		return 1;
	}
}
//while循环持续查询就绪的io
int testSelect2() {
	int file_fd = open("/home/xue/test/testread.txt", O_RDONLY);
	struct timeval tv;
	fd_set readfds,tempfds;
	int ret;
	//移除readfds中的所有文件描述符
	FD_ZERO(&readfds);
	//STDIN_FILENO加入到readfds中
	FD_SET(STDIN_FILENO, &readfds); //STDIN_FILENO:标准输入设备(一般是键盘)的文件描述符
	FD_SET(file_fd, &readfds);
	while(1){
		//select调用后tv的值会变，所以每次调用前都要重新初始化。新版linux会改成剩余时间，大多数unix不确定
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		tempfds = readfds;//因为调用select后fds会清空仅保留可读，故建立一个tempfds
		ret = select(file_fd + 1, &tempfds, NULL, NULL, &tv); //现在readfds中有两个fd,如果其中有一个可读，则返回
		printf("finish select\n");

		if (ret == -1) { //发生错误
			perror("select");
			return 1;
		} else if (!ret) { //ret返回值为0，表示超时
			printf("%d seconds elapsed.\n", TIMEOUT);
			return 0;
		}
		//select返回后，readfds会清空，仅保留可读的fd
		if (FD_ISSET(STDIN_FILENO, &readfds)) { //检查STDIN_FILENO是否在readfds中
			char buf[BUF_LEN + 1];
			int len;
			/* guaranteed to not block */
			len = read(STDIN_FILENO, buf, BUF_LEN);
			if (len == -1) {
				continue;
			}
			if (len) {
				buf[len] = '\0';
				printf("read: %s\n", buf);
			}
		}
	}
	return 0;
}

//用select实现可移植的sleep
int a_sleep(int microsecond) {
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = microsecond;
	select(0, NULL, NULL, NULL, &tv);
	return 0;
}
/**
 * poll不需要计算最大的文件描述符加1
 * 文件描述符值比较大时，poll更有效率，比如fd==900时，select需要检查0～900
 * poll本质上和select没有区别，但是它没有最大连接数的限制，原因是它是基于链表来存储的
 */
int testPoll() {
	struct pollfd fds[2];
	int ret;
	//观测stdin的输入
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	//观测stdout的输出
	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;
	ret = poll(fds, 2, TIMEOUT * 1000);//毫秒，负数表示永远等待
	if (ret == -1) {//出错
		perror("poll");
		return 1;
	}
	if (!ret) {//超时
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}
	if (fds[0].revents & POLLIN)//检查revents中有POLLIN这个标志
        printf("stdin is readable\n");
	if (fds[1].revents & POLLOUT)
		printf("stdout is writable\n");

}
int main_fileioadvance(void) {
    testSelect();

    printf("----before sleep\n");
    a_sleep(5E6);
    printf("----after sleep\n");

    testPoll();
    return 0;
}
