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

int sys_fstat(int fd, struct stat *statbuf) {
    int result = fstat(fd, statbuf);
    if (result == -1) {
        perror("sys_fstat");
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

// System Information Operations
int sys_sysinfo(struct sysinfo *info) {
    int result = sysinfo(info);
    if (result == -1) {
        perror("sys_sysinfo");
    }
    return result;
}

int sys_uname(struct utsname *buf) {
    int result = uname(buf);
    if (result == -1) {
        perror("sys_uname");
    }
    return result;
}

long sys_sysconf(int name) {
    long result = sysconf(name);
    if (result == -1 && errno != 0) {
        perror("sys_sysconf");
    }
    return result;
}

// User & Group Management
uid_t sys_getuid(void) {
    return getuid();
}

gid_t sys_getgid(void) {
    return getgid();
}

int sys_setuid(uid_t uid) {
    int result = setuid(uid);
    if (result == -1) {
        perror("sys_setuid");
    }
    return result;
}

int sys_setgid(gid_t gid) {
    int result = setgid(gid);
    if (result == -1) {
        perror("sys_setgid");
    }
    return result;
}

struct passwd *sys_getpwnam(const char *name) {
    errno = 0;
    struct passwd *result = getpwnam(name);
    if (result == NULL && errno != 0) {
        perror("sys_getpwnam");
    }
    return result;
}

struct group *sys_getgrnam(const char *name) {
    errno = 0;
    struct group *result = getgrnam(name);
    if (result == NULL && errno != 0) {
        perror("sys_getgrnam");
    }
    return result;
}

// File System Operations
int sys_statfs(const char *path, struct statfs *buf) {
    int result = statfs(path, buf);
    if (result == -1) {
        perror("sys_statfs");
    }
    return result;
}

int sys_sync(void) {
    sync();
    return 0;
}

int sys_mount(const char *source, const char *target,
             const char *filesystemtype, unsigned long mountflags,
             const void *data) {
    int result = mount(source, target, filesystemtype, mountflags, data);
    if (result == -1) {
        perror("sys_mount");
    }
    return result;
}

int sys_umount(const char *target) {
    int result = umount(target);
    if (result == -1) {
        perror("sys_umount");
    }
    return result;
}

int sys_symlink(const char *target, const char *linkpath) {
    int result = symlink(target, linkpath);
    if (result == -1) {
        perror("sys_symlink");
    }
    return result;
}

ssize_t sys_readlink(const char *pathname, char *buf, size_t bufsiz) {
    ssize_t result = readlink(pathname, buf, bufsiz);
    if (result == -1) {
        perror("sys_readlink");
    }
    return result;
}

// Terminal I/O Operations
int sys_tcgetattr(int fd, struct termios *termios_p) {
    int result = tcgetattr(fd, termios_p);
    if (result == -1) {
        perror("sys_tcgetattr");
    }
    return result;
}

int sys_tcsetattr(int fd, int optional_actions, const struct termios *termios_p) {
    int result = tcsetattr(fd, optional_actions, termios_p);
    if (result == -1) {
        perror("sys_tcsetattr");
    }
    return result;
}

int sys_tcsendbreak(int fd, int duration) {
    int result = tcsendbreak(fd, duration);
    if (result == -1) {
        perror("sys_tcsendbreak");
    }
    return result;
}

int sys_tcflush(int fd, int queue_selector) {
    int result = tcflush(fd, queue_selector);
    if (result == -1) {
        perror("sys_tcflush");
    }
    return result;
}

int sys_ioctl(int fd, unsigned long request, void *arg) {
    int result = ioctl(fd, request, arg);
    if (result == -1) {
        perror("sys_ioctl");
    }
    return result;
}

int sys_isatty(int fd) {
    int result = isatty(fd);
    if (result == 0 && errno != ENOTTY) {
        perror("sys_isatty");
    }
    return result;
}

// Message Queue Operations
mqd_t sys_mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr) {
    mqd_t mqd = mq_open(name, oflag, mode, attr);
    if (mqd == (mqd_t)-1) {
        perror("sys_mq_open");
    }
    return mqd;
}

int sys_mq_close(mqd_t mqdes) {
    int result = mq_close(mqdes);
    if (result == -1) {
        perror("sys_mq_close");
    }
    return result;
}

int sys_mq_unlink(const char *name) {
    int result = mq_unlink(name);
    if (result == -1) {
        perror("sys_mq_unlink");
    }
    return result;
}

int sys_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio) {
    int result = mq_send(mqdes, msg_ptr, msg_len, msg_prio);
    if (result == -1) {
        perror("sys_mq_send");
    }
    return result;
}

