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
		dup2(fd[0], 0);
		execlp("wc", "wc", NULL);
	} else {
		close(fd[0]);
		dup2(fd[1], 1);
		execlp("ls", "ls", NULL);
	}
	return 0;
}
