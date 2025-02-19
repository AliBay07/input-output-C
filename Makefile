CC=gcc
DD=gdb
READ_FILE=files/test_read.txt
WRITE_FILE=files/test_write.txt
CFLAGS=-g -o

libc: eval-libc.c
	$(CC) eval-libc.c $(CFLAGS) eval-libc && ./eval-libc 

syscall: eval-syscall.c
	$(CC) eval-syscall.c $(CFLAGS) eval-syscall && ./eval-syscall $(READ_FILE) $(WRITE_FILE)

test: stdes.c test.c
	$(CC) stdes.c test.c $(CFLAGS) test && ./test $(READ_FILE) $(WRITE_FILE)

run_test:
	./test $(READ_FILE) $(WRITE_FILE)

mytest_read: stdes.c mytest_read.c
	clear && $(CC) stdes.c mytest_read.c -g -o mytest_read && ./mytest_read $(READ_FILE) $(WRITE_FILE)

run_mytest_read: stdes.c mytest_read.c
	./mytest_read $(READ_FILE) $(WRITE_FILE)

test_format: stdes.c test-format.c
	clear && $(CC) stdes.c test-format.c -g -o test-format && ./test-format $(WRITE_FILE) $(READ_FILE)

test_rand: stdes.c test-rand.c
	clear && $(CC) stdes.c test-rand.c -g -o test-rand && ./test-rand

debug_test: stdes.c test.c
	$(CC) stdes.c test.c $(CFLAGS) test && gdb test