#ifndef MY_UTILS_H
#define MY_UTILS_H

// Structure definition for storing user information
typedef struct {
    char first_name[100];  // First name of the user
    char *last_name;       // Last name of the user (dynamically allocated)
    char gender;           // Gender of the user ('F' or 'M')
    int age;               // Age of the user in years
} USER_RECORD;

// Function to print a hello message using a USER_RECORD
void print_hello_message(USER_RECORD *this_user);

// Function to remove the newline character from a string
char *remove_newline(char *s);

#endif

