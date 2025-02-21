CC=gcc
CFLAGS=-Wall -Wextra -fPIC -Ilib
LDFLAGS=-shared
RPATH=-Wl,-rpath,'$$ORIGIN/../lib'

# Directory variables
LIB_DIR=lib
APP_DIR=app

# Source and object files
LIB_SRC=$(LIB_DIR)/syscalls.c
APP_SRC=$(APP_DIR)/main.c
LIB_OBJ=$(LIB_DIR)/syscalls.o
APP_OBJ=$(APP_DIR)/main.o

# Library name
LIBNAME=$(LIB_DIR)/libsyscalls.so

# Default target
all: $(LIBNAME) $(APP_DIR)/main.o

# Create the library
$(LIBNAME): $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $<

# Compile the object file for the library
$(LIB_OBJ): $(LIB_SRC) $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Create the main application
$(APP_DIR)/main.o: $(APP_SRC) $(LIBNAME)
	$(CC) $(CFLAGS) $(APP_SRC) -o $@ -L$(LIB_DIR) -lsyscalls $(RPATH)

clean:
	rm -f $(LIB_OBJ) $(APP_OBJ) $(APP_DIR)/*.*o $(LIB_DIR)/*.*o

.PHONY: all clean