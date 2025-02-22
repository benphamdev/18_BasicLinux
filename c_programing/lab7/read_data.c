#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

int main(int argc, char * argv[]) {
    int fd;
    char buf[32] = {
        0
    };
    ssize_t ret;
    fd = open("luckfox.txt", O_RDWR);
    if (fd == -1) {
        printf("open is error\n");
        return -1;
    }
    printf("fd is %d\n", fd);
    ret = read(fd, buf, 32);
    if (ret == -1) {
        printf("read is error\n");
        return -2;
    }
    printf("buf is %s", buf);
    printf("ret is %ld\n", ret);
    close(fd);
    return 0;
}