ssize_t sys_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio) {
    ssize_t result = mq_receive(mqdes, msg_ptr, msg_len, msg_prio);
    if (result == -1) {
        perror("sys_mq_receive");
    }
    return result;
}

int sys_mq_getattr(mqd_t mqdes, struct mq_attr *attr) {
    int result = mq_getattr(mqdes, attr);
    if (result == -1) {
        perror("sys_mq_getattr");
    }
    return result;
}

int sys_mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr) {
    int result = mq_setattr(mqdes, newattr, oldattr);
    if (result == -1) {
        perror("sys_mq_setattr");
    }
    return result;
}

// Semaphore Operations
sem_t *sys_sem_open(const char *name, int oflag, mode_t mode, unsigned int value) {
    sem_t *result = sem_open(name, oflag, mode, value);
    if (result == SEM_FAILED) {
        perror("sys_sem_open");
    }
    return result;
}

int sys_sem_close(sem_t *sem) {
    int result = sem_close(sem);
    if (result == -1) {
        perror("sys_sem_close");
    }
    return result;
}

int sys_sem_unlink(const char *name) {
    int result = sem_unlink(name);
    if (result == -1) {
        perror("sys_sem_unlink");
    }
    return result;
}

int sys_sem_wait(sem_t *sem) {
    int result = sem_wait(sem);
    if (result == -1) {
        perror("sys_sem_wait");
    }
    return result;
}

int sys_sem_trywait(sem_t *sem) {
    int result = sem_trywait(sem);
    if (result == -1 && errno != EAGAIN) {
        perror("sys_sem_trywait");
    }
    return result;
}

int sys_sem_post(sem_t *sem) {
    int result = sem_post(sem);
    if (result == -1) {
        perror("sys_sem_post");
    }
    return result;
}

int sys_sem_getvalue(sem_t *sem, int *sval) {
    int result = sem_getvalue(sem, sval);
    if (result == -1) {
        perror("sys_sem_getvalue");
    }
    return result;
}

// Event Monitoring
int sys_poll(struct pollfd *fds, nfds_t nfds, int timeout) {
    int result = poll(fds, nfds, timeout);
    if (result == -1) {
        perror("sys_poll");
    }
    return result;
}

int sys_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    int result = select(nfds, readfds, writefds, exceptfds, timeout);
    if (result == -1) {
        perror("sys_select");
    }
    return result;
}

int sys_epoll_create(int size) {
    int result = epoll_create(size);
    if (result == -1) {
        perror("sys_epoll_create");
    }
    return result;
}

int sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    int result = epoll_ctl(epfd, op, fd, event);
    if (result == -1) {
        perror("sys_epoll_ctl");
    }
    return result;
}

int sys_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
    int result = epoll_wait(epfd, events, maxevents, timeout);
    if (result == -1) {
        perror("sys_epoll_wait");
    }
    return result;
}

// File Monitoring
int sys_inotify_init(void) {
    int result = inotify_init();
    if (result == -1) {
        perror("sys_inotify_init");
    }
    return result;
}

int sys_inotify_add_watch(int fd, const char *pathname, uint32_t mask) {
    int result = inotify_add_watch(fd, pathname, mask);
    if (result == -1) {
        perror("sys_inotify_add_watch");
    }
    return result;
}

