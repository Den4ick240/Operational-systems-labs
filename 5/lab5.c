#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define READING_BUFFER_SIZE 8
#define MAX_NUMBER_OF_LINES 1024

int buildLinesTable(int fileDescriptor, int *lineOffsets);
void printLine(int fileDescriptor, int *lineOffsets, int lineNumber);
int readNumber(int numberOfLines);

int main (int argc, char **argv) {
	int fileDescriptor;
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
	
	numberOfLines = buildLinesTable(fileDescriptor, lineOffsets);

	while ((i = readNumber(numberOfLines)) != 0) {
		if (i == -1) {
			continue;
		}
		printf("line number %d:\n", i);
		printLine(fileDescriptor, lineOffsets, i);
	}
}

int buildLinesTable(int fileDescriptor, int *lineOffsets) {
	char readingBuffer[READING_BUFFER_SIZE];
	int bufferCounter, readedBytesNum, i, numberOfLines;
	
	numberOfLines = 0;
	bufferCounter = 0; 
	readedBytesNum = 0;
	i = 0;
	
	lineOffsets[0] = -1;
	
	while (1) {
		if (bufferCounter == readedBytesNum) {
			bufferCounter = 0;
			readedBytesNum = read(fileDescriptor, &readingBuffer, sizeof(char) * READING_BUFFER_SIZE);
		}
		if (readedBytesNum == 0 && errno == EINTR) {
			continue;
		}
		if (readedBytesNum == 0 || numberOfLines >= MAX_NUMBER_OF_LINES) {
			break;
		}
		if (readingBuffer[bufferCounter] == '\n') {
			numberOfLines++; 
			lineOffsets[numberOfLines] = i;
		}				
		i++;
		bufferCounter++;
	}
	if (lineOffsets[numberOfLines] != i - 1 && numberOfLines < MAX_NUMBER_OF_LINES) {
		numberOfLines++; 
		lineOffsets[numberOfLines] = i;
	}
	printf("File contains %d line%s\n", numberOfLines, numberOfLines == 1 ? "" : "s");
	return numberOfLines;
}

void printLine(int fileDescriptor, int *lineOffsets, int lineNumber) {
	char *line = NULL;
	int lineSize;
	int readedBytesNumber;
	
	lineSize = 	lineOffsets[lineNumber] - lineOffsets[lineNumber - 1];
	
	line = (char *)malloc(sizeof(char) * lineSize);
	if (line == NULL) {
		printf("Error: couldn't allocate memory of size %d\n", lineSize);
		perror(strerror(errno));
		return;
	}
	
	lseek(fileDescriptor, lineOffsets[lineNumber - 1] + 1, SEEK_SET);
	do {
		readedBytesNumber = read(fileDescriptor, line, sizeof(char) * (lineSize - 1));
	} while (readedBytesNumber == -1 && errno == EINTR);
	
	line[lineSize - 1] = 0;
	printf("%s\n", line);
	free(line);
}

int readNumber(int numberOfLines) {
	int scanfResult;
	int enteredNumber;
	
	do {
		scanfResult = scanf("%d", &enteredNumber);
	} while (scanfResult == EOF && errno == EINTR);
	
	if (scanfResult == EOF) {
		printf("Error: couldn't read from input stream, try again\n");
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
