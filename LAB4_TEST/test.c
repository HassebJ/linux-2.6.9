#include "stdio.h"
#include <linux/unistd.h>
#include <sys/syscall.h>

#define __NR_cp_range 285
//_syscall1(long, toggle_fss, int, is_enabled)
//_syscall1(long, toggle_fss_profiling, int, is_enabled)

int main() {


   int num_elements = 50;
   int size = 50 * sizeof(int);
   int *array = malloc(size);
   int i = 0;
   for(; i < num_elements; i++){
	   array[i] = i*2;
   }
	char * buf = malloc(20);

	long res = syscall(183, buf, 20);

	fprintf(stdout, "array: %lu, array+size-1: %lu", array, array+size-1);

    res = syscall(__NR_cp_range, array, array+size-1, 2);


   return 0;
}
