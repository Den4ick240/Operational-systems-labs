#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main() {
	static const int BUFF_SIZE = 8;
	int readBytes;
	char buffer[BUFF_SIZE];
	while ((readBytes = read(0, buffer, BUFF_SIZE)) != 0) {
		char *s;
		if (readBytes == -1) {
			if (errno == EINTR) continue;
			else break;
		}
		for (s = buffer; *s != 0; s++)
			*s = toupper(*s);
		if (write(1, buffer, readBytes) == -1) {
			perror(strerror(errno));
		}
	}
	if (readBytes == -1) {
	    perror(strerror(errno));
	    exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
