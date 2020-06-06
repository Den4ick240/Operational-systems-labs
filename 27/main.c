#include "stdio.h"
#include "stdlib.h"

#define BUFF_SIZE 32

int main(int argc, char **argv) {
	FILE *pin = NULL;
	char buff[BUFF_SIZE];
	int pcloseStatus;
	
	if (argc < 2) {
	    printf("usage: %s fileName\n", argv[0]);
	    exit(EXIT_FAILURE);
	}
	
	snprintf(buff, BUFF_SIZE, "cat %s | grep '^$' | wc -l", argv[1]);
	pin = popen(buff, "r");
	if (pin == NULL) {
		printf("Couldn't open pipe\n");
		exit(EXIT_FAILURE);
	}
	int a;
	fscanf(pin, "%d", &a);
	pcloseStatus = pclose(pin);
	if (WIFEXITED(pcloseStatus) == 0 || WEXITSTATUS(pcloseStatus) != 0) {
		printf("Couldn't close pipe\n");
		exit(EXIT_FAILURE);
	}
	printf("result: %d\n", a);
	exit(EXIT_SUCCESS);
}
