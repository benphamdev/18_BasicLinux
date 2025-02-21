#include "syscalls.h"
#include <string.h>  // Add this for strlen()

void print_message(const char *message) {
    if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

char *read_line(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        perror("getline");
        exit(EXIT_FAILURE);
    }

    return line;
}