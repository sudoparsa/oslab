#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signum) {
	printf("Handling...\n");
}

int main() {
	signal(SIGALRM, sig_handler);
	alarm(5);
	printf("Looping forever ...\n");
	pause();
	printf("This line should never be executed\n");
	return 0;
}

