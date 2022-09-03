#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int fd[2];
	int res = pipe(fd);
	int ret = fork();
	if (ret ==0) {
		char buffer[256];
		close(fd[1]);
		read(fd[0], buffer, 256);
		printf("Child process is: %d\n%s\n", getpid(), buffer);
	} else {
		close(fd[0]);
		write(fd[1], "Hello World!", 256);
		printf("Parent process is: %d\n", getpid());
	}
	return 0;
}
