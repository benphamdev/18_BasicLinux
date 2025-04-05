#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../src/web_server.h"

static struct MHD_Daemon *web_daemon = NULL;

// Signal handler to handle Ctrl+C and graceful shutdown
void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nShutting down web server...\n");
        if (web_daemon != NULL) {
            stop_web_server(web_daemon);
            web_daemon = NULL;
        }
        exit(0);
    }
}

int main() {
    // Set up signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Could not set up signal handler");
        return 1;
    }
    
    printf("\n===== System Call Library Web Demo =====\n");
    printf("Starting web server on port %d...\n", SERVER_PORT);
    
    // Initialize and start the web server
    web_daemon = init_web_server();
    if (web_daemon == NULL) {
        fprintf(stderr, "Failed to initialize web server\n");
        return 1;
    }
    
    printf("Web server running at http://localhost:%d\n", SERVER_PORT);
    printf("Press Ctrl+C to quit\n");
    
    // Keep the main thread running
    while (1) {
        sleep(1);
    }
    
    return 0;
}