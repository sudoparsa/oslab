#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
int main () {
	long result;
	result = syscall(__NR_mkdir, "testdir", 0777);
	printf("The result is %ld.\n", result);
	return 0;
}
