#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <aio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h> // Add this for intptr_t

/**
 * Function to print a message to stdout
 * @param message The message to print
 * @return void
*/
void print_message(const char *message);

/**
 * Function to read a line of text from stdin
 * @return Pointer to the read line
*/
char *read_line(void);

// File Management
int sys_open(const char *pathname, int flags, mode_t mode);
ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
int sys_close(int fd);
off_t sys_lseek(int fd, off_t offset, int whence);
int sys_stat(const char *pathname, struct stat *statbuf);

// Process Management
pid_t sys_fork(void);
int sys_execve(const char *pathname, char *const argv[], char *const envp[]);
pid_t sys_wait(int *status);
void sys_exit(int status);
pid_t sys_getpid(void);

// Signal Handling
int sys_kill(pid_t pid, int sig);
int sys_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sys_pause(void);

// Memory Management
void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int sys_munmap(void *addr, size_t length);
int sys_brk(void *addr);

// Directory Management
int sys_mkdir(const char *pathname, mode_t mode);
int sys_rmdir(const char *pathname);
int sys_chdir(const char *path);
char *sys_getcwd(char *buf, size_t size);

// Networking
int sys_socket(int domain, int type, int protocol);
int sys_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int sys_listen(int sockfd, int backlog);
int sys_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int sys_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t sys_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sys_recv(int sockfd, void *buf, size_t len, int flags);

// Thread Management
int sys_clone(int (*fn)(void *), void *stack, int flags, void *arg);
int sys_futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3);

// Time Management
int sys_gettimeofday(struct timeval *tv, struct timezone *tz);
int sys_nanosleep(const struct timespec *req, struct timespec *rem);

// Resource Management
int sys_getrlimit(int resource, struct rlimit *rlim);
int sys_setrlimit(int resource, const struct rlimit *rlim);

// File Permission Handling
int sys_chmod(const char *pathname, mode_t mode);
int sys_chown(const char *pathname, uid_t owner, gid_t group);
int sys_access(const char *pathname, int mode);
mode_t sys_umask(mode_t mask);

// Asynchronous I/O
int sys_aio_read(struct aiocb *aiocbp);
int sys_aio_write(struct aiocb *aiocbp);
int sys_aio_error(const struct aiocb *aiocbp);
ssize_t sys_aio_return(struct aiocb *aiocbp);
int sys_aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout);
int sys_aio_cancel(int fd, struct aiocb *aiocbp);

// Inter-Process Communication (IPC)
int sys_pipe(int pipefd[2]);
int sys_shmget(key_t key, size_t size, int shmflg);
void *sys_shmat(int shmid, const void *shmaddr, int shmflg);
int sys_shmdt(const void *shmaddr);

#endif