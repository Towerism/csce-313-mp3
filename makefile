# makefile

TARGET=memtest
CFLAGS=-c -g

all: memtest

ackerman.o: ackerman.c 
	gcc $(CFLAGS) ackerman.c
memory_map.o: memory_map.c
	gcc $(CFLAGS) memory_map.c
my_allocator.o : my_allocator.c
	gcc $(CFLAGS) my_allocator.c
memtest.o : memtest.c
	gcc $(CFLAGS) memtest.c
memtest: memtest.o ackerman.o my_allocator.o memory_map.o
	gcc -o $(TARGET) memtest.o ackerman.o my_allocator.o memory_map.o

.PHONY: clean
clean:
	rm -rf *.o *.out $(TARGET)

