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
		printf("Usage: %s <file name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid == 0) {
		if (execl("/bin/cat", "cat", argv[1], (char*)0) == -1) {
			perror(strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	if (wait(&status) == -1) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("\nChild process with pid %ld finished\n", pid);
	exit(EXIT_SUCCESS);
}
