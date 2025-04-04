# change to y to enable debugging support
DEBUG=n

CC=gcc
CFLAGS=-Wall -Wextra -fPIC -Ilib -D_GNU_SOURCE
LDFLAGS=-shared
RPATH=-Wl,-rpath,'$$ORIGIN/../lib'
LIBS=-lpthread -lrt

# Check for capability support - we'll auto-detect if the header exists
CAP_CHECK := $(shell echo "\#include <sys/capability.h>" | $(CC) -E - >/dev/null 2>&1 && echo "y" || echo "n")
ifeq ($(CAP_CHECK), y)
    CFLAGS += -DHAVE_SYS_CAPABILITY_H
    LIBS += -lcap
endif

# Directory variables
LIB_DIR=lib
APP_DIR=app
OBJ_DIR=obj
BIN_DIR=bin

# Create directories if they don't exist
$(shell mkdir -p $(LIB_DIR) $(APP_DIR) $(OBJ_DIR) $(BIN_DIR))

# Source and object files
LIB_SRC=$(LIB_DIR)/syscalls.c
APP_SRC=$(APP_DIR)/main.c
LIB_OBJ=$(OBJ_DIR)/syscalls.o
APP_OBJ=$(OBJ_DIR)/main.o

# Library name
LIBNAME=$(LIB_DIR)/libsyscalls.so

ifeq ($(DEBUG), y)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

# Default target
all: $(LIBNAME) $(BIN_DIR)/main

# Create the library
$(LIBNAME): $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

# Compile library object file
$(LIB_OBJ): $(LIB_SRC) $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main application object file
$(APP_OBJ): $(APP_SRC) $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Create the main executable
$(BIN_DIR)/main: $(APP_OBJ) $(LIBNAME)
	$(CC) -o $@ $< -L$(LIB_DIR) -lsyscalls $(RPATH) $(LIBS)

clean:
	rm -f $(OBJ_DIR)/*.o $(LIBNAME) $(BIN_DIR)/* testfile.txt

help:
	@echo "make: compile the library and the main application"
	@echo "make clean: remove object files and generated files"
	@echo "make help: display this message"
	@echo "DEBUG=y make: compile with debugging support"

.PHONY: all clean help