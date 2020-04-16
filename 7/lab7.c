#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#define WAIT_TIME 5
#define SIZE 1024
#define BUFF_SIZE 32

int buildFileStringTable(int *endls, char *fileMap, int fileSize) {
	int n = 1, i;
	endls[0] = 0;
	for (i = 0; i < fileSize && n < SIZE; i++) {
		if (fileMap[i] == '\n') {
			endls[n++] = i + 1;
		}
	}
	if (i != endls[n - 1]) {
		endls[n++] = i;
	}
	printf("File contains %d %s\n", n - 1, (n == 2 ? "line" : "lines"));
	return n;
}

void printString(int *endls, int id, char *fileMap) {
	char *strBuff = NULL;
	int size = endls[id] - endls[id - 1] - 1;
	int i;
	
	strBuff =  (char *)malloc(sizeof(char) * (size));
	if (strBuff == NULL) {
		perror(strerror(errno));
		return;
	}
	fwrite(fileMap + sizeof(char) * endls[id - 1], sizeof(char), (size), stdout);
	printf("\n");	
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
		id = atoi(line);
	}
	
	close(fd);
	if (id <= 0) {
		printf("\nSorry\n");
		return 0;
	}
	return id;
}

int main(int argc, char **argv) {
	int fd;
	int endls[SIZE];
	int n, i;
	char * fileMap = NULL;
	int fileSize;
	
	if (argc < 2) {
		printf("Enter a file name as an argument\n");
		exit(0);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror(strerror(errno));
		exit(1);
	}
	
	fileSize = lseek(fd, 0, SEEK_END);
	fileMap = (char*) mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);
	if (fileMap == NULL) {
		perror(strerror(errno));
		exit(1);
	}
	
	n = buildFileStringTable(endls, fileMap, fileSize);
	
	if (n == 0) {
		return 0;
	}
	
	int id = waitAndRead(n);
	if (id == 0) {
		return 0;
	}
	printString(endls, id, fileMap);
	
	while (1) {
		id = scanNumber(n);
		if (id == 0) {
			continue;
		}
		printString(endls, id, fileMap);
	}
	close(fd);
	return 0;
}
