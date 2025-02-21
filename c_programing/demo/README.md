# System Call Library Demo

This project demonstrates how to create a custom system call library and an application that uses it.

## Project Structure

```
demo/
├── lib/
│   ├── syscall.h
│   └── syscall.c
├── app/
│   └── main.c
└── Makefile
```

## Building the Library

1. Navigate to the lib directory
2. Compile the library:

```bash
gcc -c syscall.c -o syscall.o
ar rcs libsyscall.a syscall.o
```

## Building the Application

1. Make sure the library is built first
2. Compile the application:

```bash
gcc -o app main.c -L./lib -lsyscall
```

## Example Usage

The library provides system call wrappers for common operations:

```c
// In my application
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

## Supported System Calls

- sys_open(): Open files
- sys_read(): Read from file descriptor
- sys_write(): Write to file descriptor
- sys_close(): Close file descriptor
- sys_lseek(): Reposition file offset
- sys_stat(): Get file status

## Error Handling

All functions return -1 on error and set errno appropriately. Check errno for detailed error information.
