/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

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

void 
kfree_all(char *args[])
{
	int i;
	for (i=0; args[i]; i++)
		kfree(args[i]);

}

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int
execv(char *progname, char **args)
{
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;
    int num_args;
    char **args_in;
    char *kprog;
    unsigned int copy_addr;

	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, &v);
	if (result) {
		return result;
	}

    // if args don't exist, return EFAULT
    if(sizeof(args) == 0)
        return EFAULT;    
    num_args = sizeof(args)/sizeof(args[0]);

    // if the args are not NULL terminated, then there is not
    // enough memory allocated for them
    if(args[num_args-1] != NULL) {
        vfs_close(v);
        return ENOMEM;
    }

    // If the args cannot be allocated theres something wrong with virtual mem
    args_in = (char **)kmalloc(sizeof(char**) * num_args);
    if (args_in==NULL)
	{
		return ENOMEM;
	}

    // allocate space for args that will go into kernel space
    int i;
    for (i=0; i<num_args; i++)
	{
		args_in[i] = kmalloc(strlen(args[i])+1);
		args_in[i+1] = NULL;
		if (args_in[i]==NULL)
		{
			kfree_all(args_in);
			kfree(args_in);
			return -ENOMEM;
		}
		result = copyin(args[i], args_in[i], strlen(args[i])+1);
		if (result)
		{
			kfree_all(args_in);
			kfree(args_in);
			return -result;
		}

	}

    
    kprog = (char *) kmalloc(strlen(progname)+1);
    if (kprog==NULL)
    {
        return ENOMEM;
    }    

    result = copyin(progname, kprog, strlen(progname)+1);
    if (result)
	{
		kfree_all(args_in);
		kfree(args_in);
		kfree(kprog);
		return result;
	}

	/* We should be a new thread. */
    as_destroy(curthread->t_vmspace);
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
        as_destroy(curthread->t_vmspace);
        kfree_all(args_in);
        kfree(args_in);
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}




    /* set up stack with arguments here */
	char *new_args = (char *) kmalloc(sizeof(char *) * (num_args+1));
	if (new_args==NULL)
	{
		as_destroy(curthread->t_vmspace);
		kfree_all(args_in);
		kfree(args_in);
		return ENOMEM;
	}

    
	copy_addr = stackptr;

    // copy out from kernel to userland
    int arg_length;
    int tail;
	for (i=0; i<num_args; i++)
	{
		arg_length = strlen(args_in[i])+1;
		copy_addr -= arg_length;
		tail = 0;
		if (copy_addr & 0x3)
		{
			tail = copy_addr & 0x3;
			copy_addr -= tail;
		}
		copyout(args_in[i], copy_addr, arg_length);
		new_args[i] = copy_addr;
	}
	new_args[i] = NULL;
	copy_addr -= sizeof(char *) * (i+1);
	copyout(new_args, copy_addr, sizeof(char *) * (i+1));

	kfree_all(args_in);
	kfree(args_in);
	kfree(new_args);

	/* change name of current thread */
	curthread->t_name = kprog;


	/* Warp to user mode. */
	md_usermode(num_args, NULL /*userspace addr of args*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

