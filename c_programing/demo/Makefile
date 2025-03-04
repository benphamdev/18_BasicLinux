# change to y to enable debugging support
DEBUG=n

CC=gcc
CFLAGS=-Wall -Wextra -fPIC -Ilib
LDFLAGS=-shared
RPATH=-Wl,-rpath,'$$ORIGIN/../lib'

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
	$(CC) $(LDFLAGS) -o $@ $<

# Compile library object file
$(LIB_OBJ): $(LIB_SRC) $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main application object file
$(APP_OBJ): $(APP_SRC) $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Create the main executable
$(BIN_DIR)/main: $(APP_OBJ) $(LIBNAME)
	$(CC) -o $@ $< -L$(LIB_DIR) -lsyscalls $(RPATH)

clean:
	rm -f $(OBJ_DIR)/*.o $(LIBNAME) $(BIN_DIR)/*

help:
	@echo "make: compile the library and the main application"
	@echo "make clean: remove object files"
	@echo "make help: display this message"
	@echo "DEBUG=y make: compile with debugging support"

.PHONY: all clean help