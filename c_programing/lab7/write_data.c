#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd;
    ssize_t ret;
    char buf[] = "boys and girls\n hi,children!";
    char filename[] = "luckfox.txt";

    fd = open(filename, O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    printf("fd is %d\n", fd);
    ret = write(fd, buf, strlen(buf));
    if (ret == -1) {
        perror("write");
        close(fd);
        return -1;
    }

    printf("write %zd bytes to file %s\n", ret, filename);
    close(fd);
    return 0;
}