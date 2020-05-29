#include "stdio.h"
#include "stdlib.h"

#define BUFF_SIZE 8

int main(int argc, char **argv) {
	FILE *fin = NULL, *pout = NULL;
	char buff[BUFF_SIZE];
	if (argc < 2) {
		printf("usage: %s fileName\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	fin = fopen(argv[1], "r");
	if (fin == NULL) {
		printf("Couldn't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	pout = popen("wc -l", "w");
	if (pout == NULL) {
		printf("Couldn't open pipe\n");
		if (fclose(fin) == -1) {
			printf("Couldn't close file %s\n", argv[1]);
		}
		exit(EXIT_FAILURE);
	}
	while (fgets(buff, BUFF_SIZE, fin) != NULL) {
		if (*buff == '\n') {
			fputs(buff, pout);
		}
	}
	if (fclose(fin) == -1) {
		printf("Couldn't close file %s\n", argv[1]);
	}
	if (pclose(pout) == -1) {
		printf("Couldn't close pipe\n");
	}
	exit(EXIT_SUCCESS);
}
