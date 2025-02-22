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
    fd = open("luckfox.txt", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        printf("open is error\n");
        return -1;
    }
    ret = read(fd, buf, 8);
    if (ret < 0) {
        printf("read is error\n");
        return -2;
    }
    printf("buf is %s\n", buf);
    printf("ret is %ld\n", ret);
    ret = lseek(fd, 0, SEEK_CUR);
    printf("ret is %ld\n", ret);
    close(fd);
    return 0;
}
