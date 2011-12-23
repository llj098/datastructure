CC=gcc
CFLAGS = -Wall -g

avl:
	$(CC) $(CFLAGS) avl.h avl_test.c -o $@