int sys_inotify_rm_watch(int fd, int wd) {
    int result = inotify_rm_watch(fd, wd);
    if (result == -1) {
        perror("sys_inotify_rm_watch");
    }
    return result;
}

// Process Scheduling
int sys_sched_get_priority_max(int policy) {
    int result = sched_get_priority_max(policy);
    if (result == -1) {
        perror("sys_sched_get_priority_max");
    }
    return result;
}

int sys_sched_get_priority_min(int policy) {
    int result = sched_get_priority_min(policy);
    if (result == -1) {
        perror("sys_sched_get_priority_min");
    }
    return result;
}

int sys_sched_setscheduler(pid_t pid, int policy, const struct sched_param *param) {
    int result = sched_setscheduler(pid, policy, param);
    if (result == -1) {
        perror("sys_sched_setscheduler");
    }
    return result;
}

int sys_sched_getscheduler(pid_t pid) {
    int result = sched_getscheduler(pid);
    if (result == -1) {
        perror("sys_sched_getscheduler");
    }
    return result;
}

int sys_sched_setparam(pid_t pid, const struct sched_param *param) {
    int result = sched_setparam(pid, param);
    if (result == -1) {
        perror("sys_sched_setparam");
    }
    return result;
}

int sys_sched_getparam(pid_t pid, struct sched_param *param) {
    int result = sched_getparam(pid, param);
    if (result == -1) {
        perror("sys_sched_getparam");
    }
    return result;
}

int sys_sched_yield(void) {
    int result = sched_yield();
    if (result == -1) {
        perror("sys_sched_yield");
    }
    return result;
}

// Process Control
int sys_prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
    int result = prctl(option, arg2, arg3, arg4, arg5);
    if (result == -1) {
        perror("sys_prctl");
    }
    return result;
}

// Capabilities - only if supported
#if defined(__linux__) && defined(HAVE_SYS_CAPABILITY_H)
int sys_capget(cap_user_header_t hdrp, cap_user_data_t datap) {
    int result = capget(hdrp, datap);
    if (result == -1) {
        perror("sys_capget");
    }
    return result;
}

int sys_capset(cap_user_header_t hdrp, const cap_user_data_t datap) {
    int result = capset(hdrp, datap);
    if (result == -1) {
        perror("sys_capset");
    }
    return result;
}
#endif

// Advanced File Operations
int sys_fallocate(int fd, int mode, off_t offset, off_t len) {
    int result = fallocate(fd, mode, offset, len);
    if (result == -1) {
        perror("sys_fallocate");
    }
    return result;
}

int sys_ftruncate(int fd, off_t length) {
    int result = ftruncate(fd, length);
    if (result == -1) {
        perror("sys_ftruncate");
    }
    return result;
}

int sys_fsync(int fd) {
    int result = fsync(fd);
    if (result == -1) {
        perror("sys_fsync");
    }
    return result;
}

int sys_fdatasync(int fd) {
    int result = fdatasync(fd);
    if (result == -1) {
        perror("sys_fdatasync");
    }
    return result;
}

int sys_posix_fadvise(int fd, off_t offset, off_t len, int advice) {
    int result = posix_fadvise(fd, offset, len, advice);
    if (result != 0) {
        errno = result;
        perror("sys_posix_fadvise");
    }
    return result;
}

int sys_madvise(void *addr, size_t length, int advice) {
    int result = madvise(addr, length, advice);
    if (result == -1) {
        perror("sys_madvise");
    }
    return result;
}

int sys_sync_file_range(int fd, off_t offset, off_t nbytes, unsigned int flags) {
    int result = sync_file_range(fd, offset, nbytes, flags);
    if (result == -1) {
        perror("sys_sync_file_range");
    }
    return result;
}

int sys_splice(int fd_in, off_t *off_in, int fd_out, off_t *off_out, size_t len, unsigned int flags) {
    int result = splice(fd_in, off_in, fd_out, off_out, len, flags);
    if (result == -1) {
        perror("sys_splice");
    }
    return result;
}

