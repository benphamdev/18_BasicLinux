#include "../lib/syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <poll.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sched.h>

// Forward declarations
void *thread_function(void *arg);
// Add signal handler declaration
void signal_handler(int signum);

// Static variable for signal handling
static volatile sig_atomic_t signal_received = 0;

// Signal handler function implementation
void signal_handler(int signum) {
    signal_received = 1;
    printf("\nReceived signal %d (SIGINT)\n", signum);
}

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

// Demo for advanced file operations
void demo_advanced_file_operations() {
    print_message("\n--- Advanced File Operations Demo ---\n");
    
    // Create a file for demonstrations
    int fd = sys_open("advanced_file_test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        return;
    }
    
    printf("File created successfully\n");
    
    // Pre-allocate space for the file (20KB)
    if (sys_fallocate(fd, 0, 0, 20480) == 0) {
        printf("Pre-allocated 20KB for file\n");
        
        // Get file status to verify size
        struct stat st;
        if (sys_fstat(fd, &st) == 0) {
            printf("File size after allocation: %ld bytes\n", (long)st.st_size);
        }
        
        // Write some data
        const char *data = "This is test data for advanced file operations.";
        if (sys_write(fd, data, strlen(data)) != -1) {
            printf("Successfully wrote data to file\n");
            
            // Sync the data to disk
            if (sys_fdatasync(fd) == 0) {
                printf("Data successfully synced to disk\n");
            }
            
            // Truncate the file to just 50 bytes
            if (sys_ftruncate(fd, 50) == 0) {
                printf("File truncated to 50 bytes\n");
                
                // Sync all metadata to disk
                if (sys_fsync(fd) == 0) {
                    printf("File completely synced to disk\n");
                }
            }
        }
    }
    
    // Advise the kernel about access patterns
    if (sys_posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL) == 0) {
        printf("Advised kernel about sequential access pattern\n");
    }
    
    // Close the file
    sys_close(fd);
    
    // File copy using copy_file_range
    int src_fd = sys_open("advanced_file_test.txt", O_RDONLY, 0);
    int dst_fd = sys_open("advanced_file_copy.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (src_fd != -1 && dst_fd != -1) {
        ssize_t bytes = sys_copy_file_range(src_fd, NULL, dst_fd, NULL, 50, 0);
        if (bytes > 0) {
            printf("Copied %ld bytes from source to destination using copy_file_range\n", (long)bytes);
        }
        
        sys_close(src_fd);
        sys_close(dst_fd);
    }
    
    // Clean up files
    unlink("advanced_file_test.txt");
    unlink("advanced_file_copy.txt");
    printf("Files cleaned up\n");
}

// Demo for extended attributes
void demo_extended_attributes() {
    print_message("\n--- Extended Attributes Demo ---\n");
    
    // Create a test file
    int fd = sys_open("xattr_test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        return;
    }
    
    const char *data = "This is a test file for extended attributes.";
    sys_write(fd, data, strlen(data));
    sys_close(fd);
    
    printf("Test file created\n");
    
    // Set an extended attribute
    const char *attr_value = "test_value";
    if (sys_setxattr("xattr_test.txt", "user.comment", attr_value, strlen(attr_value), 0) == 0) {
        printf("Set extended attribute 'user.comment' to '%s'\n", attr_value);
        
        // Get the extended attribute
        char buffer[64];
        ssize_t len = sys_getxattr("xattr_test.txt", "user.comment", buffer, sizeof(buffer));
        if (len > 0) {
            buffer[len] = '\0';
            printf("Read extended attribute 'user.comment': '%s'\n", buffer);
        }
        
        // List all extended attributes
        char list[256];
        ssize_t list_len = sys_listxattr("xattr_test.txt", list, sizeof(list));
        if (list_len > 0) {
            printf("Extended attributes for file:\n");
            int i = 0;
            while (i < list_len) {
                printf("  %s\n", list + i);
                i += strlen(list + i) + 1;
            }
        }
        
        // Remove the extended attribute
        if (sys_removexattr("xattr_test.txt", "user.comment") == 0) {
            printf("Removed extended attribute 'user.comment'\n");
        }
    }
    
    // Clean up
    unlink("xattr_test.txt");
    printf("Test file removed\n");
}

