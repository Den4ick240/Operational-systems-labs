#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_NUM 3

int main() {
    FILE *fout = popen("./child.exe", "w");
    char *text[LINE_NUM] = { "TeXt1\n", "TEXT2\n", "text3\n" };
    int i;
    if (fout == NULL) {
	printf("Couldn't create pipe\n");
	exit(EXIT_FAILURE);
    }
    for (i = 0; i < LINE_NUM; i++) {
	fprintf(fout, "%s", text[i]);
    }
    if (pclose(fout) == -1) {
	printf("Couldn't close the pipe\n");
	exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

