#include <linux/module.h>
#include <linux/timex.h>
#include <linux/errno.h>
#include <linux/smp_lock.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
/*
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/param.h>
*/



/**
 * sys_getpid - return the thread group id of the current process
 *
 * Note, despite the name, this returns the tgid not the pid.  The tgid and
 * the pid are identical unless CLONE_THREAD was specified on clone() in
 * which case the tgid is the same in all threads of the same group.
 *
 * This is SMP safe as current->tgid does not change.
 */
/*
asmlinkage long sys_mycounter(unsigned long *ct)
{

   unsigned long *my_kernel_tmp;

   my_kernel_tmp = malloc(sizeof(unsigned long));

   copy_from_user(my_kernel_tmp, ct, sizeof(unsigned long));

   (*my_kernel_tmp)++;

   copy_to_user(ct, my_kernel_tmp, sizeof(unsigned long));

   return (*my_kernel_tmp);
}
*/


asmlinkage long sys_mycounter(unsigned long *ct)
{

   unsigned long my_kernel_tmp;

   //my_kernel_tmp = malloc(sizeof(unsigned long));

   copy_from_user(&my_kernel_tmp, ct, sizeof(unsigned long));

   my_kernel_tmp++;

   copy_to_user(ct, &my_kernel_tmp, sizeof(unsigned long));

   return my_kernel_tmp;
}
