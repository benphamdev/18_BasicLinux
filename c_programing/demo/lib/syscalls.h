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
#include <pwd.h>      // For passwd struct
#include <grp.h>      // For group struct
#include <sys/sysinfo.h>  // For sysinfo struct
#include <sys/utsname.h>  // For utsname struct
#include <sys/mount.h>    // For mount
#include <sys/statfs.h>   // For statfs struct
#include <sys/vfs.h>      // For statfs
#include <linux/limits.h> // For PATH_MAX
#include <termios.h>   // For terminal I/O
#include <sys/ioctl.h> // For ioctl operations
#include <mqueue.h>    // For POSIX message queues
#include <semaphore.h> // For POSIX semaphores
#include <poll.h>      // For poll/select operations
#include <sys/epoll.h> // For epoll
#include <sys/inotify.h> // For inotify
#include <sched.h>     // For scheduling operations

// Add these include directives
#include <sys/quota.h>    // For quotactl
#include <sys/time.h>     // For gettimeofday, getitimer
#include <sys/resource.h> // For getrusage
#include <sys/syscall.h>  // For direct syscall access
#include <signal.h>       // For sigaction
#include <sys/file.h>     // For flock
#include <sys/socket.h>   // For socket operations
#include <time.h>         // For timer_t, clockid_t
#include <sys/un.h>       // For AF_UNIX sockets
#include <sys/xattr.h>    // For extended attributes
#include <sys/sendfile.h> // For sendfile
#include <linux/falloc.h> // For fallocate flags
#include <sys/swap.h>     // For swapon/swapoff

// Optional capability support - only enabled if available
#if defined(__linux__) && defined(HAVE_SYS_CAPABILITY_H)
#include <sys/capability.h> // For capabilities
#endif

#include <sys/prctl.h> // For process control

// Include additional headers for specific system calls
#ifdef __linux__
// For fallocate
// Remove the _GNU_SOURCE define since it's already in the Makefile
#include <fcntl.h>

// For splice, tee, vmsplice
#include <fcntl.h>
#include <sys/uio.h>

// For copy_file_range (newer Linux kernels)
#include <unistd.h>

// For getresuid, getresgid, setresuid, setresgid
#include <unistd.h>

// For syncfs
#include <unistd.h>
#endif

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
int sys_fstat(int fd, struct stat *statbuf);

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

// System Information Operations
int sys_sysinfo(struct sysinfo *info);
int sys_uname(struct utsname *buf);
long sys_sysconf(int name);

// User & Group Management
uid_t sys_getuid(void);
gid_t sys_getgid(void);
int sys_setuid(uid_t uid);
int sys_setgid(gid_t gid);
struct passwd *sys_getpwnam(const char *name);
struct group *sys_getgrnam(const char *name);

// File System Operations
int sys_statfs(const char *path, struct statfs *buf);
int sys_sync(void);
int sys_mount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data);
int sys_umount(const char *target);
int sys_symlink(const char *target, const char *linkpath);
ssize_t sys_readlink(const char *pathname, char *buf, size_t bufsiz);

// Terminal I/O Operations
int sys_tcgetattr(int fd, struct termios *termios_p);
int sys_tcsetattr(int fd, int optional_actions, const struct termios *termios_p);
int sys_tcsendbreak(int fd, int duration);
int sys_tcflush(int fd, int queue_selector);
int sys_ioctl(int fd, unsigned long request, void *arg);
int sys_isatty(int fd);

// Message Queue Operations
mqd_t sys_mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
int sys_mq_close(mqd_t mqdes);
int sys_mq_unlink(const char *name);
int sys_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);
ssize_t sys_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);
int sys_mq_getattr(mqd_t mqdes, struct mq_attr *attr);
int sys_mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);

// Semaphore Operations
sem_t *sys_sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int sys_sem_close(sem_t *sem);
int sys_sem_unlink(const char *name);
int sys_sem_wait(sem_t *sem);
int sys_sem_trywait(sem_t *sem);
int sys_sem_post(sem_t *sem);
int sys_sem_getvalue(sem_t *sem, int *sval);

// Event Monitoring
int sys_poll(struct pollfd *fds, nfds_t nfds, int timeout);
int sys_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int sys_epoll_create(int size);
int sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int sys_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

// File Monitoring
int sys_inotify_init(void);
int sys_inotify_add_watch(int fd, const char *pathname, uint32_t mask);
int sys_inotify_rm_watch(int fd, int wd);

