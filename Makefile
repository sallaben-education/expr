#
# File: Makefile
# Creator: Steve Allaben
# Last modified: <Sat, 14 Oct 2017 sallaben>
#

PROGRAMS = expr

CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror
OUT = expr
FILES = main.c rdp.c tdp.c node.c tree.c stack.c queue.c

auto: $(FILES)
	$(CC) $(CFLAGS) -o $(OUT) $(FILES)

valgrind: expr
	valgrind -v ./$(OUT) --leak-check=full --track-origins=yes 