// Demo for advanced memory management
void demo_advanced_memory_management() {
    print_message("\n--- Advanced Memory Management Demo ---\n");
    
    // Allocate some memory
    size_t pagesize = sysconf(_SC_PAGESIZE);
    size_t region_size = pagesize * 4; // 4 pages
    
    void *ptr = sys_mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        printf("Failed to allocate memory region\n");
        return;
    }
    
    printf("Allocated memory region of %zu bytes at %p\n", region_size, ptr);
    
    // Write some data to the first page
    memset(ptr, 'A', pagesize);
    printf("Wrote data to first page\n");
    
    // Change memory protection for the second page to read-only
    void *second_page = (char*)ptr + pagesize;
    if (sys_mprotect(second_page, pagesize, PROT_READ) == 0) {
        printf("Changed second page to read-only\n");
        
        // Attempt to write to it would cause SIGSEGV
        printf("Skipping write to read-only memory to avoid SIGSEGV\n");
    }
    
    // Check if pages are in memory
    unsigned char *vec = malloc(4);
    if (sys_mincore(ptr, region_size, vec) == 0) {
        printf("Memory residency status:\n");
        for (size_t i = 0; i < 4; i++) {
            printf("  Page %zu: %s\n", i, (vec[i] & 1) ? "in memory" : "not in memory");
        }
    }
    free(vec);
    
    // Lock the third page in memory
    void *third_page = (char*)ptr + pagesize * 2;
    if (sys_mlock(third_page, pagesize) == 0) {
        printf("Locked third page in memory\n");
        
        // Later, unlock it
        if (sys_munlock(third_page, pagesize) == 0) {
            printf("Unlocked third page\n");
        }
    }
    
    // Sync the memory to disk (would matter for shared mappings)
    if (sys_msync(ptr, region_size, MS_SYNC) == 0) {
        printf("Memory region synced with disk\n");
    }
    
    // Free the memory
    if (sys_munmap(ptr, region_size) == 0) {
        printf("Memory region freed\n");
    }
}

