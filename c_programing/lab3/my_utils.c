#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_utils.h"

// Function to print a hello message using the user's information
void print_hello_message(USER_RECORD *this_user) {
    printf("Hello, %s %s! You are %d years old and your gender is %c.\n", 
           this_user->first_name, this_user->last_name, 
           this_user->age, this_user->gender);
}

// Function to remove the newline character from a string
char *remove_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';  // Replace newline with null terminator
    }
    return s;  // Return the modified string
}

