#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LINE_NUM 3

void parent(int pipefd[2]);
void child(int pipefd[2]);

int main() {
	int pid;
    int pipefd[2];
	
	if (pipe(pipefd) == -1) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("pipe created\n");
	pid = fork();
	
	if (pid == 0) {
		parent(pipefd);
	}
	else if (pid > 0) {
		child(pipefd);
	}
	else {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}

void parent(int pipefd[2]) {
	char *text[LINE_NUM] = { "TeXt1\n", "TEXT2\n", "text3\n" };
	
	if (close(pipefd[0]) == -1) {
		perror(strerror(errno));
	}

	for (int i = 0; i < LINE_NUM; i++) 
		write(pipefd[1], text[i], strlen(text[i]));
	
	if (close(pipefd[1]) == -1)
		perror(strerror(errno));

}

void child(int pipefd[2]) {
	static const int BUFF_SIZE = 8;
	int readBytes;
	char buffer[BUFF_SIZE];
	
	if (close(pipefd[1]) == -1)
		perror(strerror(errno));
	
	while ((readBytes = read(pipefd[0], buffer, BUFF_SIZE)) > 0) {
		for (char *s = buffer; *s != 0; s++) 
			*s = toupper(*s);
		printf("%s", buffer);
	}
	
	if (readBytes == -1) 
		perror(strerror(errno));
	
	if (close(pipefd[0]) == -1)
		perror(strerror(errno));
}