// Demo for socket options and advanced networking
void demo_advanced_networking() {
    print_message("\n--- Advanced Networking Demo ---\n");
    
    // Create a socket pair (two connected sockets)
    int sv[2];
    if (sys_socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0) {
        printf("Created socket pair: [%d, %d]\n", sv[0], sv[1]);
        
        // Set socket option on the first socket
        int optval = 1;
        if (sys_setsockopt(sv[0], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == 0) {
            printf("Set SO_REUSEADDR option on socket %d\n", sv[0]);
            
            // Get the socket option back to verify
            int getoptval;
            socklen_t optlen = sizeof(getoptval);
            if (sys_getsockopt(sv[0], SOL_SOCKET, SO_REUSEADDR, &getoptval, &optlen) == 0) {
                printf("Retrieved SO_REUSEADDR option value: %d\n", getoptval);
            }
        }
        
        // Send a message from sv[0] to sv[1]
        const char *msg = "Hello from socketpair!";
        if (sys_sendto(sv[0], msg, strlen(msg), 0, NULL, 0) > 0) {
            printf("Sent message: '%s'\n", msg);
            
            // Receive the message on sv[1]
            char buf[128];
            ssize_t recv_len = sys_recvfrom(sv[1], buf, sizeof(buf) - 1, 0, NULL, NULL);
            if (recv_len > 0) {
                buf[recv_len] = '\0';
                printf("Received message: '%s'\n", buf);
            }
        }
        
        // Get peer name (should be same as local name for socketpair)
        struct sockaddr_un addr;
        socklen_t addrlen = sizeof(addr);
        if (sys_getpeername(sv[0], (struct sockaddr*)&addr, &addrlen) == 0) {
            printf("Got peer name for socket %d\n", sv[0]);
        }
        
        // Shutdown the write side of the first socket
        if (sys_shutdown(sv[0], SHUT_WR) == 0) {
            printf("Shut down write side of socket %d\n", sv[0]);
        }
        
        // Close both sockets
        sys_close(sv[0]);
        sys_close(sv[1]);
        printf("Closed both sockets\n");
    }
}

// Demo for system timers
void demo_system_timers() {
    print_message("\n--- System Timers Demo ---\n");
    
    // Set up a timer that will deliver SIGALRM
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    
    // Set up the signal handler first
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        printf("Failed to set up signal handler\n");
        return;
    }
    
    // Create the timer
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    
    if (sys_timer_create(CLOCK_REALTIME, &sev, &timerid) != 0) {
        printf("Failed to create timer\n");
        return;
    }
    
    printf("Created timer\n");
    
    // Start the timer: fire in 2 seconds, then every 3 seconds
    its.it_value.tv_sec = 2;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 3;
    its.it_interval.tv_nsec = 0;
    
    if (sys_timer_settime(timerid, 0, &its, NULL) == 0) {
        printf("Started timer: first in 2s, then every 3s\n");
        
        // Get current timer value
        struct itimerspec curr;
        if (sys_timer_gettime(timerid, &curr) == 0) {
            printf("Timer has %lds %ldns until next expiration\n",
                  (long)curr.it_value.tv_sec, curr.it_value.tv_nsec);
        }
        
        // Wait for timer to fire
        printf("Waiting for timer to fire...\n");
        sleep(3); // Should see the signal fire during this time
        
        // Check for overruns
        int overruns = sys_timer_getoverrun(timerid);
        printf("Timer overrun count: %d\n", overruns);
        
        // Disable the timer
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        
        if (sys_timer_settime(timerid, 0, &its, NULL) == 0) {
            printf("Disabled timer\n");
        }
        
        // Delete the timer
        if (sys_timer_delete(timerid) == 0) {
            printf("Deleted timer\n");
        }
    }
}

// Demo for process priority and resource usage
void demo_process_priority() {
    print_message("\n--- Process Priority and Resource Usage Demo ---\n");
    
    // Get current process priority
    int prio = sys_getpriority(PRIO_PROCESS, 0);
    printf("Current process priority: %d\n", prio);
    
    // Try to increase the priority slightly (lower number = higher priority)
    int new_prio = prio - 1;
    if (sys_setpriority(PRIO_PROCESS, 0, new_prio) == 0) {
        printf("Changed process priority to %d\n", new_prio);
        
        // Check the new priority
        prio = sys_getpriority(PRIO_PROCESS, 0);
        printf("New process priority: %d\n", prio);
    } else {
        printf("Could not change priority (may need higher privileges)\n");
    }
    
    // Alternatively, use nice()
    int nice_val = sys_nice(1);
    if (nice_val != -1 || errno == 0) {
        printf("Increased nice value by 1 to %d\n", nice_val);
    }
    
    // Get resource usage statistics
    struct rusage usage;
    if (sys_getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Resource usage statistics:\n");
        printf("  User CPU time used: %ld.%06ld sec\n",
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("  System CPU time used: %ld.%06ld sec\n",
               usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        printf("  Maximum resident set size: %ld KB\n", usage.ru_maxrss);
        printf("  Page faults: %ld\n", usage.ru_majflt);
        printf("  Voluntary context switches: %ld\n", usage.ru_nvcsw);
        printf("  Involuntary context switches: %ld\n", usage.ru_nivcsw);
    }
    
    // Set up an interval timer (ITIMER_REAL)
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    
    // Set up a signal handler for SIGALRM
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGALRM, &sa, NULL) == 0) {
        printf("Set up signal handler for SIGALRM\n");
        
        if (sys_setitimer(ITIMER_REAL, &timer, NULL) == 0) {
            printf("Set up interval timer for 1 second\n");
            printf("Waiting for timer...\n");
            sleep(2); // Should get interrupted by the timer
            
            // Get current timer value
            struct itimerval curr_timer;
            if (sys_getitimer(ITIMER_REAL, &curr_timer) == 0) {
                printf("Remaining timer time: %ld.%06ld sec\n",
                       curr_timer.it_value.tv_sec, curr_timer.it_value.tv_usec);
            }
        }
    }
}

