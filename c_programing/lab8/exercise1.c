#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

/*
* Requires:
* This program's source code must reside in the current
* working directory in a readable file named "exercise1.c".
*
* Effects:
* Reads this program's source code, via mmap(), and prints
* that source code to stdout.
*/

int main(void) {
    struct stat statbuf;
    int fd, size;
    char *buf;

    // Open the file and get its size.
    fd = open("exercise1.c", O_RDONLY);
    if (fd < 0 || fstat(fd, &statbuf) < 0) {
        fprintf(stderr, "open() or fstat() failed.\n");
        return 1;
    }
    size = statbuf.st_size;

    // Map the file to a new virtual memory area.
    buf = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == MAP_FAILED) {
        fprintf(stderr, "mmap() failed.\n");
        close(fd);
        return 1;
    }

    // Print out the contents of the file to stdout.
    for (int i = 0; i < size; i++) {
        printf("%c", buf[i]);
    }

    // Clean up. Ignore the return values because we exit anyway.
    (void)munmap(buf, size);
    (void)close(fd);
    return 0;
}