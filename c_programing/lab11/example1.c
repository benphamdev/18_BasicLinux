#include <signal.h>
#include <stdio.h>
void signal_handler_fun(int signum) { printf("catch signal %d\n", signum); }
int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler_fun);
    while (1);
    return 0;
}