// Demo for user and group ID operations
void demo_user_group_ids() {
    print_message("\n--- User and Group IDs Demo ---\n");
    
    // Get real and effective user IDs
    uid_t uid = sys_getuid();
    uid_t euid = sys_geteuid();
    
    printf("Real user ID: %u\n", uid);
    printf("Effective user ID: %u\n", euid);
    
    // Get real and effective group IDs
    gid_t gid = sys_getgid();
    gid_t egid = sys_getegid();
    
    printf("Real group ID: %u\n", gid);
    printf("Effective group ID: %u\n", egid);
    
    // Get real, effective, and saved user IDs
    uid_t ruid, euid2, suid;
    if (sys_getresuid(&ruid, &euid2, &suid) == 0) {
        printf("Real user ID: %u\n", ruid);
        printf("Effective user ID: %u\n", euid2);
        printf("Saved user ID: %u\n", suid);
    }
    
    // Get real, effective, and saved group IDs
    gid_t rgid, egid2, sgid;
    if (sys_getresgid(&rgid, &egid2, &sgid) == 0) {
        printf("Real group ID: %u\n", rgid);
        printf("Effective group ID: %u\n", egid2);
        printf("Saved group ID: %u\n", sgid);
    }
    
    // Get supplementary group IDs
    int ngroups = sys_getgroups(0, NULL);
    if (ngroups > 0) {
        gid_t *groups = malloc(ngroups * sizeof(gid_t));
        if (groups && sys_getgroups(ngroups, groups) > 0) {
            printf("Supplementary groups (%d):", ngroups);
            for (int i = 0; i < ngroups; i++) {
                printf(" %u", groups[i]);
            }
            printf("\n");
        }
        free(groups);
    }
    
    // Note: The following operations usually require root privileges
    // Attempting to change user/group IDs for demonstration only
    
    printf("\nNote: The following operations typically require root privileges.\n");
    printf("If running as non-root, these operations will fail gracefully.\n\n");
    
    // Try to temporarily change effective UID and restore
    if (sys_seteuid(uid) == 0) {
        printf("Changed effective user ID to %u\n", uid);
        
        // Restore
        if (sys_seteuid(euid) == 0) {
            printf("Restored effective user ID to %u\n", euid);
        }
    }
    
    // Try to temporarily change effective GID and restore
    if (sys_setegid(gid) == 0) {
        printf("Changed effective group ID to %u\n", gid);
        
        // Restore
        if (sys_setegid(egid) == 0) {
            printf("Restored effective group ID to %u\n", egid);
        }
    }
}

