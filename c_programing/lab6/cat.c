#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int fd, n;
    char buff[512];

    if (argc != 2) {
        printf("ENTER CORRECT ARGUMENTS :\n");
        return 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("ERROR");
        return 1;
    }

    while ((n = read(fd, buff, sizeof(buff))) > 0) {
        if (write(1, buff, n) != n) {
            perror("write");
            close(fd);
            return 1;
        }
    }

    if (n < 0) {
        perror("read");
    }

    close(fd);
    return 0;
}