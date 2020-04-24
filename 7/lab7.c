#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAX_NUMBER_OF_LINES 1024
#define TIMEOUT 5000

int buildLinesTable(char* fileMap, off_t fileMapSize, int *lineOffsets);
void printLine(char *fileMap, off_t fileMapSize, int *lineOffsets, int lineNumber);
int readNumber(int numberOfLines);
int readNumberWithTimeLimit(int numberOfLines);

int main (int argc, char **argv) {
	int fileDescriptor;
	char *fileMap = NULL;
	off_t fileMapSize;
	int lineOffsets[MAX_NUMBER_OF_LINES + 1];
	int numberOfLines, i = 0;
	
	if (argc < 2) {
		printf("usage: %s file_name\n", argv[0]);
		exit(0);
	}
	
	fileDescriptor = open(argv[1], O_RDONLY);
	if (fileDescriptor == -1) {
		printf("Couldn't open file");
		perror(strerror(errno));
		exit(1);
	}
	
	fileMapSize = lseek(fileDescriptor, 0, SEEK_END);
	fileMap = (char*) mmap(0, fileMapSize, PROT_READ, MAP_SHARED, fileDescriptor, 0);
	if (fileMap == NULL) {
		perror(strerror(errno));
		exit(1);
	}
	
	numberOfLines = buildLinesTable(fileMap, fileMapSize, lineOffsets);

	if ((i = readNumberWithTimeLimit(numberOfLines)) < 1) {
		return 0; 
	}
	printLine(fileMap, fileMapSize, lineOffsets, i);
	
	while ((i = readNumber(numberOfLines)) != 0) {
		if (i == -1) {
			continue;
		}
		printLine(fileMap, fileMapSize, lineOffsets, i);
	}
}

int buildLinesTable(char* fileMap, off_t fileMapSize, int *lineOffsets) {
	int i, numberOfLines;
	
	numberOfLines = 0;
	i = 0;
	
	lineOffsets[0] = -1;
	
	while (1) {
		if (i == fileMapSize || numberOfLines >= MAX_NUMBER_OF_LINES) {
			break;
		}
		if (fileMap[i] == '\n') {
			numberOfLines++; 
			lineOffsets[numberOfLines] = i;
		}				
		i++;
	}
	if (lineOffsets[numberOfLines] != i - 1 && numberOfLines < MAX_NUMBER_OF_LINES) {
		numberOfLines++; 
		lineOffsets[numberOfLines] = i;
	}
	printf("File contains %d line%s\n", numberOfLines, numberOfLines == 1 ? "" : "s");
	return numberOfLines;
}

void printLine(char* fileMap, off_t fileMapSize, int *lineOffsets, int lineNumber) {
	int lineSize = lineOffsets[lineNumber] - lineOffsets[lineNumber - 1];
	int lineStart = 1 + lineOffsets[lineNumber - 1];
	
	write(STDOUT_FILENO, fileMap + lineStart, sizeof(char) * lineSize);
	printf("\n");
}

int readNumber(int numberOfLines) {
	int scanfResult;
	int enteredNumber;
	
	do {
		scanfResult = scanf("%d", &enteredNumber);
	} while (scanfResult == EOF && errno == EINTR);
	
	if (scanfResult == EOF) {
		printf("Error: couldn't read from input stream\n");
		perror(strerror(errno));
		return -1;
	}
	
	if (scanfResult == 0 || enteredNumber < 0 || numberOfLines < enteredNumber) {
		printf("Bad input, enter a number from 1 to %d, or enter 0 to exit\n", numberOfLines);
		while (scanfResult == 0 && getc(stdin) != '\n');
		return -1;
	}
	return enteredNumber;
}

int readNumberWithTimeLimit(int numberOfLines) {
	int pollRet;
	struct pollfd fds;
	do {
		printf("Enter a line number within %lf seconds\n", (double)TIMEOUT / 1000);
		fds.fd = STDIN_FILENO;
		fds.events = POLLIN;
		pollRet = poll(&fds, 1, TIMEOUT);
		if (pollRet == -1 && errno == EINTR) {
			continue;
		}
		if (pollRet == -1) {
			perror(strerror(errno));
			return -1;
		}
		if (pollRet == 0) {
			printf("Time is out, sorry");
		}
		return readNumber(numberOfLines);
	} while (0);
}
