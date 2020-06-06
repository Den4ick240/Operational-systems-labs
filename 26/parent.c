#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define LINE_NUM 3

int main() {
    FILE *fout = popen("./child.exe", "w");
    char *text[LINE_NUM] = { "TeXt1\n", "TEXT2\n", "text3\n" };
    int i, pcloseStatus;
    if (fout == NULL) {
	printf("Couldn't create pipe\n");
	exit(EXIT_FAILURE);
    }
    for (i = 0; i < LINE_NUM; i++) {
	if (fprintf(fout, "%s", text[i]) != strlen(text[i])) {
	    perror(strerror(errno));
	    break;
	}
    }
    pcloseStatus = pclose(fout);
    if (WIFEXITED(pcloseStatus) == 0 || WEXITSTATUS(pcloseStatus) != 0) {
	printf("Error, pclose exit status: %d\n", pcloseStatus);
	exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}

