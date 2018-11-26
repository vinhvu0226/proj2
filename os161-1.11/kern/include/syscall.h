#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);

pid_t sys_getpid(int *ret);

int sys_fork(struct trapframe* parenttf, int *ret);
#endif /* _SYSCALL_H_ */
