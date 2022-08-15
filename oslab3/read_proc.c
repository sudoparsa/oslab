#include <stdio.h>
#include <stdlib.h>

int main() {
	char ch;
	FILE* textfile = textfile = fopen("/proc/version", "r");
	FILE* outfile = fopen("Linux_Version.txt", "w");
	if (textfile == NULL) {
		printf("Invalid Path\n");
		return 1;
	}
	do {
		ch = fgetc(textfile);
		fprintf(outfile, "%c", ch);
	} while (ch!=EOF);
	fclose(textfile);
	fclose(outfile);
	printf("Done\n");
	return 0;
}
