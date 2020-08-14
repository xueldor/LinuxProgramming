#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void sigint_handler(int signo){
	if(signo == SIGINT){
		printf("Caught SIGINT!pid=%d\n",getpid());
	}else if(signo == SIGTERM){
		printf("Caught SIGTERM\n");
	}else{
		fprintf(stderr,"Unexpected signal\n");
		exit(EXIT_FAILURE);
	}
//	exit(EXIT_SUCCESS);
}

int main(void) {
	//SIGINT没有被忽略时才处理它
	if(signal(SIGINT,SIG_IGN) != SIG_IGN){
		if(signal(SIGINT,sigint_handler) == SIG_ERR){
			fprintf(stderr,"Cannot handle SIGINT\n");
			exit(EXIT_FAILURE);
		}
	}
	if(signal(SIGTERM,sigint_handler) == SIG_ERR){
		fprintf(stderr,"Cannot handle SIGTERM\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGPROF,SIG_DFL) == SIG_ERR){
		fprintf(stderr,"Cannot handle SIGPROF\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGHUP,SIG_IGN) == SIG_ERR){
		fprintf(stderr,"Cannot handle SIGHUP\n");
		exit(EXIT_FAILURE);
	}
	printf("pid=%d\n", getpid());
	//调用fork时，子进程完全继承父进程的信号语义。因为子进程与父进程共享同一个地址空间。
	pid_t pid = fork();
	if(!pid){
		//in sub process
		printf("pid in sub =%d\n", getpid());
		pid_t ppid = getppid();

	}
	raise (SIGINT);//给自己发信号,等价kill(getpid(), SIGINT);
	for(;;){
		pause();//暂停，等收到信号，执行完sigint_handler后，从这里继续执行，除非sigint_handler中调用exit直接退出
		printf("continue\n");
	}
	return EXIT_SUCCESS;
}
