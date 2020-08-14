#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main(void) {
	pid_t targetid;
    printf("");
	scanf("%d",&targetid);
	printf("Will kill %d",targetid);
	int signal;
	while(scanf("%d",&signal) > 0) {
		printf("signal is %d",signal);
		kill(targetid, signal);
	}
	return EXIT_SUCCESS;
}
