#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	printf("\n%d) simple test for fork, getpid, and _exit\n",1);
	int x;
	pid_t pid;
	pid_t pid2;
	pid = fork();

	if (pid<0) {
		warn("fork");
	}

	pid2 = getpid();
	printf("fork return: %d\n",pid);
	if (pid !=0){
		printf("getpid parent pid: %d\n",pid2);
	}
	else if (pid == 0){
		printf("get pid child pid: %d\n", pid2);
		printf("%d)child exit here",2);
		_exit(0);
	}
	

	printf("this should only print %d time\n",1);
	
	return 0;
}
