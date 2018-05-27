# 
# Author: dmliscinsky
# 

CC = gcc
CPP = g++
CFLAGS = -s -O3 -Wall -Wno-comment
CPPFLAGS = -std=c++11 -fpermissive
LIBS = -lpthread


EXAMPLE_DIR = ./examples


PROGS = subprocess_ipc.so $(EXAMPLE_DIR)/example_shell_1


.PHONY: clean


all: $(PROGS)


subprocess_ipc.so: ipc.c ipc.h debug.h
	$(CC) $(CFLAGS) ipc.c -o $@ -fPIC -shared
	

$(EXAMPLE_DIR)/example_shell_1: $(EXAMPLE_DIR)/example_shell_1.c
	$(CC) $(CFLAGS) $^ -o $@


clean:
	@rm -f $(PROGS) *.o
