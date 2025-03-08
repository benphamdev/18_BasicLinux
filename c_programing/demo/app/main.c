#include "../lib/syscalls.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Forward declarations
void *thread_function(void *arg);

// Demo for file operations
void demo_file_operations() {
    print_message("\n--- File Operations Demo ---\n");
    
    // Create a file
    int fd = sys_open("testfile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd != -1) {
        // Write to file
        const char *message = "Hello, System Call Library!\n";
        sys_write(fd, message, strlen(message));
        sys_close(fd);
        
        // Read from file
        fd = sys_open("testfile.txt", O_RDONLY, 0);
        if (fd != -1) {
            char buffer[128];
            ssize_t bytes_read = sys_read(fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                print_message("Read from file: ");
                print_message(buffer);
            }
            sys_close(fd);
        }
        
        // Get file info
        struct stat st;
        if (sys_stat("testfile.txt", &st) == 0) {
            printf("File size: %ld bytes\n", (long)st.st_size);
            printf("File permissions: %o\n", st.st_mode & 0777);
        }
    }
}

// Demo for process operations
void demo_process_operations() {
    print_message("\n--- Process Operations Demo ---\n");
    
    printf("Current process ID: %d\n", sys_getpid());
    
    pid_t pid = sys_fork();
    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d, Parent PID: %d)\n", sys_getpid(), getppid());
        sys_exit(42);  // Exit with code 42
    } else if (pid > 0) {
        // Parent process
        printf("Parent process (PID: %d) created child with PID: %d\n", sys_getpid(), pid);
        
        int status;
        sys_wait(&status);
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    }
}

// Signal handler function
static volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum) {
    signal_received = 1;
    printf("\nReceived signal %d (SIGINT)\n", signum);
}

// Demo for signal handling operations
void demo_signal_operations() {
    print_message("\n--- Signal Handling Operations Demo ---\n");
    
    // Reset the signal flag
    signal_received = 0;
    
    // Set up signal handler
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;  // Use our custom handler
    sys_sigaction(SIGINT, &act, NULL);
    
    printf("Signal handler registered. Press Ctrl+C to test...\n");
    printf("Waiting for signal (timeout: 10 seconds)...\n");
    
    // Wait for signal or timeout
    int count = 0;
    while (!signal_received && count < 10) {
        sys_nanosleep(&(struct timespec){1, 0}, NULL);  // Sleep for 1 second
        count++;
        printf(".");
        fflush(stdout);
    }
    
    if (!signal_received) {
        printf("\nTimeout reached. No signal received.\n");
    }
    
    // Reset signal handler to default
    act.sa_handler = SIG_DFL;
    sys_sigaction(SIGINT, &act, NULL);
    printf("Signal handler reset to default.\n");
}

// Demo for memory management operations
void demo_memory_operations() {
    print_message("\n--- Memory Management Operations Demo ---\n");
    
    // Allocate memory
    void *ptr = sys_mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr != MAP_FAILED) {
        printf("Allocated memory at address: %p\n", ptr);
        
        // Write to memory
        strcpy(ptr, "Hello, Memory Management!");
        printf("Wrote to memory: %s\n", (char *)ptr);
        
        // Deallocate memory
        sys_munmap(ptr, 4096);
        printf("Deallocated memory\n");
    }
}

// Demo for directory operations
void demo_directory_operations() {
    print_message("\n--- Directory Operations Demo ---\n");
    
    // Get current working directory
    char cwd[256];
    if (sys_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    
    // Create a directory
    if (sys_mkdir("testdir", 0755) == 0) {
        printf("Created directory: testdir\n");
        
        // Change to that directory
        if (sys_chdir("testdir") == 0) {
            if (sys_getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("New working directory: %s\n", cwd);
            }
            
            // Go back to parent directory
            if (sys_chdir("..") == 0) {
                printf("Changed back to parent directory\n");
                
                // Remove the directory
                if (sys_rmdir("testdir") == 0) {
                    printf("Removed directory: testdir\n");
                }
            }
        }
    }
}

// Demo for networking operations
void demo_networking_operations() {
    print_message("\n--- Networking Operations Demo ---\n");
    
    // Create a socket
    int sockfd = sys_socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd != -1) {
        printf("Created socket: %d\n", sockfd);
        
        // Bind to a port
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (sys_bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            printf("Bound to port 8080\n");
            
            // Listen for connections
            if (sys_listen(sockfd, 10) == 0) {
                printf("Listening for connections...\n");
                
                // Accept a connection
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int clientfd = sys_accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
                if (clientfd != -1) {
                    printf("Accepted connection from client\n");
                    
                    // Send data
                    const char *message = "Hello, Networking!";
                    sys_send(clientfd, message, strlen(message), 0);
                    
                    // Close connection
                    sys_close(clientfd);
                }
            }
        }
        
        // Close socket
        sys_close(sockfd);
    }
}

// Thread function implementation
void *thread_function(void *arg) {
    (void)arg; // Silence unused parameter warning
    printf("Thread is running\n");
    return NULL;
}

// Demo for thread operations
void demo_thread_operations() {
    print_message("\n--- Thread Operations Demo ---\n");
    
    // Create a thread
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_function, NULL) == 0) {
        printf("Created thread\n");
        
        // Wait for thread to finish
        pthread_join(thread, NULL);
        printf("Thread finished\n");
    }
}

// Demo for time operations
void demo_time_operations() {
    print_message("\n--- Time Operations Demo ---\n");
    
    struct timeval tv;
    if (sys_gettimeofday(&tv, NULL) == 0) {
        printf("Current time: %ld.%06ld seconds since epoch\n", 
               (long)tv.tv_sec, (long)tv.tv_usec);
    }
    
    printf("Sleeping for 1 second...\n");
    struct timespec ts = {1, 0};  // 1 second
    sys_nanosleep(&ts, NULL);
    printf("Awake!\n");
}

