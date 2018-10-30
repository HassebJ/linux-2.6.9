#include "stdio.h"
#include <linux/unistd.h>
#include <sys/syscall.h>

#define __NR_cp_range 285
//_syscall1(long, toggle_fss, int, is_enabled)
//_syscall1(long, toggle_fss_profiling, int, is_enabled)

int main() {


	int num_elements = 64;
	int size = num_elements * sizeof(int);
	int *array = malloc(size);
	int i = 0;
	for(; i < num_elements; i++){
		array[i] = i;
	}

	fprintf(stderr, "inital checkpoint => array: %lu, array+size-1: %lu\n", array, array+size-1);

	long  res = syscall(__NR_cp_range, array, array+size-1, 2);


	for(; i < num_elements; i++){
		array[i] = i*2;
	}

	fprintf(stderr, "second checkpoint => array: %lu, array+size-1: %lu\n", array, array+size-1);

	res = syscall(__NR_cp_range, array, array+size-1, 2);




	return 0;
}
