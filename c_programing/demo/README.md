# System Call Library Demo

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![Language: C](https://img.shields.io/badge/Language-C-green.svg)

> A comprehensive library that provides wrappers around Linux system calls for simplified system-level programming.

## 📚 Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Building Instructions](#building-instructions)
  - [Building the Library](#building-the-library)
  - [Building the Application](#building-the-application)
- [Example Usage](#example-usage)
- [Supported System Calls](#supported-system-calls)
- [Library Functionalities](#library-functionalities)
  - [File Management](#file-management)
  - [Process Management](#process-management)
  - [Signal Handling](#signal-handling)
  - [Memory Management](#memory-management)
  - [Directory Management](#directory-management)
  - [Networking](#networking)
  - [Thread Management](#thread-management)
  - [Time Management](#time-management)
  - [Resource Management](#resource-management)
  - [File Permission Handling](#file-permission-handling)
  - [Asynchronous I/O](#asynchronous-io)
  - [Inter-Process Communication (IPC)](#inter-process-communication-ipc)
- [Error Handling](#error-handling)

## 📋 Overview

This project demonstrates how to create a custom system call library and an application that uses it. The library provides a clean, consistent interface to Linux system calls, making system-level programming more accessible and maintainable.
(xây dựng 1 thư viện các lời gọi hệ thống và ứng dụng).

## 🏗️ Project Structure

```
demo/
├── lib/
│   ├── syscall.h
│   └── syscall.c
├── app/
│   └── main.c
├── bin/
│   └── main.c
├── obj/
│   └── main.c
└── Makefile
└── README.md
```

## 🔧 Building Instructions

### Building the Library

```bash
# Navigate to the lib directory
cd lib

# Compile the library
gcc -c syscall.c -o syscall.o
ar rcs libsyscall.a syscall.o
```

### Building the Application

```bash
# Ensure the library is built first
cd ../app

# Compile the application
gcc -o app main.c -L../lib -lsyscall
```

## 🚀 Example Usage

```c
// In your application
#include "syscall.h"

int main() {
    // Open a file
    int fd = sys_open("test.txt", O_RDONLY);

    // Read from file
    char buffer[100];
    int bytes = sys_read(fd, buffer, sizeof(buffer));

    // Close file
    sys_close(fd);

    return 0;
}
```

## ✅ Supported System Calls

The library currently wraps the following system calls:

- `sys_open()`: Open or create a file
- `sys_read()`: Read from file descriptor
- `sys_write()`: Write to file descriptor
- `sys_close()`: Close file descriptor
- `sys_lseek()`: Reposition file offset
- `sys_stat()`: Get file status

## 📖 Library Functionalities

### File Management

Functions for file I/O operations:

| Function                                                      | Description                           |
| ------------------------------------------------------------- | ------------------------------------- |
| `int sys_open(const char *filename, int flags, mode_t mode)`  | Opens or creates a file               |
| `ssize_t sys_read(int fd, void *buffer, size_t count)`        | Reads data from a file into a buffer  |
| `ssize_t sys_write(int fd, const void *buffer, size_t count)` | Writes data from a buffer into a file |
| `int sys_close(int fd)`                                       | Closes an open file                   |
| `off_t sys_lseek(int fd, off_t offset, int whence)`           | Repositions the file offset           |

> **Use Case**: Storing logs, handling configuration files, and managing user data.

### Process Management

Functions for creating and controlling processes:

| Function                                                                    | Description                                |
| --------------------------------------------------------------------------- | ------------------------------------------ |
| `pid_t sys_fork()`                                                          | Creates a child process                    |
| `pid_t sys_wait(int *status)`                                               | Waits for a child process to terminate     |
| `int sys_exit(int status)`                                                  | Terminates the current process             |
| `pid_t sys_getpid()`                                                        | Gets the process ID of the current process |
| `int sys_execve(const char *path, char *const argv[], char *const envp[]);` | Executes a new program                     |

> **Use Case**: Running multiple processes, executing shell commands from C programs.

### Signal Handling

Functions for inter-process communication via signals:

| Function                                                                               | Description                                   |
| -------------------------------------------------------------------------------------- | --------------------------------------------- |
| `int sys_kill(pid_t pid, int sig)`                                                     | Sends a signal to a process                   |
| `int sys_pause()`                                                                      | Suspends execution until a signal is received |
| `int sys_sigaction(int signum, const struct sigaction *act, struct sigaction*oldact);` | Sets signal handling for a process            |

> **Use Case**: Handling interruptions (SIGINT), stopping or killing processes.

### Memory Management

Functions for efficient memory allocation:

| Function                                                                               | Description                                        |
| -------------------------------------------------------------------------------------- | -------------------------------------------------- |
| `void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)` | Maps a file or memory to a process's address space |
| `int sys_munmap(void *addr, size_t length)`                                            | Unmaps a memory region                             |
| `int sys_brk(void *addr)`                                                              | Changes the end of the data segment                |

> **Use Case**: Shared memory between processes, efficient memory handling.

### Directory Management

Functions to handle directories and file system structure:

| Function                                       | Description                   |
| ---------------------------------------------- | ----------------------------- |
| `int sys_mkdir(const char *path, mode_t mode)` | Creates a new directory       |
| `int sys_rmdir(const char *path)`              | Removes a directory           |
| `int sys_chdir(const char *path)`              | Changes the working directory |

> **Use Case**: Creating and managing directory structures for applications.

### Networking

Functions for socket programming and network communication:

| Function                                                                      | Description                      |
| ----------------------------------------------------------------------------- | -------------------------------- |
| `int sys_socket(int domain, int type, int protocol)`                          | Creates a socket                 |
| `int sys_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`    | Binds a socket to an address     |
| `int sys_listen(int sockfd, int backlog)`                                     | Listens for incoming connections |
| `int sys_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`       | Accepts an incoming connection   |
| `int sys_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)` | Connects to a remote server      |
| `ssize_t sys_send(int sockfd, const void *buffer, size_t length, int flags)`  | Sends data over a socket         |
| `ssize_t sys_recv(int sockfd, void *buffer, size_t length, int flags)`        | Receives data from a socket      |

> **Use Case**: Creating a simple server-client communication system.

### Thread Management

Functions for creating and managing threads:

| Function                                                                                                   | Description                                    |
| ---------------------------------------------------------------------------------------------------------- | ---------------------------------------------- |
| `int sys_clone(int (*fn)(void *), void *stack, int flags, void *arg);`                                     | Creates a new thread using `clone` system call |
| `int sys_futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3);` | Fast userspace mutex for synchronization       |

> **Use Case**: Implementing multi-threaded applications for concurrent processing and responsive UIs.

### Time Management

| Function                                                               | Description                             |
| ---------------------------------------------------------------------- | --------------------------------------- |
| `int sys_gettimeofday(struct timeval *tv, struct timezone *tz);`       | Gets the current time of day            |
| `int sys_nanosleep(const struct timespec *req, struct timespec *rem);` | Suspends execution for a specified time |

### Resource Management

| Function                                                       | Description                        |
| -------------------------------------------------------------- | ---------------------------------- |
| `int sys_getrlimit(int resource, struct rlimit \*rlim);`       | Gets resource limits for a process |
| `int sys_setrlimit(int resource, const struct rlimit \*rlim);` | Sets resource limits for a process |

### File Permission Handling

Functions to manage file permissions and ownership:

| Function                                                    | Description                  |
| ----------------------------------------------------------- | ---------------------------- |
| `int sys_chmod(const char *path, mode_t mode)`              | Changes file permissions     |
| `int sys_chown(const char *path, uid_t owner, gid_t group)` | Changes file owner and group |
| `int sys_access(const char *path, int mode)`                | Checks file accessibility    |
| `int sys_umask(mode_t mask)`                                | Sets file mode creation mask |
| `mode_t sys_getumask()`                                     | Gets the current umask value |

> **Use Case**: Implementing secure file operations with proper permissions management.

### Asynchronous I/O

Functions for non-blocking file operations:

| Function                                                                                          | Description                                       |
| ------------------------------------------------------------------------------------------------- | ------------------------------------------------- |
| `int sys_aio_read(struct aiocb *aiocbp)`                                                          | Asynchronous read from file                       |
| `int sys_aio_write(struct aiocb *aiocbp)`                                                         | Asynchronous write to file                        |
| `int sys_aio_error(const struct aiocb *aiocbp)`                                                   | Gets completion status of asynchronous request    |
| `ssize_t sys_aio_return(struct aiocb *aiocbp)`                                                    | Gets return status of completed request           |
| `int sys_aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout)` | Waits for asynchronous I/O operations to complete |
| `int sys_aio_cancel(int fd, struct aiocb *aiocbp)`                                                | Cancels an outstanding asynchronous I/O request   |

> **Use Case**: Implementing high-performance I/O operations without blocking the main thread, especially useful for applications handling multiple files simultaneously.

### Inter-Process Communication (IPC)

| Function                                                       | Description                                    |
| -------------------------------------------------------------- | ---------------------------------------------- |
| `int sys_pipe(int pipefd[2]);`                                 | Creates a pipe for inter-process communication |
| `int sys_shmget(key_t key, size_t size, int shmflg);`          | Allocates a shared memory segment              |
| `void *sys_shmat(int shmid, const void *shmaddr, int shmflg);` | Attaches a shared memory segment               |
| `int sys_shmdt(const void *shmaddr);`                          | Detaches a shared memory segment               |

## ⚠️ Error Handling

All functions in the library follow a consistent error handling pattern:

- Return `-1` on error
- Set `errno` with appropriate error code
- Check `errno` to get detailed error information

```c
int fd = sys_open("nonexistent.txt", O_RDONLY);
if (fd == -1) {
    perror("Error opening file");
    // Handle the error appropriately
}
```
