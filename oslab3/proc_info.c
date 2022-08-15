#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_file(char* path, char* string) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Invalid Path\n");
		return 1;
	}
	char ch;
	int i=0;
	do {
		ch = fgetc(file);
		if (ch == '\000')
			ch = '\n';
		string[i] = ch;
		if (ch != -1)
			i += 1;
	} while (ch!=EOF);
	string[i] = 0;
	fclose(file);
	return 0;
}


int main(int argc, char* argv[]) {
	printf("PID: %s\n\n", argv[1]);
	char path[] = "/proc/";
	const char* pid = argv[1];
	char buffer[10000];
	char* a = malloc(256);

	char comm[] = "/comm";
	strcpy(a, path);
	strcat(a, pid);
	strcat(a, comm);
	read_file(a, buffer);
	printf("Name: %s\n", buffer);

	char status[] = "/status";
	strcpy(a, path);
	strcat(a, pid);
	strcat(a, status);
	read_file(a, buffer);
	char vmsize[] = "VmSize";
	char* ptr = strstr(buffer, vmsize);
	if (ptr == NULL)
		printf("VmSize: 0 kB\n\n");
	else {
		char en[] = "\n";
		char* end = strstr(ptr, en);
		end[0] = 0;
		printf("%s\n\n", ptr);
	}

	char cmdline[] = "/cmdline";
	strcpy(a, path);
	strcat(a, pid);
	strcat(a, cmdline);
	read_file(a, buffer);
	printf("Cmdline:\n%s\n", buffer);

	char environ[] = "/environ";
	strcpy(a, path);
	strcat(a, pid);
	strcat(a, environ);
	read_file(a, buffer);
	printf("Environ:\n%s\n", buffer);

	return 0;
}
