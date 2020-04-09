#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

#define MAX_STRING_LENGTH 4096

struct StringListNode {
    char *str;
    struct StringListNode *next;
};

int main() {
    struct StringListNode *head = NULL,
            *buffNode = NULL;
    char *inputBuffer = NULL;
    int outputCode = 0;

    printf("Enter some lines. Enter \".\" and programm will print all the lines you entered\n");

    inputBuffer = (char *) malloc(sizeof(char) * MAX_STRING_LENGTH);
    if (inputBuffer == NULL) {
        perror(strerror(errno));
        exit(0);
    }

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

        buffNode->str = (char *) malloc(sizeof(char) * (strlen(inputBuffer) + 1));
        if (buffNode->str == NULL) {
            outputCode = 1;
            free(buffNode);
            perror(strerror(errno));
            break;
        }
        strcpy(buffNode->str, inputBuffer);
        buffNode->next = head;
        head = buffNode;
    }

    free(inputBuffer);
    while (head != NULL) {
        printf("%s", head->str);
        buffNode = head;
        head = head->next;
        free(buffNode->str);
        free(buffNode);
    }

    exit(outputCode);
}
