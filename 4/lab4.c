#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

#define MAX_STRING_LENGTH 4096

struct StringListNode {
    char *str;
    struct StringListNode *next;
};

void printAndDestroyList(struct StringListNode *head);

int main() {
    struct StringListNode *head = NULL,
            *buffNode = NULL;
    char inputBuffer[MAX_STRING_LENGTH];
	int outputCode = 0;

    printf("Enter some lines. Enter \".\" and programm will print all the lines you entered\n");

    while (1) {
        if (NULL == fgets(inputBuffer, MAX_STRING_LENGTH, stdin)) {
            outputCode = 1;
            perror(strerror(errno));
            break;
        }

        if (inputBuffer[0] == '.') {
            break;
        }

        buffNode = (struct StringListNode *) malloc(sizeof(struct StringListNode));
        if (buffNode == NULL) {
            outputCode = 1;
            perror(strerror(errno));
            break;
        }

        buffNode->str = strdup(inputBuffer);
        buffNode->next = head;
        head = buffNode;
    }

	printAndDestroyList(head);
    exit(outputCode);
}

void printAndDestroyList(struct StringListNode *head) {
	struct StringListNode *buffNode;
    while (head != NULL) {
        printf("%s", head->str);
        buffNode = head;
        head = head->next;
        free(buffNode->str);
        free(buffNode);
    }
}

