MAKEFLAGS += --silent --jobs=12

CC = gcc
CFLAGS = -c -Wall -Werror -Wextra -Wpedantic -fPIC -O3
LDFLAGS = -shared

TARGET = libescape
HEADER = escape.h 
TARGET_SHARED = $(TARGET).so
TARGET_STATIC = $(TARGET).a

PREFIX=/usr/local
INSTALL_DIR = $(PREFIX)/lib
INCLUDE_DIR = $(PREFIX)/include

SRC_DIR = ./src
SRCS = $(shell find $(SRC_DIR) -name '*.c')
BUILD_DIR = ./build
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

esc		= 
r		= $(esc)[0m
b		= $(esc)[1m
black	= $(esc)[30m
red		= $(esc)[31m
green	= $(esc)[32m
yellow	= $(esc)[33m
blue	= $(esc)[34m
magenta = $(esc)[35m
cyan	= $(esc)[36m
white	= $(esc)[37m

all: shared static

shared: $(TARGET_SHARED)

static: $(TARGET_STATIC)

$(TARGET_SHARED): $(OBJS)
	printf "$(yellow)Creating $(cyan)$(b)$@$(r)\n"
	$(CC) $(LDFLAGS) $^ -o $@
	strip $@

$(TARGET_STATIC): $(OBJS)
	printf "$(yellow)Creating $(cyan)$(b)$@$(r)\n"
	ar rcs $@  $^

$(BUILD_DIR)/%.o: %.c
	printf "$(magenta)Compiling $(cyan)$(b)$(shell basename $^)$(r)\n"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

install: $(TARGET)*
	printf "$(green)Installing $(cyan)$(b)$(HEADER)$^$(r)\n"
	mkdir -p $(INCLUDE_DIR)
	cp $(SRC_DIR)/$(HEADER) $(INCLUDE_DIR)
	mkdir -p $(INSTALL_DIR)
	cp $^ $(INSTALL_DIR)

uninstall:
	printf "$(red)Uninstalling $(cyan)$(b)$(INCLUDE_DIR)/$(HEADER)"
	rm -f $(INCLUDE_DIR)/$(HEADER)
	printf "$(shell ls $(INSTALL_DIR)/$(TARGET)*)\n"
	rm -f $(shell find $(INSTALL_DIR) -name '$(TARGET).*') 

examples: $(TARGET_STATIC)
	printf "$(yellow)Creating example $(cyan)$(b)demo$(r)\n"
	$(CC) example/demo.c $^ -o demo
	printf "$(yellow)Creating example $(cyan)$(b)benchmark$(r)\n"
	$(CC) example/benchmark.c $^ -o benchmark

clean:
	printf "$(red)Removing $(cyan)$(b)"
	printf "$(shell find . -name 'build') "
	rm -rf $(BUILD_DIR)
	printf "$(shell find . -name '$(TARGET).*') "
	rm -f $(TARGET)*
	printf "$(shell find . -name 'demo') "
	rm -f demo
	printf "$(shell find . -name 'benchmark')$(r)\n"
	rm -f benchmark

.PHONY: all shared static install-shared install-static install uninstall examples clean
