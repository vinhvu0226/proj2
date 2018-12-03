#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	printf("%d) fork then print\n",1);
	pid_t pid;
	pid = fork();
	printf("pid: %d\n",pid);
	
	return 0;
}
