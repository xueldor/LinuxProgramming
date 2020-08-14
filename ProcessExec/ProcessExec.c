#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "apue.h"

static void my_exit1();
static void my_exit2();

void testExec(){
    char cwd[100];//确保长度能够容纳路径
    realpath("./", cwd);
    printf("current dir is : %s\n",cwd);

	int ret;
//	//不管参数对不对，bash会取代当前程序。后面的都不会执行
//	ret = execl("/bin/bash","bash","/home/xue/testexec",NULL);//运行testexec文件
//	if(ret == -1){
//		perror("execl bash");
//	}
    //约定第一个arg用程序名
    ret = execl("../toTestExecl/PrintArgs","../toTestExecl/PrintArgs","firstParam","secondParam",NULL);//运行testexec文件
    if(ret == -1){
        perror("execl PrintArgs");
    }
	//如果上面的代码成功执行，那么新的程序替换当前进程的正文/数据/堆栈，后面的代码也就不会执行了
	ret = execl("/usr/bin/gedit","gedit","/home/xue/testexec",NULL);//用gedit打开文件
	if(ret == -1){
		perror("execl gedit");
	}
}

void testFork(){
	pid_t pid;
	printf("hahaha%d\n",getpid());//当前进程执行一次。末尾加\n在fork前刷新缓冲避免hahaha输出两次
	pid = fork();//fork一个新进程.在父进程中，返回子进程的pid;在子进程中，返回0表示fork成功
	printf("wawawa%d",getpid());//当前进程和子进程各执行一次
	if(pid > 0){
		//fork函数返回，继续执行
		printf("I am the parent of pid=%d,my pid is %d!\n", pid,getpid());
		printf("Waiting for child\n");
		int status;
		pid = wait(&status);//当某个子进程死亡，wait立即返回
		waitpid(pid,&status, WNOHANG);//等待指定的子进程
		if(pid == -1)
			perror("wait");
		if(WIFEXITED(status))
			printf("Normal termination with exit status=%d\n", WEXITSTATUS(status));
		if(WIFSIGNALED(status))
			printf("Killed by signal=%d%s\n", WTERMSIG(status),
					WCOREDUMP(status) ? "(dumped core)" : "");
		if(WIFSTOPPED(status))
			printf("Stopped by signal=%d\n", WSTOPSIG(status));
		if(WIFCONTINUED(status))
			printf("Continued\n");
		sleep(3);
	}else if(!pid){//
		//这是在fork出的子进程内
		printf("I am the baby!My pid is %d,my parent pid is %d\n",getpid(),getppid());
		int ret = execl("/usr/bin/gedit","gedit","/home/xue/testexec",NULL);//用gedit打开文件
		if(ret == -1){
			perror("execl");
		}
		sleep(6);
	}else if(pid == -1){
		perror("fork");
	}
}

int main(int argc, char* args[]) {
    for(int i = 0;i<argc;++i){
        printf("args[%d]:%s",i,args[i]);
    }
    printf("\n");

    //注册程序退出时执行的函数
	if(atexit(my_exit1) != 0){
		err_sys("Can't register my_exit1");
	}
	if(atexit(my_exit2) != 0){
		err_sys("Can't register my_exit2");
	}
	if(atexit(my_exit2) != 0){
		err_sys("Can't register my_exit2");
	}
	printf("my process id(PID) is %d\n",getpid());
	printf("my parent's process id(PID) is %d\n",getppid());

	sleep(3);
	testExec();
//	testFork();
	return EXIT_SUCCESS;
}
static void my_exit1(){
	printf("first exit handler\n");
}
static void my_exit2(){
	printf("second exit handler\n");
}