// Demo for file locking
void demo_file_locking() {
    print_message("\n--- File Locking Demo ---\n");
    
    // Create a file to lock
    int fd = sys_open("lock_test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        return;
    }
    
    // Write some data
    const char *data = "This is a test file for file locking.";
    sys_write(fd, data, strlen(data));
    
    printf("File created for locking demonstration\n");
    
    // Try to get an exclusive lock on the file
    if (sys_flock(fd, LOCK_EX | LOCK_NB) == 0) {
        printf("Acquired exclusive lock on file\n");
        
        // Spawn a child process that will try to lock the same file
        pid_t pid = sys_fork();
        
        if (pid == 0) {
            // Child process
            printf("[Child] Attempting to lock the file...\n");
            
            // Try to get a shared lock (should fail because parent has exclusive lock)
            if (sys_flock(fd, LOCK_SH | LOCK_NB) == -1) {
                printf("[Child] Failed to acquire lock (as expected)\n");
                
                // Wait a bit for parent to release the lock
                printf("[Child] Waiting for parent to release lock...\n");
                sleep(2);
                
                // Try again, this time blocking until lock is available
                if (sys_flock(fd, LOCK_SH) == 0) {
                    printf("[Child] Acquired shared lock after parent released it\n");
                    
                    // Release lock
                    if (sys_flock(fd, LOCK_UN) == 0) {
                        printf("[Child] Released lock\n");
                    }
                }
            }
            
            sys_exit(0);
        } else if (pid > 0) {
            // Parent process
            // Hold the lock for a second
            printf("[Parent] Holding exclusive lock for 1 second...\n");
            sleep(1);
            
            // Release the lock
            if (sys_flock(fd, LOCK_UN) == 0) {
                printf("[Parent] Released exclusive lock\n");
            }
            
            // Wait for child to finish
            int status;
            sys_wait(&status);
        }
    }
    
    // Clean up
    sys_close(fd);
    unlink("lock_test.txt");
    printf("Lock test file removed\n");
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

// Demo for system information operations
void demo_system_info_operations() {
    print_message("\n--- System Information Demo ---\n");
    
    // Get system information
    struct sysinfo si;
    if (sys_sysinfo(&si) == 0) {
        printf("System uptime: %ld seconds\n", si.uptime);
        printf("Total RAM: %lu bytes\n", si.totalram);
        printf("Free RAM: %lu bytes\n", si.freeram);
        printf("Number of processes: %d\n", si.procs);
    }
    
    // Get system name and version
    struct utsname un;
    if (sys_uname(&un) == 0) {
        printf("System name: %s\n", un.sysname);
        printf("Node name: %s\n", un.nodename);
        printf("Release: %s\n", un.release);
        printf("Version: %s\n", un.version);
        printf("Machine: %s\n", un.machine);
    }
    
    // Get system configuration values
    printf("Maximum path length: %ld\n", sys_sysconf(_PC_PATH_MAX));
    printf("Maximum number of open files: %ld\n", sys_sysconf(_SC_OPEN_MAX));
    printf("Number of processors configured: %ld\n", sys_sysconf(_SC_NPROCESSORS_CONF));
}

// Demo for user and group operations
void demo_user_group_operations() {
    print_message("\n--- User and Group Management Demo ---\n");
    
    // Get current user and group IDs
    uid_t uid = sys_getuid();
    gid_t gid = sys_getgid();
    printf("Current user ID: %d\n", uid);
    printf("Current group ID: %d\n", gid);
    
    // Get user information by name
    const char *username = "root";
    struct passwd *pw = sys_getpwnam(username);
    if (pw != NULL) {
        printf("User '%s' information:\n", username);
        printf("  UID: %d\n", pw->pw_uid);
        printf("  GID: %d\n", pw->pw_gid);
        printf("  Home: %s\n", pw->pw_dir);
        printf("  Shell: %s\n", pw->pw_shell);
    }
    
    // Get group information by name
    const char *groupname = "wheel";
    struct group *gr = sys_getgrnam(groupname);
    if (gr != NULL) {
        printf("Group '%s' information:\n", groupname);
        printf("  GID: %d\n", gr->gr_gid);
        printf("  Members: ");
        char **members = gr->gr_mem;
        while (*members) {
            printf("%s ", *members);
            members++;
        }
        printf("\n");
    }
}

// Demo for filesystem operations
void demo_filesystem_operations() {
    print_message("\n--- Filesystem Operations Demo ---\n");
    
    // Get filesystem statistics
    struct statfs fs_stats;
    if (sys_statfs("/", &fs_stats) == 0) {
        printf("Filesystem statistics for root (/):\n");
        printf("  Total blocks: %ld\n", fs_stats.f_blocks);
        printf("  Free blocks: %ld\n", fs_stats.f_bfree);
        printf("  Block size: %ld bytes\n", fs_stats.f_bsize);
    }
    
    // Create a symbolic link
    const char *target = "testfile.txt";
    const char *linkpath = "testlink";
    if (sys_symlink(target, linkpath) == 0) {
        printf("Created symbolic link '%s' -> '%s'\n", linkpath, target);
        
        // Read the symbolic link
        char buf[256];
        ssize_t len = sys_readlink(linkpath, buf, sizeof(buf)-1);
        if (len != -1) {
            buf[len] = '\0';
            printf("Symbolic link points to: %s\n", buf);
        }
    }
    
    // Sync filesystem
    sys_sync();
    printf("Filesystem synced\n");
}

// Demo for terminal I/O operations
void demo_terminal_operations() {
    print_message("\n--- Terminal I/O Operations Demo ---\n");
    
    // Check if stdout is a terminal
    if (sys_isatty(STDOUT_FILENO)) {
        printf("STDOUT is a terminal\n");
        
        // Get terminal attributes
        struct termios orig_termios, new_termios;
        if (sys_tcgetattr(STDOUT_FILENO, &orig_termios) == 0) {
            printf("Retrieved terminal attributes\n");
            
            // Make a copy to modify
            new_termios = orig_termios;
            
            // Disable echo
            new_termios.c_lflag &= ~ECHO;
            
            // Set the new attributes
            if (sys_tcsetattr(STDOUT_FILENO, TCSANOW, &new_termios) == 0) {
                printf("Echo disabled. Type something (not displayed): ");
                char input[256];
                // Fix: Check return value of fgets
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    printf("\nYou typed: %s", input);
                }
                
                // Restore original settings
                sys_tcsetattr(STDOUT_FILENO, TCSANOW, &orig_termios);
                printf("Terminal settings restored\n");
            }
            
            // Get window size using ioctl
            struct winsize ws;
            if (sys_ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
                printf("Terminal size: %d rows x %d columns\n", ws.ws_row, ws.ws_col);
            }
        }
    } else {
        printf("STDOUT is not a terminal\n");
    }
}

