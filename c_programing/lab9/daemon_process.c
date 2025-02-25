#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(void)
{
 pid_t pid;
 // Step 1: Create a new process
 pid = fork();
 // The parent process exits directly
 if (pid > 0)
 {
 exit(0);
 }
 if (pid == 0)
 {
 // Step 2: Call setsid function to detach from
the control terminal
 setsid();
 // Step 3: Change the working directory
 chdir("/");
 // Step 4: Reset the umask
 umask(0);
 // Step 5: Close file descriptors 0, 1, and 2
 for (int i = 1; i < 4; i++)
 {
 close(i);
 }
 while (1)
 {
 }
 }
 return 0;
}