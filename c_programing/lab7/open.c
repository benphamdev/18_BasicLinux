#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

int main(int argc, char * argv[]) {
  int fd;
  fd = open("luckfox.txt", O_CREAT | O_RDWR, 0666);
  if (fd == -1) {
    printf("open is error\n");
    return -1;
  }
  printf("fd is %d\n", fd);
  return 0;
}
