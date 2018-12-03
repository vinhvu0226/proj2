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
#include <synch.h>

int sys__exit(int code, int *ret){

	lock_acquire(proctable[curthread->pid]->lk_p);
	
	//code = _MKWAIT_EXIT(code);
	proctable[curthread->pid]->exitcode = code;
	proctable[curthread->pid]->exited = 1;
	int pid;
	for (pid=0; pid <MAX_RUNNING_PROCESSES; pid++) {
		if (proctable[pid] == NULL){
			
		}
		else if(proctable[pid]->parentpid == curthread->pid){
			proctable[pid]->parentpid = -1;
		}
	}
	*ret = 0;
	lock_release(proctable[curthread->pid]->lk_p);
	thread_exit();
	return 0;
}
