# change to y to enable debugging support
DEBUG=n

# Directory variables
BUILD_DIR=build
BIN_DIR=$(BUILD_DIR)/bin
OBJ_DIR=$(BUILD_DIR)/obj
INCLUDE_DIR=include
SRC_DIR=src

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Create subdirectories for object files
$(shell mkdir -p $(OBJ_DIR)/infrastructure $(OBJ_DIR)/core $(OBJ_DIR)/interfaces)

# Compiler flags
CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -Wall -Wextra -fPIC -D_GNU_SOURCE
LDFLAGS=-shared
RPATH=-Wl,-rpath,'$$ORIGIN/../../src/interfaces'
LIBS=-lpthread -lrt

# Debug configuration
ifeq ($(DEBUG), y)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

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

# Check for libcurl for AI integration
CURL_CHECK := n
ifeq ($(PKG_CONFIG_EXISTS), y)
	CURL_CHECK := $(shell pkg-config --exists libcurl && echo "y" || echo "n")
	ifeq ($(CURL_CHECK), y)
		CFLAGS += $(shell pkg-config --cflags libcurl)
		LIBS += $(shell pkg-config --libs libcurl)
	else
		$(warning "libcurl not found. AI integration will be disabled.")
		CFLAGS += -DNO_AI_SUPPORT
	endif
else
	$(warning "pkg-config not found. Cannot check for libcurl. AI integration will be disabled.")
	CFLAGS += -DNO_AI_SUPPORT
endif

# Check for json-c for AI integration
JSON_CHECK := n
ifeq ($(PKG_CONFIG_EXISTS), y)
	JSON_CHECK := $(shell pkg-config --exists json-c && echo "y" || echo "n")
	ifeq ($(JSON_CHECK), y)
		CFLAGS += $(shell pkg-config --cflags json-c)
		LIBS += $(shell pkg-config --libs json-c)
	else
		$(warning "json-c not found. AI integration will be disabled.")
		CFLAGS += -DNO_AI_SUPPORT
	endif
else
	$(warning "pkg-config not found. Cannot check for json-c. AI integration will be disabled.")
	CFLAGS += -DNO_AI_SUPPORT
endif

# Source files
CORE_SRCS=$(wildcard $(SRC_DIR)/core/*.c)
INFRA_SRCS=$(wildcard $(SRC_DIR)/infrastructure/*.c)
INTERFACE_SRCS=$(wildcard $(SRC_DIR)/interfaces/*.c)
MAIN_SRC=$(SRC_DIR)/main.c
WEB_MAIN_SRC=$(SRC_DIR)/web_main.c

# Object files
CORE_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CORE_SRCS))
INFRA_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(INFRA_SRCS))
INTERFACE_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(INTERFACE_SRCS))
MAIN_OBJ=$(OBJ_DIR)/main.o
WEB_MAIN_OBJ=$(OBJ_DIR)/web_main.o

# Library output
SYSCALLS_LIB=$(SRC_DIR)/interfaces/libsyscalls.so

# Main executables
MAIN_APP=$(BIN_DIR)/main
WEB_APP=$(BIN_DIR)/web_server

# Default target
all: $(SYSCALLS_LIB) $(MAIN_APP)
ifeq ($(MHD_CHECK), y)
all: $(WEB_APP)
endif

# Create the syscalls library
$(SYSCALLS_LIB): $(INFRA_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile main.c
$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile web_main.c
$(WEB_MAIN_OBJ): $(WEB_MAIN_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for core objects
$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for infrastructure objects
$(OBJ_DIR)/infrastructure/%.o: $(SRC_DIR)/infrastructure/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for interface objects
$(OBJ_DIR)/interfaces/%.o: $(SRC_DIR)/interfaces/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Interface objects excluding web_server if MHD is not available
ifeq ($(MHD_CHECK), y)
FILTERED_INTERFACE_OBJS = $(INTERFACE_OBJS)
else
FILTERED_INTERFACE_OBJS = $(filter-out $(OBJ_DIR)/interfaces/web_server.o, $(INTERFACE_OBJS))
endif

# Create the main executable
$(MAIN_APP): $(MAIN_OBJ) $(CORE_OBJS) $(FILTERED_INTERFACE_OBJS) $(SYSCALLS_LIB)
	$(CC) -o $@ $(MAIN_OBJ) $(CORE_OBJS) $(FILTERED_INTERFACE_OBJS) -L$(SRC_DIR)/interfaces -lsyscalls $(RPATH) $(LIBS) $(if $(MHD_CHECK),$(WEB_LIBS),)

# Build the web interface if libmicrohttpd is available
ifeq ($(MHD_CHECK), y)
$(WEB_APP): $(WEB_MAIN_OBJ) $(INTERFACE_OBJS) $(CORE_OBJS) $(SYSCALLS_LIB)
	$(CC) -o $@ $(WEB_MAIN_OBJ) $(INTERFACE_OBJS) $(CORE_OBJS) -L$(SRC_DIR)/interfaces -lsyscalls $(RPATH) $(LIBS) $(WEB_LIBS)
endif

# Clean target
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* $(SRC_DIR)/interfaces/libsyscalls.so testfile.txt advanced_file_test.txt

# Web interface target
web: $(WEB_APP)

# Help target
help:
	@echo "Available targets:"
	@echo "  make         - Build main application and library"
	@echo "  make web     - Build web interface (requires libmicrohttpd)"
	@echo "  make clean   - Remove all build artifacts"
	@echo "  make help    - Show this help message"
	@echo "  DEBUG=y make - Build with debug symbols"

.PHONY: all clean help web