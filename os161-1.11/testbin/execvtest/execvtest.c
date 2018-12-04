#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    pid_t i = fork();
    if (i == 0)
    {
        execv("./prcs1", (char *[]){ "./prcs1", argv[1], NULL });
        _exit(1);
    }
    else if (i > 0)
    {
        execv("./prcs2", (char *[]){ "./prcs2", argv[0], NULL });
        _exit(2);
    }
    else
    {
        printf("fork failed");
        _exit(3);
    }
}
