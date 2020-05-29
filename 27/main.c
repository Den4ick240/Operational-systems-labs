#include "stdio.h"
#include "stdlib.h"

#define BUFF_SIZE 8

int main(int argc, char **argv) {
	FILE *pin = NULL;
	char buff[BUFF_SIZE];
	sprintf(buff, "cat %s | grep '^$' | wc -l", argv[1]);
	pin = popen(buff, "r");
	if (pin == NULL) {
		printf("Couldn't open pipe\n");
		exit(EXIT_FAILURE);
	}
	int a;
	fscanf(pin, "%d", &a);
	printf("%d\n", a);
	if (pclose(pin) == -1) {
		printf("Couldn't close pipe\n");
	}
	exit(EXIT_SUCCESS);
}
