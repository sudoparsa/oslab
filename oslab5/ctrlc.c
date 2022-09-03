#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signum) {
	signal(SIGINT, SIG_DFL);
	printf("\nPress Ctrl-C again to terminate the program\n");
}

int main() {
	signal(SIGINT, sig_handler);
	printf("Looping forever...\n");
	while(1);
}

