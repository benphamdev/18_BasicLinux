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
  - [File Permission Handling](#file-permission-handling)
  - [Asynchronous I/O](#asynchronous-io)
- [Error Handling](#error-handling)

## 📋 Overview

This project demonstrates how to create a custom system call library and an application that uses it. The library provides a clean, consistent interface to Linux system calls, making system-level programming more accessible and maintainable.

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

| Function                                             | Description                                 |
| ---------------------------------------------------- | ------------------------------------------- |
| `pid_t sys_fork()`                                   | Creates a child process                     |
| `int sys_exec(const char *path, char *const argv[])` | Replaces current process with a new program |
| `pid_t sys_wait(int *status)`                        | Waits for a child process to terminate      |
| `int sys_exit(int status)`                           | Terminates the current process              |
| `pid_t sys_getpid()`                                 | Gets the process ID of the current process  |

> **Use Case**: Running multiple processes, executing shell commands from C programs.

### Signal Handling

Functions for inter-process communication via signals:

| Function                                            | Description                                   |
| --------------------------------------------------- | --------------------------------------------- |
| `int sys_kill(pid_t pid, int sig)`                  | Sends a signal to a process                   |
| `void sys_signal(int signum, void (*handler)(int))` | Sets a handler for a signal                   |
| `int sys_pause()`                                   | Suspends execution until a signal is received |

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

| Function                                       | Description                        |
| ---------------------------------------------- | ---------------------------------- |
| `int sys_mkdir(const char *path, mode_t mode)` | Creates a new directory            |
| `int sys_rmdir(const char *path)`              | Removes a directory                |
| `int sys_chdir(const char *path)`              | Changes the working directory      |
| `char *sys_getcwd(char *buffer, size_t size)`  | Gets the current working directory |

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

| Function                                                                                                            | Description                   |
| ------------------------------------------------------------------------------------------------------------------- | ----------------------------- |
| `int sys_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)` | Creates a new thread          |
| `int sys_pthread_join(pthread_t thread, void **retval)`                                                             | Waits for thread termination  |
| `int sys_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)`                               | Initializes a mutex           |
| `int sys_pthread_mutex_lock(pthread_mutex_t *mutex)`                                                                | Locks a mutex                 |
| `int sys_pthread_mutex_unlock(pthread_mutex_t *mutex)`                                                              | Unlocks a mutex               |
| `int sys_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)`                                           | Waits on a condition variable |
| `int sys_pthread_cond_signal(pthread_cond_t *cond)`                                                                 | Signals a condition variable  |

> **Use Case**: Implementing multi-threaded applications for concurrent processing and responsive UIs.

### File Permission Handling

Functions to manage file permissions and ownership:

| Function                                                    | Description                          |
| ----------------------------------------------------------- | ------------------------------------ |
| `int sys_chmod(const char *path, mode_t mode)`              | Changes file permissions             |
| `int sys_chown(const char *path, uid_t owner, gid_t group)` | Changes file owner and group         |
| `int sys_access(const char *path, int mode)`                | Checks file accessibility            |
| `int sys_umask(mode_t mask)`                                | Sets file mode creation mask         |
| `int sys_getumask(void)`                                    | Gets current file mode creation mask |

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
