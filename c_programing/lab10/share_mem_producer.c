// PRODUCER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
int main() {
 const int SIZE = 4096; /* buffersize (bytes) */
 const char *name = "OS"; /* shared memory object name */
 const char *data_0 = "Hello";
 const char *data_1 = "World!";
 int shm_fd; /* shared memory file descriptor */
 void *ptr;
 /* create the shared memory file descriptor */
 shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
 /* configure the size of the shared memory file */
 ftruncate(shm_fd, SIZE);
 /* memory map the shared memory file */
 ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
 /* write to the shared memory file */
 sprintf(ptr,"%s",data_0);
 ptr += strlen(data_0);
 sprintf(ptr,"%s",data_1);
 ptr += strlen(data_1);
 return 0;
}

