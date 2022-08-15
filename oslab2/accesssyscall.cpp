#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
int main (int argc, char** argv) {
	long result_exist, result_permission;
	printf("Path: %s\n", argv[1]);
	result_exist = access(argv[1], F_OK);
	result_permission = access(argv[1], X_OK);
	if (result_exist == -1) {
		printf("Invalid path\n");
	} else if (result_permission == -1) {
		printf("Access denied\n");
	} else {
		printf("Everything seems fine\n");
	}
	return 0;
}