int sys_tee(int fd_in, int fd_out, size_t len, unsigned int flags) {
    int result = tee(fd_in, fd_out, len, flags);
    if (result == -1) {
        perror("sys_tee");
    }
    return result;
}

int sys_vmsplice(int fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags) {
    int result = vmsplice(fd, iov, nr_segs, flags);
    if (result == -1) {
        perror("sys_vmsplice");
    }
    return result;
}

ssize_t sys_copy_file_range(int fd_in, off_t *off_in, int fd_out, off_t *off_out, size_t len, unsigned int flags) {
    ssize_t result = copy_file_range(fd_in, off_in, fd_out, off_out, len, flags);
    if (result == -1) {
        perror("sys_copy_file_range");
    }
    return result;
}

// Extended Attributes
ssize_t sys_getxattr(const char *path, const char *name, void *value, size_t size) {
    ssize_t result = getxattr(path, name, value, size);
    if (result == -1) {
        perror("sys_getxattr");
    }
    return result;
}

ssize_t sys_lgetxattr(const char *path, const char *name, void *value, size_t size) {
    ssize_t result = lgetxattr(path, name, value, size);
    if (result == -1) {
        perror("sys_lgetxattr");
    }
    return result;
}

ssize_t sys_fgetxattr(int fd, const char *name, void *value, size_t size) {
    ssize_t result = fgetxattr(fd, name, value, size);
    if (result == -1) {
        perror("sys_fgetxattr");
    }
    return result;
}

int sys_setxattr(const char *path, const char *name, const void *value, size_t size, int flags) {
    int result = setxattr(path, name, value, size, flags);
    if (result == -1) {
        perror("sys_setxattr");
    }
    return result;
}

int sys_lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags) {
    int result = lsetxattr(path, name, value, size, flags);
    if (result == -1) {
        perror("sys_lsetxattr");
    }
    return result;
}

int sys_fsetxattr(int fd, const char *name, const void *value, size_t size, int flags) {
    int result = fsetxattr(fd, name, value, size, flags);
    if (result == -1) {
        perror("sys_fsetxattr");
    }
    return result;
}

ssize_t sys_listxattr(const char *path, char *list, size_t size) {
    ssize_t result = listxattr(path, list, size);
    if (result == -1) {
        perror("sys_listxattr");
    }
    return result;
}

ssize_t sys_llistxattr(const char *path, char *list, size_t size) {
    ssize_t result = llistxattr(path, list, size);
    if (result == -1) {
        perror("sys_llistxattr");
    }
    return result;
}

ssize_t sys_flistxattr(int fd, char *list, size_t size) {
    ssize_t result = flistxattr(fd, list, size);
    if (result == -1) {
        perror("sys_flistxattr");
    }
    return result;
}

int sys_removexattr(const char *path, const char *name) {
    int result = removexattr(path, name);
    if (result == -1) {
        perror("sys_removexattr");
    }
    return result;
}

int sys_lremovexattr(const char *path, const char *name) {
    int result = lremovexattr(path, name);
    if (result == -1) {
        perror("sys_lremovexattr");
    }
    return result;
}

int sys_fremovexattr(int fd, const char *name) {
    int result = fremovexattr(fd, name);
    if (result == -1) {
        perror("sys_fremovexattr");
    }
    return result;
}

// Advanced Memory Management
int sys_mprotect(void *addr, size_t len, int prot) {
    int result = mprotect(addr, len, prot);
    if (result == -1) {
        perror("sys_mprotect");
    }
    return result;
}

int sys_msync(void *addr, size_t length, int flags) {
    int result = msync(addr, length, flags);
    if (result == -1) {
        perror("sys_msync");
    }
    return result;
}

int sys_mincore(void *addr, size_t length, unsigned char *vec) {
    int result = mincore(addr, length, vec);
    if (result == -1) {
        perror("sys_mincore");
    }
    return result;
}

