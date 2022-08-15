#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
int main (int argc, char ** argv) {
	long result;
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	result = syscall(442, a, b);
	printf("The result is %d\n", result);
	return 0;
}
