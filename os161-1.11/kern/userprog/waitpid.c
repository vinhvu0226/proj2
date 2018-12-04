#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>
#include <machine/trapframe.h>
#include <synch.h>
#include <machine/vm.h>

int sys_waitpid(int pid, int *status, int options, int *ret){
	(void)options;
	int err;
	
	//  return error if pid is not valid... NEEDED?
	if (pid >= MAX_RUNNING_PROCESSES || pid < 0 || proctable[pid] == NULL ){
		*ret = -1;
		return EFAULT;
	}

	// return error if status is an invalid pointer
	if (status == NULL){
		*ret = -1;
		return EFAULT;
	}
	if ((vaddr_t)status >= (vaddr_t)USERTOP) {
		*ret = -1;
		return EFAULT;
	}
	if ((vaddr_t)status % 4 != 0){
		*ret = -1;
		return EFAULT;
	}

	// return error if options invalid or unsupported
	if (options !=0) {
		*ret = -1;
		return EINVAL;
	}
	// maybe not necessary
	if (curthread->pid != proctable[pid]->parentpid){
		*ret = -1;
		return EINVAL;
	}

	// wait for process with pid to exit
	if (proctable[pid]->exited == 0){
		lock_acquire(proctable[pid]->lk_p);
	}

	*ret = pid;
	
	err = copyout((const void*)&(proctable[pid]->exitcode), (userptr_t) status, sizeof(proctable[pid]->exitcode));
	if (err){
		*ret = -1;
		return err;
	}

	lock_release(proctable[pid]->lk_p);
	
	lock_destroy(proctable[pid]->lk_p);
	kfree(proctable[pid]);
	proctable[pid] = NULL;	
	return 0;
}
