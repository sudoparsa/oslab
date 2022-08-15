#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
int main () {
	long result;
	result = syscall(441);
	printf("The result is %ld\n", result);
	return 0;
}
