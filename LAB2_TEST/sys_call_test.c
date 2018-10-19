#include "stdio.h"
#include <linux/unistd.h>
#include <sys/syscall.h>

#define __NR_mycounter 285
_syscall1(long, mycounter, unsigned long*, ct)


int main() {

   int i;
   unsigned long my_user_input = 0;

   for (i = 0; i < 10; i++) {
       mycounter(&my_user_input);
//        long res = syscall(__NR_mycounter, &my_user_input);
        
	fprintf(stdout, "in iteration %d: %ld\n", i, my_user_input);
   }

   return 0;
}
