#include "sys/types.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "wait.h"
#include "errno.h"
#include "string.h"

int main(int argc, char **argv) {
	pid_t pid;
	int status;
	if (argc < 2) {
		printf("Usage: %s <executable name> <executable argument 1> ... \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid == 0) {
		if (execvp(argv[1], &argv[1]) == -1) {
			perror(strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	if (wait(&status) == -1) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("\nChild's status: %d\n", WEXITSTATUS(status));
	exit(EXIT_SUCCESS);
}
