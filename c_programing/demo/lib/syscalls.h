#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
    * Function to print a message to stdout
    * @param message The message to print
    * @return void
*/
void print_message(const char *message);

/**
    * Function to read a line of text from stdin
    * @return Pointer to the read line
*/
char *read_line(void);

#endif