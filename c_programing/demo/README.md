# System Call Library Demo

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![Language: C](https://img.shields.io/badge/Language-C-green.svg)

> A comprehensive library that provides wrappers around Linux system calls for simplified system-level programming.

## 📚 Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [Building Instructions](#building-instructions)
  - [Building the Library](#building-the-library)
  - [Building the Application](#building-the-application)
  - [Building the Web Interface](#building-the-web-interface)
- [Running the Web Server](#running-the-web-server)
- [DeepSeek AI Chat Interface](#deepseek-ai-chat-interface)
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
  - [Terminal I/O](#terminal-io)
  - [Message Queues](#message-queues)
  - [Semaphores](#semaphores)
  - [Event Monitoring](#event-monitoring)
  - [File Monitoring](#file-monitoring)
  - [Process Scheduling](#process-scheduling)
  - [Process Control](#process-control)
  - [Capabilities](#capabilities)
- [Error Handling](#error-handling)
- [AI Integration](#ai-integration)

## 📋 Overview

This project demonstrates how to create a custom system call library and an application that uses it. The library provides a clean, consistent interface to Linux system calls, making system-level programming more accessible and maintainable.
(xây dựng 1 thư viện các lời gọi hệ thống và ứng dụng).

## 🏗️ Project Structure

```
demo/
├── include/           # Header files
│   ├── demos.h
│   ├── syscalls.h
│   ├── web_server.h
│   └── ai_integration.h  # DeepSeek AI integration
├── src/               # Source files
│   ├── main.c         # Main application entry point
│   ├── web_main.c     # Web server entry point
│   ├── core/          # Core functionality
│   │   └── demos.c    # Demo implementations
│   ├── infrastructure/
│   │   └── syscalls.c # System call wrappers
│   └── interfaces/
│       ├── web_server.c   # Web interface
│       └── ai_integration.c # DeepSeek AI integration
├── build/             # Build artifacts
│   ├── bin/           # Executables
│   └── obj/           # Object files
├── data/              # Data files
│   ├── test.txt
│   └── .env           # Environment file for DeepSeek API key
├── web/               # Web interface files
│   ├── assets/
│   │   └── logo.png
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   └── app.js
│   └── templates/
│       ├── index.html
│       ├── header.html
│       ├── footer.html
│       └── deepseek_chat.html # DeepSeek AI chat interface
├── Makefile           # Build automation
└── README.md          # Documentation
```

## 📦 Dependencies

Before building the project, ensure you have the required dependencies:

```bash
# Install libmicrohttpd for the web interface
sudo apt-get install pkg-config libmicrohttpd-dev

# Install libcurl and json-c for DeepSeek AI integration
sudo apt-get install libcurl4-openssl-dev libjson-c-dev
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

### Building the Web Interface

You can build the entire project including the web interface with a single command:

```bash
# Build everything (library, application, and web interface)
make

# Or to build just the web interface
make web
```

## 🌐 Running the Web Server

After building the project, you can run the web server with:

```bash
./build/bin/web_server
```

The server will start and you can access the web interface by navigating to <http://localhost:8080> in your web browser.

## 🤖 DeepSeek AI Chat Interface

The project includes an AI-powered chat interface that lets you interact with DeepSeek AI about your codebase. This feature allows you to ask questions about the project, request explanations of system calls, or get help with programming issues.

### Setting up DeepSeek AI

1. Create a `.env` file in your project root or `data/` directory with your DeepSeek API key:

   ```
   DEEPSEEK_API_KEY=your_api_key_here
   ```

2. The web server will automatically initialize the AI system on startup.

### Using the AI Chat Interface

1. Navigate to the main page at <http://localhost:8080>
2. Click on "Chat with DeepSeek AI about this project"
3. In the chat interface, click "Get Project Context" to allow the AI to scan and understand your codebase
4. Type your questions in the chat box and receive AI-powered responses

### Features of the AI Chat Interface

- Code-aware context: The AI understands your project structure and can reference specific files
- Code formatting: Code snippets in responses are properly formatted with syntax highlighting
- Project structure scanning: Automatically builds context from your codebase
- Real-time interaction: Immediate responses to your questions about the project

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

### Terminal I/O

Functions for terminal device manipulation:

| Function                                                                           | Description                       |
| ---------------------------------------------------------------------------------- | --------------------------------- |
| `int sys_tcgetattr(int fd, struct termios *termios_p)`                             | Gets terminal attributes          |
| `int sys_tcsetattr(int fd, int optional_actions, const struct termios *termios_p)` | Sets terminal attributes          |
| `int sys_tcsendbreak(int fd, int duration)`                                        | Sends a break condition           |
| `int sys_tcflush(int fd, int queue_selector)`                                      | Flushes terminal I/O buffer       |
| `int sys_ioctl(int fd, unsigned long request, void *arg)`                          | Controls device parameters        |
| `int sys_isatty(int fd)`                                                           | Tests if file descriptor is a TTY |

> **Use Case**: Creating console applications with custom input handling, implementing terminal-based UIs, or setting raw mode for games.

### Message Queues

Functions for POSIX message queue operations:

| Function                                                                                  | Description                   |
| ----------------------------------------------------------------------------------------- | ----------------------------- |
| `mqd_t sys_mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr)`       | Opens a message queue         |
| `int sys_mq_close(mqd_t mqdes)`                                                           | Closes a message queue        |
| `int sys_mq_unlink(const char *name)`                                                     | Removes a message queue       |
| `int sys_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int prio)`    | Sends a message               |
| `ssize_t sys_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *prio)`  | Receives a message            |
| `int sys_mq_getattr(mqd_t mqdes, struct mq_attr *attr)`                                   | Gets message queue attributes |
| `int sys_mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr)` | Sets message queue attributes |

> **Use Case**: Implementing reliable inter-process communication with priority support.

### Semaphores

Functions for POSIX semaphore operations:

| Function                                                                            | Description                              |
| ----------------------------------------------------------------------------------- | ---------------------------------------- |
| `sem_t *sys_sem_open(const char *name, int oflag, mode_t mode, unsigned int value)` | Opens a named semaphore                  |
| `int sys_sem_close(sem_t *sem)`                                                     | Closes a semaphore                       |
| `int sys_sem_unlink(const char *name)`                                              | Removes a named semaphore                |
| `int sys_sem_wait(sem_t *sem)`                                                      | Locks a semaphore                        |
| `int sys_sem_trywait(sem_t *sem)`                                                   | Non-blocking attempt to lock a semaphore |
| `int sys_sem_post(sem_t *sem)`                                                      | Unlocks a semaphore                      |
| `int sys_sem_getvalue(sem_t *sem, int *sval)`                                       | Gets semaphore value                     |

> **Use Case**: Synchronizing access to shared resources between processes.

### Event Monitoring

Functions for monitoring file descriptors for events:

| Function                                                                                             | Description                                                 |
| ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- |
| `int sys_poll(struct pollfd *fds, nfds_t nfds, int timeout)`                                         | Waits for events on multiple file descriptors               |
| `int sys_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *tv)` | Monitors multiple file descriptors for readiness            |
| `int sys_epoll_create(int size)`                                                                     | Creates an epoll instance (scalable I/O event notification) |
| `int sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`                             | Controls epoll instance                                     |
| `int sys_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)`               | Waits for events on an epoll instance                       |

> **Use Case**: Building high-performance servers that handle multiple connections efficiently.

### File Monitoring

Functions for monitoring file system events:

| Function                                                             | Description                              |
| -------------------------------------------------------------------- | ---------------------------------------- |
| `int sys_inotify_init(void)`                                         | Initializes a new inotify instance       |
| `int sys_inotify_add_watch(int fd, const char *path, uint32_t mask)` | Adds a watch to an inotify instance      |
| `int sys_inotify_rm_watch(int fd, int wd)`                           | Removes a watch from an inotify instance |

> **Use Case**: File system event monitoring for auto-reload functionality in applications, implementing backup systems, or file synchronization tools.

### Process Scheduling

Functions for controlling process scheduling attributes:

| Function                                                                             | Description                           |
| ------------------------------------------------------------------------------------ | ------------------------------------- |
| `int sys_sched_get_priority_max(int policy)`                                         | Gets maximum scheduling priority      |
| `int sys_sched_get_priority_min(int policy)`                                         | Gets minimum scheduling priority      |
| `int sys_sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)` | Sets scheduling policy and parameters |
| `int sys_sched_getscheduler(pid_t pid)`                                              | Gets scheduling policy                |
| `int sys_sched_setparam(pid_t pid, const struct sched_param *param)`                 | Sets scheduling parameters            |
| `int sys_sched_getparam(pid_t pid, struct sched_param *param)`                       | Gets scheduling parameters            |
| `int sys_sched_yield(void)`                                                          | Yields processor voluntarily          |

> **Use Case**: Implementing real-time applications, prioritizing critical tasks, or optimizing CPU utilization in multi-threaded applications.

### Process Control

Functions for advanced process control:

| Function                                                                                                    | Description                 |
| ----------------------------------------------------------------------------------------------------------- | --------------------------- |
| `int sys_prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)` | Controls process attributes |

> **Use Case**: Setting process name, controlling core dumps, managing security-related process attributes.

### Capabilities

Functions for Linux capabilities (fine-grained privileges):

| Function                                                              | Description               |
| --------------------------------------------------------------------- | ------------------------- |
| `int sys_capget(cap_user_header_t hdrp, cap_user_data_t datap)`       | Gets process capabilities |
| `int sys_capset(cap_user_header_t hdrp, const cap_user_data_t datap)` | Sets process capabilities |

> **Use Case**: Implementing principle of least privilege in security-sensitive applications.

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

## 🧠 AI Integration

This project integrates DeepSeek's AI capabilities to provide contextual assistance for developers working with the system call library.

### AI Features

- **Contextual Understanding**: The AI can scan your project structure and understand the relationships between different components
- **Code Assistance**: Get help with implementation details, debugging, and understanding complex system calls
- **Interactive Chat**: Natural language interface to ask questions about the codebase
- **Project Context**: The AI maintains context about your specific project during the conversation

### API Configuration

To use the DeepSeek AI features, you need to:

1. Obtain an API key from DeepSeek
2. Create a `.env` file in your project root or data directory with the following content:

   ```
   DEEPSEEK_API_KEY=your_api_key_here
   ```

### API Functions

The AI integration provides several functions that can be used in your own applications:

```c
// Initialize AI with API key
bool ai_init(const char *api_key);

// Initialize AI from .env file
bool ai_init_from_env_file(const char *env_file_path);

// Generate text using DeepSeek AI
char *ai_generate_text(const char *prompt, const char *model_name);

// Clean up AI resources
void ai_cleanup(void);

// Set AI response randomness (0.0-1.0)
void ai_set_temperature(float temp);
```

### Example AI Integration

```c
#include "ai_integration.h"
#include <stdio.h>

int main() {
    // Initialize AI from environment file
    if (!ai_init_from_env_file(NULL)) {
        fprintf(stderr, "Failed to initialize AI\n");
        return 1;
    }

    // Generate text with DeepSeek
    char *response = ai_generate_text(
        "Explain how the sys_open function works", NULL);

    if (response) {
        printf("AI Response:\n%s\n", response);
        free(response);
    }

    // Clean up
    ai_cleanup();
    return 0;
}
```

### Web Interface

The AI capabilities are also available through the built-in web interface:

1. Navigate to <http://ip-web-server:8080/deepseek-chat> in your browser
2. Click "Chat with DeepSeek AI about this project"
3. Use the chat interface to ask questions about your code

This interface is particularly useful for exploring the functionality of the system call library and understanding how different parts work together.