// Demo for resource limit operations
void demo_resource_limit_operations() {
    print_message("\n--- Resource Limit Operations Demo ---\n");
    
    struct rlimit rlim;
    if (sys_getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("Current file descriptor limit: %ld\n", (long)rlim.rlim_cur);
        
        rlim.rlim_cur = 1024;
        if (sys_setrlimit(RLIMIT_NOFILE, &rlim) == 0) {
            printf("Set file descriptor limit to 1024\n");
        }
    }
}

// Demo for file permission operations
void demo_file_permission_operations() {
    print_message("\n--- File Permission Operations Demo ---\n");
    
    // Change file permissions
    if (sys_chmod("testfile.txt", 0644) == 0) {
        printf("Changed file permissions to 0644\n");
    }
    
    // Change file owner
    if (sys_chown("testfile.txt", 1000, 1000) == 0) {
        printf("Changed file owner to user 1000, group 1000\n");
    }
    
    // Change file group
    if (sys_chown("testfile.txt", -1, 1000) == 0) {
        printf("Changed file group to 1000\n");
    }
}

// Demo for asynchronous I/O operations
void demo_async_io_operations() {
    print_message("\n--- Asynchronous I/O Operations Demo ---\n");
    
    // Prepare data structure
    struct aiocb aiocb;
    memset(&aiocb, 0, sizeof(aiocb));
    aiocb.aio_fildes = sys_open("testfile.txt", O_RDONLY, 0);
    aiocb.aio_buf = malloc(256);
    aiocb.aio_nbytes = 256;
    
    // Read asynchronously
    if (sys_aio_read(&aiocb) == 0) {
        printf("Reading asynchronously from file...\n");
        
        // Wait for completion
        while (sys_aio_error(&aiocb) == EINPROGRESS) {
            printf("Waiting for completion...\n");
            struct timespec timeout = {0, 100000000};  // 0.1 second
            
            // Fix: Create the proper list of aiocb pointers
            const struct aiocb *list[1] = {&aiocb};
            sys_aio_suspend(list, 1, &timeout);
        }
        
        // Check result
        ssize_t bytes_read = sys_aio_return(&aiocb);
        if (bytes_read > 0) {
            // Fix: Cast away volatile to avoid warning
            char *buffer = (char *)aiocb.aio_buf;
            buffer[bytes_read] = '\0';
            printf("Read asynchronously: %s\n", buffer);
        }
    }
    
    // Clean up - cast to avoid volatile warning
    void *buf_to_free = (void *)aiocb.aio_buf;
    free(buf_to_free);
    sys_close(aiocb.aio_fildes);
}

// Demo for pipe operations
void demo_pipe_operations() {
    print_message("\n--- Pipe Operations Demo ---\n");
    
    int pipefd[2];
    if (sys_pipe(pipefd) == 0) {
        pid_t pid = sys_fork();
        
        if (pid == 0) {
            // Child process - read from pipe
            sys_close(pipefd[1]);  // Close write end
            
            char buffer[256];
            ssize_t bytes_read = sys_read(pipefd[0], buffer, sizeof(buffer) - 1);
            sys_close(pipefd[0]);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("Child received: %s\n", buffer);
            }
            
            sys_exit(0);
            
        } else if (pid > 0) {
            // Parent process - write to pipe
            sys_close(pipefd[0]);  // Close read end
            
            const char *message = "Hello from parent via pipe!";
            sys_write(pipefd[1], message, strlen(message));
            sys_close(pipefd[1]);
            
            // Wait for child to finish
            int status;
            sys_wait(&status);
        }
    }
}

void diplay_menu() {
    print_message("\n------ DEMO MENU ------\n");
    print_message("1. File Operations Demo\n");
    print_message("2. Process Operations Demo\n");
    print_message("3. Directory Operations Demo\n");
    print_message("4. Pipe Operations Demo\n");
    print_message("5. Time Operations Demo\n");
    print_message("6. Signal Handling Demo\n");
    print_message("7. Memory Management Demo\n");
    print_message("8. File Permission Demo\n");
    print_message("9. Basic I/O Demo\n");
    print_message("0. Exit Program\n");
    print_message("----------------------\n");
    print_message("Enter your choice: ");
}

int main() {
    int choice = 0;
    char input[10];
    
    // Display program header
    print_message("\n===== System Call Library Demo =====\n");
    print_message("This program demonstrates various system call wrappers\n\n");
    
    while (1) {
        // Display menu
        diplay_menu();
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Convert input to integer
        choice = atoi(input);
        
        // Process user choice
        switch (choice) {
            case 1:
                demo_file_operations();
                break;
            case 2:
                demo_process_operations();
                break;
            case 3:
                demo_directory_operations();
                break;
            case 4:
                demo_pipe_operations();
                break;
            case 5:
                demo_time_operations();
                break;
            case 6:
                demo_signal_operations();
                break;
            case 7:
                demo_memory_operations();
                break;
            case 8:
                demo_file_permission_operations();
                break;
            case 9:
                print_message("\n--- Basic I/O Demo ---\n");
                print_message("Enter a message: ");
                char *message = read_line();
                print_message("You entered: ");
                print_message(message);
                free(message); // Free allocated memory
                break;
            case 0:
                print_message("\nExiting System Call Library Demo. Goodbye!\n");
                return 0;
            default:
                print_message("\nInvalid choice. Please try again.\n");
                break;
        }
        
        // Pause after each demo (except exit)
        if (choice != 0) {
            print_message("\nPress Enter to return to menu...");
            getchar(); // Consume any remaining newline from previous input
            fgets(input, sizeof(input), stdin);
        }
    }
    
    return 0;
}