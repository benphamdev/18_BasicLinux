#include <signal.h>
int raise(int sig);

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(void) {
    printf("raise before\n");
    raise(9);
    printf("raise after\n");
    return 0;
}