// Demo for message queue operations
void demo_message_queue_operations() {
    print_message("\n--- Message Queue Operations Demo ---\n");
    
    // Setup message queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;
    
    // Open (create) a message queue
    const char *queue_name = "/test_queue";
    mqd_t mqd = sys_mq_open(queue_name, O_CREAT | O_RDWR, 0644, &attr);
    
    if (mqd != (mqd_t)-1) {
        printf("Message queue created: %s\n", queue_name);
        
        // Fork a process to receive messages
        pid_t pid = sys_fork();
        
        if (pid == 0) {
            // Child process - receive messages
            char buffer[256];
            unsigned int prio;
            
            printf("[Child] Waiting for message...\n");
            ssize_t bytes = sys_mq_receive(mqd, buffer, sizeof(buffer), &prio);
            
            if (bytes >= 0) {
                buffer[bytes] = '\0';
                printf("[Child] Received message: '%s' with priority %u\n", buffer, prio);
            }
            
            sys_mq_close(mqd);
            sys_exit(0);
            
        } else if (pid > 0) {
            // Parent process - send a message
            const char *message = "Hello via message queue!";
            
            // Sleep briefly to ensure child is ready
            sleep(1);
            
            printf("[Parent] Sending message...\n");
            if (sys_mq_send(mqd, message, strlen(message), 1) == 0) {
                printf("[Parent] Message sent successfully\n");
            }
            
            // Wait for child to finish
            int status;
            sys_wait(&status);
            
            // Clean up
            sys_mq_close(mqd);
            sys_mq_unlink(queue_name);
            printf("Message queue removed\n");
        }
    }
}

