#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_utils.h"

#define NUM_TIMES_TO_PRINT 3

int main(int argc, char *argv[])
{
    // (1) Declare variables;
    USER_RECORD myself;
    char temp_last_name[50];
    FILE *fp; // file pointer; used to read from pre-saved age.txt

    // (2) Ask user to type in her first name; read from standard input
    // What the user types and store up to 100 characters in member "first_name"
    // of object "myself";
    printf("Type your first name please and press Enter:\n");
    
    // Read 100 bytes from standard input;
    fgets(myself.first_name, sizeof(myself.first_name), stdin);
    
    // Remove the newline character from the end of the string;
    remove_newline(myself.first_name);

    // (3) Ask user to type in her last name; read from standard input
    // What the user types and store what the user types in member "last_name"
    // of object "myself";
    printf("Type your last name please and press Enter:\n");
    fgets(temp_last_name, sizeof(temp_last_name), stdin);
    
    // Remove the newline character from the end of the string;
    remove_newline(temp_last_name);

    // Allocate memory to store the last name; needed because last_name
    // variable inside the USER_RECORD structure is declared as a pointer only;
    myself.last_name = malloc((strlen(temp_last_name) + 1) * sizeof(char));
    strcpy(myself.last_name, temp_last_name);

    // (4) Ask user the gender;
    printf("What's your gender? Type F or M.\n");
    myself.gender = getc(stdin);

    // (5) In this example's directory we have created a text file
    // that contains only one line with an integer number; 
    // we mean for this number to be the age of the user;
    // we do this only to also illustrate reading from a file;
    fp = fopen("age.txt", "r"); // You should error-check this; not done here.
    fscanf(fp, "%d", &myself.age);
    fclose(fp);

    // (6) Entertain user;
    printf("---\n");
    long i;
    for (i = 0; i < NUM_TIMES_TO_PRINT; i++) {
        print_hello_message(&myself);
    }

    // (7) Clean up and return;
    free(myself.last_name);
    return 0;
}