int sys_mlock(const void *addr, size_t len) {
    int result = mlock(addr, len);
    if (result == -1) {
        perror("sys_mlock");
    }
    return result;
}

int sys_munlock(const void *addr, size_t len) {
    int result = munlock(addr, len);
    if (result == -1) {
        perror("sys_munlock");
    }
    return result;
}

int sys_mlockall(int flags) {
    int result = mlockall(flags);
    if (result == -1) {
        perror("sys_mlockall");
    }
    return result;
}

int sys_munlockall(void) {
    int result = munlockall();
    if (result == -1) {
        perror("sys_munlockall");
    }
    return result;
}

// Advanced Networking
int sys_getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) {
    int result = getsockopt(sockfd, level, optname, optval, optlen);
    if (result == -1) {
        perror("sys_getsockopt");
    }
    return result;
}

int sys_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    int result = setsockopt(sockfd, level, optname, optval, optlen);
    if (result == -1) {
        perror("sys_setsockopt");
    }
    return result;
}

ssize_t sys_recvmsg(int sockfd, struct msghdr *msg, int flags) {
    ssize_t result = recvmsg(sockfd, msg, flags);
    if (result == -1) {
        perror("sys_recvmsg");
    }
    return result;
}

ssize_t sys_sendmsg(int sockfd, const struct msghdr *msg, int flags) {
    ssize_t result = sendmsg(sockfd, msg, flags);
    if (result == -1) {
        perror("sys_sendmsg");
    }
    return result;
}

ssize_t sys_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    ssize_t result = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (result == -1) {
        perror("sys_recvfrom");
    }
    return result;
}

ssize_t sys_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    ssize_t result = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (result == -1) {
        perror("sys_sendto");
    }
    return result;
}

int sys_socketpair(int domain, int type, int protocol, int sv[2]) {
    int result = socketpair(domain, type, protocol, sv);
    if (result == -1) {
        perror("sys_socketpair");
    }
    return result;
}

int sys_getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int result = getpeername(sockfd, addr, addrlen);
    if (result == -1) {
        perror("sys_getpeername");
    }
    return result;
}

int sys_getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int result = getsockname(sockfd, addr, addrlen);
    if (result == -1) {
        perror("sys_getsockname");
    }
    return result;
}

int sys_shutdown(int sockfd, int how) {
    int result = shutdown(sockfd, how);
    if (result == -1) {
        perror("sys_shutdown");
    }
    return result;
}

// System Timers
int sys_timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid) {
    int result = timer_create(clockid, sevp, timerid);
    if (result == -1) {
        perror("sys_timer_create");
    }
    return result;
}

int sys_timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value) {
    int result = timer_settime(timerid, flags, new_value, old_value);
    if (result == -1) {
        perror("sys_timer_settime");
    }
    return result;
}

int sys_timer_gettime(timer_t timerid, struct itimerspec *curr_value) {
    int result = timer_gettime(timerid, curr_value);
    if (result == -1) {
        perror("sys_timer_gettime");
    }
    return result;
}

int sys_timer_delete(timer_t timerid) {
    int result = timer_delete(timerid);
    if (result == -1) {
        perror("sys_timer_delete");
    }
    return result;
}

int sys_timer_getoverrun(timer_t timerid) {
    int result = timer_getoverrun(timerid);
    if (result == -1) {
        perror("sys_timer_getoverrun");
    }
    return result;
}

// Process Advanced Operations
int sys_setpriority(int which, id_t who, int prio) {
    int result = setpriority(which, who, prio);
    if (result == -1) {
        perror("sys_setpriority");
    }
    return result;
}

int sys_getpriority(int which, id_t who) {
    errno = 0;  // getpriority can return -1 as valid value
    int result = getpriority(which, who);
    if (result == -1 && errno != 0) {
        perror("sys_getpriority");
    }
    return result;
}