// Demo for event polling operations
void demo_event_monitoring() {
    print_message("\n--- Event Monitoring Demo ---\n");
    
    // Create a pipe for demonstration
    int pipefd[2];
    if (sys_pipe(pipefd) == 0) {
        printf("Created pipe for event demo\n");
        
        // Set up poll structure
        struct pollfd fds[1];
        fds[0].fd = pipefd[0];
        fds[0].events = POLLIN;
        
        // Fork child to write to the pipe
        pid_t pid = sys_fork();
        
        if (pid == 0) {
            // Child process - write to pipe after a delay
            sys_close(pipefd[0]);  // Close read end
            
            printf("[Child] Sleeping for 2 seconds before sending data...\n");
            sleep(2);
            
            const char *message = "Poll detected me!";
            sys_write(pipefd[1], message, strlen(message));
            
            sys_close(pipefd[1]);
            sys_exit(0);
            
        } else if (pid > 0) {
            // Parent process - poll for data
            sys_close(pipefd[1]);  // Close write end
            
            printf("[Parent] Polling for data (timeout: 5 seconds)...\n");
            int poll_result = sys_poll(fds, 1, 5000);  // 5 second timeout
            
            if (poll_result > 0) {
                // Data is available
                if (fds[0].revents & POLLIN) {
                    char buffer[128];
                    ssize_t bytes = sys_read(pipefd[0], buffer, sizeof(buffer) - 1);
                    
                    if (bytes > 0) {
                        buffer[bytes] = '\0';
                        printf("[Parent] Poll detected data: %s\n", buffer);
                    }
                }
            } else if (poll_result == 0) {
                printf("[Parent] Poll timed out, no data received\n");
            }
            
            // Clean up
            sys_close(pipefd[0]);
            
            // Wait for child
            int status;
            sys_wait(&status);
        }
    }
}

// Demo for file monitoring with inotify
void demo_file_monitoring() {
    print_message("\n--- File Monitoring Demo ---\n");
    
    // Initialize inotify
    int fd = sys_inotify_init();
    if (fd != -1) {
        printf("Initialized inotify\n");
        
        // Create a temporary file to monitor
        int filefd = sys_open("inotify_test.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (filefd != -1) {
            sys_close(filefd);
            printf("Created file to monitor: inotify_test.txt\n");
            
            // Add watch for the file
            int wd = sys_inotify_add_watch(fd, "inotify_test.txt", 
                                          IN_MODIFY | IN_ACCESS | IN_DELETE_SELF);
            
            if (wd != -1) {
                printf("Added watch for file modifications\n");
                
                // Fork a child to modify the file
                pid_t pid = sys_fork();
                
                if (pid == 0) {
                    // Child process - wait then modify file
                    sleep(1);
                    
                    printf("[Child] Modifying the file...\n");
                    filefd = sys_open("inotify_test.txt", O_WRONLY | O_APPEND, 0);
                    if (filefd != -1) {
                        const char *message = "Modified content";
                        sys_write(filefd, message, strlen(message));
                        sys_close(filefd);
                    }
                    
                    sleep(1);
                    printf("[Child] Deleting the file...\n");
                    unlink("inotify_test.txt");
                    
                    sys_exit(0);
                    
                } else if (pid > 0) {
                    // Parent process - monitor file events
                    printf("[Parent] Monitoring file events (timeout: 5 seconds)...\n");
                    
                    // Set up for select with timeout
                    fd_set readfds;
                    struct timeval timeout;
                    
                    FD_ZERO(&readfds);
                    FD_SET(fd, &readfds);
                    timeout.tv_sec = 5;
                    timeout.tv_usec = 0;
                    
                    while (1) {
                        int select_result = sys_select(fd + 1, &readfds, NULL, NULL, &timeout);
                        
                        if (select_result == -1) {
                            break;
                        } else if (select_result == 0) {
                            printf("[Parent] Timeout reached, no more events\n");
                            break;
                        }
                        
                        if (FD_ISSET(fd, &readfds)) {
                            // Buffer for inotify events
                            char buffer[4096];
                            ssize_t len = sys_read(fd, buffer, sizeof(buffer));
                            
                            if (len > 0) {
                                // Process events
                                int i = 0;
                                while (i < len) {
                                    struct inotify_event *event = (struct inotify_event *)&buffer[i];
                                    
                                    printf("[Parent] Event detected: ");
                                    
                                    if (event->mask & IN_MODIFY)
                                        printf("File was modified\n");
                                    if (event->mask & IN_ACCESS)
                                        printf("File was accessed\n");
                                    if (event->mask & IN_DELETE_SELF)
                                        printf("File was deleted\n");
                                    
                                    i += sizeof(struct inotify_event) + event->len;
                                }
                            }
                        }
                        
                        // Reset for next select
                        FD_ZERO(&readfds);
                        FD_SET(fd, &readfds);
                        timeout.tv_sec = 5;
                        timeout.tv_usec = 0;
                    }
                    
                    // Clean up
                    sys_inotify_rm_watch(fd, wd);
                    sys_close(fd);
                    
                    // Wait for child
                    int status;
                    sys_wait(&status);
                }
            }
        }
    }
}

