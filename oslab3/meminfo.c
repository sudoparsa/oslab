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

void find_string(char buffer[], char string[]) {
	char en[] = "\n";
	char *end, *ptr;
	ptr = strstr(buffer, string);
	end = strstr(ptr, en);
	char ch = end[0];
	end[0] = 0;
	printf("%s\n\n", ptr);
	end[0] = ch;
}


int main(int argc, char* argv[]) {
	char path[] = "/proc/meminfo";
	char buffer[20000];
	read_file(path, buffer);
	printf("\n");

	char memtotal[] = "MemTotal";
	find_string(buffer, memtotal);

	char memfree[] = "MemFree";
	find_string(buffer, memfree);

	char cached[] = "Cached";
	find_string(buffer, cached);

	char cm[] = "Committed_AS";
	find_string(buffer, cm);

	return 0;
}
