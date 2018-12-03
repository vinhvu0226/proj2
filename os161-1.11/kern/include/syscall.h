#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */
struct trapframe;
void syscall(struct trapframe *tf);

// support
void forkentry(void* data1, unsigned long ul);
// syscalls
int sys_reboot(int code);

int sys_getpid(int *ret);

int sys_fork(struct trapframe* parenttf, int *ret);

int sys_waitpid(int pid, int *status, int options, int *ret);

int sys_execv(char *progname, char **args);

int sys__exit(int code, int *ret);


#endif /* _SYSCALL_H_ */
