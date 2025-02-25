#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void handler(int sig) { printf("Received signal %d\n", sig); }
int main() {
    signal(SIGALRM, handler);  // Register the
    signal handler alarm(5);   // Set a timer,
    trigger SIGALRM signal after 5 seconds printf(
        "Waiting for alarm to go off...\n");
    pause();  // Block the process and wait for the signal to be triggered
    printf("Exiting...\n");
return 0;
}
