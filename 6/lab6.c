#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define WAIT_TIME 5
#define SIZE 1024
#define BUFF_SIZE 32

int buildFileStringTable(int *endls, int fd) {
	int n = 1, i = 1;
	char buff;
	endls[0] = 0;
	while (read(fd, &buff, sizeof(char) > 0 && n < SIZE)) {
		if (buff == '\n') {
			endls[n++] = i;
		}
		i++;
	}
	if (i - 1 != endls[n - 1]) {
		endls[n++] = i;
	}
	printf("File contains %d %s\n", n - 1, (n == 2 ? "line" : "lines"));
	return n;
}

void printString(int *endls, int id, int fd) {
	char *strBuff = NULL;
	int size = endls[id] - endls[id - 1];
	
	strBuff =  (char *)malloc(sizeof(char) * (size));
	if (strBuff == NULL) {
		perror(strerror(errno));
		return;
	}
	lseek(fd, endls[id - 1], SEEK_SET);
	read(fd, strBuff, sizeof(char) * (size - 1));
	strBuff[size - 1] = 0;
	printf("%s\n", strBuff);
	free(strBuff);
		
}

int scanNumber(int n) {
	int id, scanfResult = 0;
	scanfResult = scanf("%d", &id);

	if (scanfResult == 0 || id <= 0) {
		printf("Enter a number from 1 to %d\n", n - 1);
		while (scanfResult == 0 && getc(stdin) != '\n');
		return 0;
	}
	if (id >= n) {
		printf("Entered number is too big\n");
		return 0;
	}
	return id;
}

int waitAndRead(int n) {
	int fd, flags, id;
	char line[BUFF_SIZE];
	if ((fd = open("/dev/tty", O_RDONLY | O_NDELAY)) == -1) {
		perror("/dev/tty");
		return 0;
	}
	printf("Enter a string number from 1 to %d within %d seconds\n", 
		n-1, WAIT_TIME);
	sleep(WAIT_TIME);
	if (read(fd, line, BUFF_SIZE) == 0) {
		id = 0;
	}
	else {
		//flags = fcntl(fd, F_GETFL);
		//flags &= ~O_NDELAY;
		//fcntl(fd, F_SETFL, flags);
		close(fd);
		id = atoi(line);
	}
	if (id <= 0) {
		printf("\nSorry\n");
		return 0;
	}
	return id;
}

int main(int argc, char **argv) {
	int fd, terminalDescriptor;
	int endls[SIZE];
	int n, i;
	
	if (argc < 2) {
		printf("Enter a file name as an argument\n");
		exit(0);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror(strerror(errno));
		exit(1);
	}
	
	n = buildFileStringTable(endls, fd);
	
	if (n == 0) {
		return 0;
	}
	
	int id = waitAndRead(n);
	if (id == 0) {
		return 0;
	}
	printString(endls, id, fd);
	
	while (1) {
		id = scanNumber(n);
		if (id == 0) {
			continue;
		}
		printString(endls, id, fd);
	}
	return 0;
}
