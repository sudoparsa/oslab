#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *textfile;
	char *text;
	long  numbytes;
	textfile = fopen("/proc/version", "rb");
	if (textfile == NULL) {
		printf("Invalid Path\n");
		return 1;
	}
	fseek(textfile, 0L, SEEK_END);
	numbytes = ftell(textfile);
	fseek(textfile, 0L, SEEK_SET);
	text = (char*)calloc(numbytes, sizeof(char));
	if (text == NULL) {
		printf("Error while allocating\n");
		return 1;
	}
	fread(text, sizeof(char), numbytes, textfile);
	fclose(textfile);
	printf(text);
	printf("Salam\n");
	printf("%ld\n", numbytes);
	FILE *out = fopen("Linux_Version.txt", "w");
	fprintf(out, text);
	fclose(out);
	return 0;
}
