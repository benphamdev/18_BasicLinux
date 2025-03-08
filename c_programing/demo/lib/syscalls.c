#include "syscalls.h"
#include <string.h>  // Add this for strlen()

void print_message(const char *message) {
    if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

char *read_line(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        perror("getline");
        exit(EXIT_FAILURE);
    }

    return line;
}

// File Management
int sys_open(const char *pathname, int flags, mode_t mode) {
    int fd = open(pathname, flags, mode);
    if (fd == -1) {
        perror("sys_open");
    }
    return fd;
}

ssize_t sys_read(int fd, void *buf, size_t count) {
    ssize_t bytes = read(fd, buf, count);
    if (bytes == -1) {
        perror("sys_read");
    }
    return bytes;
}

ssize_t sys_write(int fd, const void *buf, size_t count) {
    ssize_t bytes = write(fd, buf, count);
    if (bytes == -1) {
        perror("sys_write");
    }
    return bytes;
}

int sys_close(int fd) {
    int result = close(fd);
    if (result == -1) {
        perror("sys_close");
    }
    return result;
}

off_t sys_lseek(int fd, off_t offset, int whence) {
    off_t pos = lseek(fd, offset, whence);
    if (pos == (off_t)-1) {
        perror("sys_lseek");
    }
    return pos;
}

int sys_stat(const char *pathname, struct stat *statbuf) {
    int result = stat(pathname, statbuf);
    if (result == -1) {
        perror("sys_stat");
    }
    return result;
}

// Process Management
pid_t sys_fork(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("sys_fork");
    }
    return pid;
}

int sys_execve(const char *pathname, char *const argv[], char *const envp[]) {
    int result = execve(pathname, argv, envp);
    // Only returns if there is an error
    if (result == -1) {
        perror("sys_execve");
    }
    return result;
}

pid_t sys_wait(int *status) {
    pid_t pid = wait(status);
    if (pid == -1) {
        perror("sys_wait");
    }
    return pid;
}

void sys_exit(int status) {
    exit(status);
}

pid_t sys_getpid(void) {
    return getpid();
}

// Signal Handling
int sys_kill(pid_t pid, int sig) {
    int result = kill(pid, sig);
    if (result == -1) {
        perror("sys_kill");
    }
    return result;
}

int sys_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
    int result = sigaction(signum, act, oldact);
    if (result == -1) {
        perror("sys_sigaction");
    }
    return result;
}

int sys_pause(void) {
    int result = pause();
    if (result == -1 && errno != EINTR) {
        perror("sys_pause");
    }
    return result;
}

// Memory Management
void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void *ptr = mmap(addr, length, prot, flags, fd, offset);
    if (ptr == MAP_FAILED) {
        perror("sys_mmap");
    }
    return ptr;
}

int sys_munmap(void *addr, size_t length) {
    int result = munmap(addr, length);
    if (result == -1) {
        perror("sys_munmap");
    }
    return result;
}

int sys_brk(void *addr) {
    void *result = sbrk(0); // Get current program break
    if (addr == NULL)
        return (intptr_t)result; // Use intptr_t for pointer-to-integer conversion
    
    if (brk(addr) == -1) {
        perror("sys_brk");
        return -1;
    }
    return 0;
}

// Directory Management
int sys_mkdir(const char *pathname, mode_t mode) {
    int result = mkdir(pathname, mode);
    if (result == -1) {
        perror("sys_mkdir");
    }
    return result;
}

int sys_rmdir(const char *pathname) {
    int result = rmdir(pathname);
    if (result == -1) {
        perror("sys_rmdir");
    }
    return result;
}

int sys_chdir(const char *path) {
    int result = chdir(path);
    if (result == -1) {
        perror("sys_chdir");
    }
    return result;
}

char *sys_getcwd(char *buf, size_t size) {
    char *result = getcwd(buf, size);
    if (result == NULL) {
        perror("sys_getcwd");
    }
    return result;
}

// Networking
int sys_socket(int domain, int type, int protocol) {
    int sockfd = socket(domain, type, protocol);
    if (sockfd == -1) {
        perror("sys_socket");
    }
    return sockfd;
}

int sys_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int result = bind(sockfd, addr, addrlen);
    if (result == -1) {
        perror("sys_bind");
    }
    return result;
}

int sys_listen(int sockfd, int backlog) {
    int result = listen(sockfd, backlog);
    if (result == -1) {
        perror("sys_listen");
    }
    return result;
}

int sys_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int result = accept(sockfd, addr, addrlen);
    if (result == -1) {
        perror("sys_accept");
    }
    return result;
}

