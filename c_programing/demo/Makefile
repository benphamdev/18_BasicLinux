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

# Check for pkg-config
PKG_CONFIG_EXISTS := $(shell which pkg-config >/dev/null 2>&1 && echo "y" || echo "n")

# Check for libmicrohttpd support for web interface
MHD_CHECK := n
ifeq ($(PKG_CONFIG_EXISTS), y)
	MHD_CHECK := $(shell pkg-config --exists libmicrohttpd && echo "y" || echo "n")
	ifeq ($(MHD_CHECK), y)
		CFLAGS += $(shell pkg-config --cflags libmicrohttpd)
		WEB_LIBS = $(shell pkg-config --libs libmicrohttpd)
	else
		$(warning "libmicrohttpd not found. Will not build web interface.")
	endif
else
	$(warning "pkg-config not found. Cannot check for libmicrohttpd.")
endif

# Directory variables
LIB_DIR=lib
APP_DIR=app
OBJ_DIR=obj
BIN_DIR=bin
SRC_DIR=src
WEB_DIR=web

# Create directories if they don't exist
$(shell mkdir -p $(LIB_DIR) $(APP_DIR) $(OBJ_DIR) $(BIN_DIR) $(SRC_DIR) $(WEB_DIR))

# Source and object files
LIB_SRC=$(LIB_DIR)/syscalls.c
APP_SRC=$(APP_DIR)/main.c
LIB_OBJ=$(OBJ_DIR)/syscalls.o
APP_OBJ=$(OBJ_DIR)/main.o

# Demos source and object
DEMOS_SRC=$(SRC_DIR)/demos.c
DEMOS_OBJ=$(OBJ_DIR)/demos.o

# Web server source and objects
WEB_SERVER_SRC=$(SRC_DIR)/web_server.c
WEB_APP_SRC=$(APP_DIR)/web_main.c
WEB_SERVER_OBJ=$(OBJ_DIR)/web_server.o
WEB_APP_OBJ=$(OBJ_DIR)/web_main.o

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

# Compile demos object file
$(DEMOS_OBJ): $(DEMOS_SRC) $(SRC_DIR)/demos.h $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main application object file
$(APP_OBJ): $(APP_SRC) $(LIB_DIR)/syscalls.h $(SRC_DIR)/demos.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile web server object file
$(WEB_SERVER_OBJ): $(WEB_SERVER_SRC) $(SRC_DIR)/web_server.h $(LIB_DIR)/syscalls.h $(SRC_DIR)/demos.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile web application object file
$(WEB_APP_OBJ): $(WEB_APP_SRC) $(SRC_DIR)/web_server.h $(LIB_DIR)/syscalls.h
	$(CC) $(CFLAGS) -c $< -o $@

# Create the main executable - ensure DEMOS_OBJ exists before building main
$(BIN_DIR)/main: $(APP_OBJ) $(DEMOS_OBJ) $(LIBNAME)
	$(CC) -o $@ $(APP_OBJ) $(DEMOS_OBJ) -L$(LIB_DIR) -lsyscalls $(RPATH) $(LIBS)

# Build the web interface if libmicrohttpd is available
ifeq ($(MHD_CHECK), y)
all: $(BIN_DIR)/web_server

$(BIN_DIR)/web_server: $(WEB_SERVER_OBJ) $(WEB_APP_OBJ) $(DEMOS_OBJ) $(LIBNAME)
	$(CC) -o $@ $(WEB_SERVER_OBJ) $(WEB_APP_OBJ) $(DEMOS_OBJ) -L$(LIB_DIR) -lsyscalls $(RPATH) $(LIBS) $(WEB_LIBS)
endif

clean:
	rm -f $(OBJ_DIR)/*.o $(LIBNAME) $(BIN_DIR)/* testfile.txt

web: $(BIN_DIR)/web_server

help:
	@echo "make: compile the library and the main application"
	@echo "make web: compile the web interface (requires libmicrohttpd)"
	@echo "make clean: remove object files and generated files"
	@echo "make help: display this message"
	@echo "DEBUG=y make: compile with debugging support"

.PHONY: all clean help web