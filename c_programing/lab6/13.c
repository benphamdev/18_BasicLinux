#include<stdio.h>

#include<sys/stat.h>

#include<time.h>

int main(int argc, char * argv[]) {
  int i, j;
  struct stat a;
  for (i = 1; i < argc; i++) {
    printf("%s : ", argv[i]);
    stat(argv[i], & a);
    if (S_ISDIR(a.st_mode)) {
      printf("is a Directory file\n");
    } else {
      printf("is Regular file\n");
    }
    printf("******File Properties********\n");
    printf("Inode Number:%d\n", a.st_ino);
    printf("UID:%o\n", a.st_uid);
    printf("GID:%o\n", a.st_gid);
    printf("No of Links:%d\n", a.st_nlink);
    printf("Last Access time:%s", asctime(localtime( & a.st_atime)));
    printf("Permission flag:%o\n", a.st_mode % 512);
    printf("size in bytes:%d\n", a.st_size);
    printf("Blocks Allocated:%d\n", a.st_blocks);
    printf("Last modification time %s\n", ctime( & a.st_atime));
  }
}