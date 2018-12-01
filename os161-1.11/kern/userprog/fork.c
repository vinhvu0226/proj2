#include <types.h>
#include <kern/errno.h>
//include <kern/fcntl.h>
//#include <kern/wait.h>
//#include <copyinout.h>
#include <uio.h>
#include <lib.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <vfs.h>
#include <vnode.h>
#include <syscall.h>
#include <test.h>
#include <synch.h>
//#include <kern/seek.h>
//#include <stat.h>

/* system call to fork a process
 * returns pid of new child process for parent
 * returns 0 for child
 * if error, new process is not created, returns -1, errno is set
 */

int sys_fork(struct trapframe* parenttf, int *ret){
	// disable interrupts
	int spl = splhigh();
	int err = 0;

	// create structs to copy to
	struct thread *child;
	struct trapframe* childtf = (struct trapframe*)kmalloc(sizeof(struct trapframe));
	if (childtf ==NULL){
		splx(spl);
		return ENOMEM;
	}
	// copying trapframe
	*childtf = *parenttf;

	// copy address space
	struct addrspace* childas;
	err = as_copy(curthread->t_vmspace,&childas);
	if (err){
		splx(spl);
		return err;
	}
	// call threadfork
	err = thread_fork(curthread->t_name, childtf, (unsigned long) childas, forkentry, &child);
	if (err){
		splx(spl);
		return err;
	}
	
	// fix here
	proctable[child->pid]->parentpid = curthread->pid;
	*ret = (int) child->pid;
	splx(spl);
	
	return 0;
}

void forkentry(void* data, unsigned long unsignedlong){
	// parent's trapframe and address space
	struct trapframe *tf = (struct trapframe*) data;
	struct addrspace *as = (struct addrspace*) unsignedlong;
 			
	// create new child trapframe
	struct trapframe stacktf;
	stacktf = *tf;
	stacktf.tf_a3 = 0;
	// set the trapframe's tf_v0 to 0
	stacktf.tf_v0 = 0;
	// increment tf_epc by 4
	stacktf.tf_epc +=4;
	
	// copy the passed address space to the current process address space and activate
	as_copy(as,&curthread->t_vmspace);
	as_activate(curthread->t_vmspace);
	 																
	//  give control back to the usermode
	mips_usermode(&stacktf);
}
