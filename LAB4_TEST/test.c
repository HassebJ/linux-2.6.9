#include "stdio.h"
#include <linux/unistd.h>
#include <sys/syscall.h>

#define __NR_inc_cp_range 286


int main() {


	int num_elements = 1024;
	int size = num_elements * sizeof(int);
	int *array = malloc(size);
	int i = 0;
	for(; i < num_elements; i++){
		array[i] = i*2;
	}

	fprintf(stderr, "inital checkpoint => array: %lu, array+size-1: %lu\n", array, array+size-1);

	long  res = syscall(__NR_inc_cp_range, array, array+size-1, 2);

	memset(array, 0x00000000, size);

	fprintf(stderr, "second checkpoint => array: %lu, array+size-1: %lu\n", array, array+size-1);

	res = syscall(__NR_inc_cp_range, array, array+size-1, 2);




	return 0;
}
