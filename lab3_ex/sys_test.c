
#include "stdio.h"
#include <linux/unistd.h>
#include <sys/syscall.h>

#define __NR_mycounter 286
//_syscall1(long, toggle_fss, int, is_enabled)
//_syscall1(long, toggle_fss_profiling, int, is_enabled)

int main() {


       long res = syscall(__NR_mycounter, 1);
   
//   toggle_fss_profiling(1);
   fprintf(stdout, "Profiling enabled: sleeping for 60 seconds");
   sleep(60);
	res = syscall(__NR_mycounter, 0);

//   toggle_fss_profiling(0);
   return 0;
}