// Demo for process scheduling
void demo_process_scheduling() {
    print_message("\n--- Process Scheduling Demo ---\n");
    
    // Get scheduling policy and priority for current process
    int policy = sys_sched_getscheduler(0);  // 0 means current process
    
    if (policy != -1) {
        // Print current scheduling policy
        printf("Current scheduling policy: ");
        switch (policy) {
            case SCHED_OTHER: printf("SCHED_OTHER (normal)\n"); break;
            case SCHED_FIFO: printf("SCHED_FIFO (real-time)\n"); break;
            case SCHED_RR: printf("SCHED_RR (real-time round-robin)\n"); break;
            default: printf("Unknown (%d)\n", policy);
        }
        
        // Get current priority
        struct sched_param param;
        if (sys_sched_getparam(0, &param) == 0) {
            printf("Current priority: %d\n", param.sched_priority);
            
            // Show priority ranges
            printf("Priority ranges:\n");
            printf("SCHED_OTHER: min=%d, max=%d\n",
                   sys_sched_get_priority_min(SCHED_OTHER),
                   sys_sched_get_priority_max(SCHED_OTHER));
            printf("SCHED_FIFO: min=%d, max=%d\n",
                   sys_sched_get_priority_min(SCHED_FIFO),
                   sys_sched_get_priority_max(SCHED_FIFO));
            printf("SCHED_RR: min=%d, max=%d\n",
                   sys_sched_get_priority_min(SCHED_RR),
                   sys_sched_get_priority_max(SCHED_RR));
        }
        
        // Demonstrate sched_yield
        printf("Calling sched_yield() to voluntarily yield CPU...\n");
        sys_sched_yield();
        printf("Returned from sched_yield()\n");
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
    print_message("10. System Information Demo\n");
    print_message("11. User and Group Management Demo\n");
    print_message("12. Filesystem Operations Demo\n");
    print_message("13. Terminal I/O Demo\n");
    print_message("14. Message Queue Demo\n");
    print_message("15. Event Monitoring Demo\n");
    print_message("16. File Monitoring Demo\n");
    print_message("17. Process Scheduling Demo\n");
    print_message("18. Advanced File Operations Demo\n");
    print_message("19. Extended Attributes Demo\n");
    print_message("20. Advanced Memory Management Demo\n");
    print_message("21. Advanced Networking Demo\n");
    print_message("22. System Timers Demo\n");
    print_message("23. Process Priority and Resource Usage Demo\n");
    print_message("24. User and Group IDs Demo\n");
    print_message("25. File Locking Demo\n");
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
            case 10:
                demo_system_info_operations();
                break;
            case 11:
                demo_user_group_operations();
                break;
            case 12:
                demo_filesystem_operations();
                break;
            case 13:
                demo_terminal_operations();
                break;
            case 14:
                demo_message_queue_operations();
                break;
            case 15:
                demo_event_monitoring();
                break;
            case 16:
                demo_file_monitoring();
                break;
            case 17:
                demo_process_scheduling();
                break;
            case 18:
                demo_advanced_file_operations();
                break;
            case 19:
                demo_extended_attributes();
                break;
            case 20:
                demo_advanced_memory_management();
                break;
            case 21:
                demo_advanced_networking();
                break;
            case 22:
                demo_system_timers();
                break;
            case 23:
                demo_process_priority();
                break;
            case 24:
                demo_user_group_ids();
                break;
            case 25:
                demo_file_locking();
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
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
        }
    }
    
    return 0;
}