int sys_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int result = connect(sockfd, addr, addrlen);
    if (result == -1) {
        perror("sys_connect");
    }
    return result;
}

ssize_t sys_send(int sockfd, const void *buf, size_t len, int flags) {
    ssize_t result = send(sockfd, buf, len, flags);
    if (result == -1) {
        perror("sys_send");
    }
    return result;
}

ssize_t sys_recv(int sockfd, void *buf, size_t len, int flags) {
    ssize_t result = recv(sockfd, buf, len, flags);
    if (result == -1) {
        perror("sys_recv");
    }
    return result;
}

// Thread Management - Using clone for low-level thread creation
int sys_clone(int (*fn)(void *), void *stack, int flags, void *arg) {
    // Silence unused parameter warnings
    (void)fn;
    (void)stack;
    (void)flags;
    (void)arg;
    
    // This is a simplified wrapper - the actual clone syscall is more complex
    errno = ENOSYS; // Not implemented
    return -1;
}

int sys_futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3) {
    // Silence unused parameter warnings
    (void)uaddr;
    (void)futex_op;
    (void)val;
    (void)timeout;
    (void)uaddr2;
    (void)val3;
    
    // This is a simplified wrapper - actual implementation depends on the platform
    errno = ENOSYS; // Not implemented
    return -1;
}

// Time Management
int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
    int result = gettimeofday(tv, tz);
    if (result == -1) {
        perror("sys_gettimeofday");
    }
    return result;
}

int sys_nanosleep(const struct timespec *req, struct timespec *rem) {
    int result = nanosleep(req, rem);
    if (result == -1) {
        perror("sys_nanosleep");
    }
    return result;
}

// Resource Management
int sys_getrlimit(int resource, struct rlimit *rlim) {
    int result = getrlimit(resource, rlim);
    if (result == -1) {
        perror("sys_getrlimit");
    }
    return result;
}

int sys_setrlimit(int resource, const struct rlimit *rlim) {
    int result = setrlimit(resource, rlim);
    if (result == -1) {
        perror("sys_setrlimit");
    }
    return result;
}

// File Permission Handling
int sys_chmod(const char *pathname, mode_t mode) {
    int result = chmod(pathname, mode);
    if (result == -1) {
        perror("sys_chmod");
    }
    return result;
}

int sys_chown(const char *pathname, uid_t owner, gid_t group) {
    int result = chown(pathname, owner, group);
    if (result == -1) {
        perror("sys_chown");
    }
    return result;
}

int sys_access(const char *pathname, int mode) {
    int result = access(pathname, mode);
    if (result == -1) {
        perror("sys_access");
    }
    return result;
}

mode_t sys_umask(mode_t mask) {
    return umask(mask);
}

// Asynchronous I/O
int sys_aio_read(struct aiocb *aiocbp) {
    int result = aio_read(aiocbp);
    if (result == -1) {
        perror("sys_aio_read");
    }
    return result;
}

int sys_aio_write(struct aiocb *aiocbp) {
    int result = aio_write(aiocbp);
    if (result == -1) {
        perror("sys_aio_write");
    }
    return result;
}

int sys_aio_error(const struct aiocb *aiocbp) {
    return aio_error(aiocbp);
}

ssize_t sys_aio_return(struct aiocb *aiocbp) {
    return aio_return(aiocbp);
}

int sys_aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout) {
    int result = aio_suspend(list, nent, timeout);
    if (result == -1) {
        perror("sys_aio_suspend");
    }
    return result;
}

int sys_aio_cancel(int fd, struct aiocb *aiocbp) {
    int result = aio_cancel(fd, aiocbp);
    if (result == -1) {
        perror("sys_aio_cancel");
    }
    return result;
}

// Inter-Process Communication (IPC)
int sys_pipe(int pipefd[2]) {
    int result = pipe(pipefd);
    if (result == -1) {
        perror("sys_pipe");
    }
    return result;
}

int sys_shmget(key_t key, size_t size, int shmflg) {
    int result = shmget(key, size, shmflg);
    if (result == -1) {
        perror("sys_shmget");
    }
    return result;
}

void *sys_shmat(int shmid, const void *shmaddr, int shmflg) {
    void *result = shmat(shmid, shmaddr, shmflg);
    if (result == (void *)-1) {
        perror("sys_shmat");
    }
    return result;
}

int sys_shmdt(const void *shmaddr) {
    int result = shmdt(shmaddr);
    if (result == -1) {
        perror("sys_shmdt");
    }
    return result;
}