// Process Scheduling
int sys_sched_get_priority_max(int policy);
int sys_sched_get_priority_min(int policy);
int sys_sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
int sys_sched_getscheduler(pid_t pid);
int sys_sched_setparam(pid_t pid, const struct sched_param *param);
int sys_sched_getparam(pid_t pid, struct sched_param *param);
int sys_sched_yield(void);

// Process Control
int sys_prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);

// Capabilities - only if supported
#if defined(__linux__) && defined(HAVE_SYS_CAPABILITY_H)
int sys_capget(cap_user_header_t hdrp, cap_user_data_t datap);
int sys_capset(cap_user_header_t hdrp, const cap_user_data_t datap);
#endif

// Advanced File Operations
int sys_fallocate(int fd, int mode, off_t offset, off_t len);
int sys_ftruncate(int fd, off_t length);
int sys_fsync(int fd);
int sys_fdatasync(int fd);
int sys_posix_fadvise(int fd, off_t offset, off_t len, int advice);
int sys_madvise(void *addr, size_t length, int advice);
int sys_sync_file_range(int fd, off_t offset, off_t nbytes, unsigned int flags);
int sys_splice(int fd_in, off_t *off_in, int fd_out, off_t *off_out, size_t len, unsigned int flags);
int sys_tee(int fd_in, int fd_out, size_t len, unsigned int flags);
int sys_vmsplice(int fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags);
ssize_t sys_copy_file_range(int fd_in, off_t *off_in, int fd_out, off_t *off_out, size_t len, unsigned int flags);

// Extended Attributes
ssize_t sys_getxattr(const char *path, const char *name, void *value, size_t size);
ssize_t sys_lgetxattr(const char *path, const char *name, void *value, size_t size);
ssize_t sys_fgetxattr(int fd, const char *name, void *value, size_t size);
int sys_setxattr(const char *path, const char *name, const void *value, size_t size, int flags);
int sys_lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags);
int sys_fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);
ssize_t sys_listxattr(const char *path, char *list, size_t size);
ssize_t sys_llistxattr(const char *path, char *list, size_t size);
ssize_t sys_flistxattr(int fd, char *list, size_t size);
int sys_removexattr(const char *path, const char *name);
int sys_lremovexattr(const char *path, const char *name);
int sys_fremovexattr(int fd, const char *name);

// Advanced Memory Management
int sys_mprotect(void *addr, size_t len, int prot);
int sys_msync(void *addr, size_t length, int flags);
int sys_mincore(void *addr, size_t length, unsigned char *vec);
int sys_mlock(const void *addr, size_t len);
int sys_munlock(const void *addr, size_t len);
int sys_mlockall(int flags);
int sys_munlockall(void);

// Advanced Networking
int sys_getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int sys_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
ssize_t sys_recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sys_sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t sys_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t sys_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
int sys_socketpair(int domain, int type, int protocol, int sv[2]);
int sys_getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int sys_getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int sys_shutdown(int sockfd, int how);

// System Timers
int sys_timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
int sys_timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
int sys_timer_gettime(timer_t timerid, struct itimerspec *curr_value);
int sys_timer_delete(timer_t timerid);
int sys_timer_getoverrun(timer_t timerid);

// Process Advanced Operations
int sys_setpriority(int which, id_t who, int prio);
int sys_getpriority(int which, id_t who);
int sys_nice(int inc);
int sys_setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
int sys_getitimer(int which, struct itimerval *curr_value);
int sys_getrusage(int who, struct rusage *usage);
pid_t sys_wait3(int *status, int options, struct rusage *rusage);
pid_t sys_wait4(pid_t pid, int *status, int options, struct rusage *rusage);

// Security & Identity
uid_t sys_geteuid(void);
gid_t sys_getegid(void);
int sys_setreuid(uid_t ruid, uid_t euid);
int sys_setregid(gid_t rgid, gid_t egid);
int sys_seteuid(uid_t euid);
int sys_setegid(gid_t egid);
int sys_getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int sys_getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
int sys_setresuid(uid_t ruid, uid_t euid, uid_t suid);
int sys_setresgid(gid_t rgid, gid_t egid, gid_t sgid);
int sys_getgroups(int size, gid_t list[]);
int sys_setgroups(size_t size, const gid_t *list);

// File System Operations
int sys_quotactl(int cmd, const char *special, int id, caddr_t addr);
int sys_swapon(const char *path, int swapflags);
int sys_swapoff(const char *path);
int sys_syncfs(int fd);
int sys_flock(int fd, int operation);

#endif /* SYSCALLS_H */