int sys_nice(int inc) {
    int result = nice(inc);
    if (result == -1) {
        perror("sys_nice");
    }
    return result;
}

int sys_setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value) {
    int result = setitimer(which, new_value, old_value);
    if (result == -1) {
        perror("sys_setitimer");
    }
    return result;
}

int sys_getitimer(int which, struct itimerval *curr_value) {
    int result = getitimer(which, curr_value);
    if (result == -1) {
        perror("sys_getitimer");
    }
    return result;
}

int sys_getrusage(int who, struct rusage *usage) {
    int result = getrusage(who, usage);
    if (result == -1) {
        perror("sys_getrusage");
    }
    return result;
}

pid_t sys_wait3(int *status, int options, struct rusage *rusage) {
    pid_t result = wait3(status, options, rusage);
    if (result == -1) {
        perror("sys_wait3");
    }
    return result;
}

pid_t sys_wait4(pid_t pid, int *status, int options, struct rusage *rusage) {
    pid_t result = wait4(pid, status, options, rusage);
    if (result == -1) {
        perror("sys_wait4");
    }
    return result;
}

// Security & Identity
uid_t sys_geteuid(void) {
    return geteuid();
}

gid_t sys_getegid(void) {
    return getegid();
}

int sys_setreuid(uid_t ruid, uid_t euid) {
    int result = setreuid(ruid, euid);
    if (result == -1) {
        perror("sys_setreuid");
    }
    return result;
}

int sys_setregid(gid_t rgid, gid_t egid) {
    int result = setregid(rgid, egid);
    if (result == -1) {
        perror("sys_setregid");
    }
    return result;
}

int sys_seteuid(uid_t euid) {
    int result = seteuid(euid);
    if (result == -1) {
        perror("sys_seteuid");
    }
    return result;
}

int sys_setegid(gid_t egid) {
    int result = setegid(egid);
    if (result == -1) {
        perror("sys_setegid");
    }
    return result;
}

int sys_getresuid(uid_t *ruid, uid_t *euid, uid_t *suid) {
    int result = getresuid(ruid, euid, suid);
    if (result == -1) {
        perror("sys_getresuid");
    }
    return result;
}

int sys_getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid) {
    int result = getresgid(rgid, egid, sgid);
    if (result == -1) {
        perror("sys_getresgid");
    }
    return result;
}

int sys_setresuid(uid_t ruid, uid_t euid, uid_t suid) {
    int result = setresuid(ruid, euid, suid);
    if (result == -1) {
        perror("sys_setresuid");
    }
    return result;
}

int sys_setresgid(gid_t rgid, gid_t egid, gid_t sgid) {
    int result = setresgid(rgid, egid, sgid);
    if (result == -1) {
        perror("sys_setresgid");
    }
    return result;
}

int sys_getgroups(int size, gid_t list[]) {
    int result = getgroups(size, list);
    if (result == -1) {
        perror("sys_getgroups");
    }
    return result;
}

int sys_setgroups(size_t size, const gid_t *list) {
    int result = setgroups(size, list);
    if (result == -1) {
        perror("sys_setgroups");
    }
    return result;
}

// File System Operations
int sys_quotactl(int cmd, const char *special, int id, caddr_t addr) {
    int result = quotactl(cmd, special, id, addr);
    if (result == -1) {
        perror("sys_quotactl");
    }
    return result;
}

int sys_swapon(const char *path, int swapflags) {
    int result = swapon(path, swapflags);
    if (result == -1) {
        perror("sys_swapon");
    }
    return result;
}

int sys_swapoff(const char *path) {
    int result = swapoff(path);
    if (result == -1) {
        perror("sys_swapoff");
    }
    return result;
}

int sys_syncfs(int fd) {
    int result = syncfs(fd);
    if (result == -1) {
        perror("sys_syncfs");
    }
    return result;
}

int sys_flock(int fd, int operation) {
    int result = flock(fd, operation);
    if (result == -1) {
        perror("sys_flock");
    }
    return result;
}