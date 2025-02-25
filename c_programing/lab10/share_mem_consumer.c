// CONSUMER 


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
int main() {
 const int SIZE = 4096; /* buffersize (bytes) */
 const char *name = "OS"; /* shared memory object name */
 int shm_fd; /* file descriptor */
 void *ptr;
 /* open the shared memory file */
 shm_fd = shm_open(name, O_RDONLY, 0666);
 /* memory map the shared memory file */
 ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
 /* read from the shared memory file */

 printf("%s",(char *)ptr);
 /* remove the shared memory file */
 shm_unlink(name);
 return 0;
}

