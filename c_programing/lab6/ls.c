#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void quit(char *message, int exit_code) {
    printf("%s\n", message);
    exit(exit_code);
}

int main(int argc, char **argv) {
    DIR *dirop;
    struct dirent *dired;
    char *dir_name;

    if (argc == 1) {
        // No arguments provided, list files in the current directory
        dir_name = ".";
    } else if (argc == 2) {
        // One argument provided, list files in the specified directory
        dir_name = argv[1];
    } else {
        quit("Invalid number of arguments", 1);
    }

    if ((dirop = opendir(dir_name)) == NULL) {
        quit("Cannot open directory", 1);
    }

    while ((dired = readdir(dirop)) != NULL) {
        printf("%10ld %s\n", dired->d_ino, dired->d_name);
    }

    closedir(dirop);
    return 0;
}