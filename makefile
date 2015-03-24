# makefile

TARGET=memtest
CFLAGS=-c -g
LFLAGS=-lm

all: memtest
test: unit_tests

ackerman.o: ackerman.c ackerman.h
	gcc $(CFLAGS) ackerman.c
memory_map.o: memory_map.c memory_map.h
	gcc $(CFLAGS) memory_map.c
my_allocator.o : my_allocator.c my_allocator.h memory_map.h
	gcc $(CFLAGS) my_allocator.c
memtest.o : memtest.c
	gcc $(CFLAGS) memtest.c
memtest: memtest.o ackerman.o my_allocator.o memory_map.o
	gcc -o $(TARGET) $(LFLAGS) memtest.o ackerman.o my_allocator.o memory_map.o
unit_tests.o: my_allocator.h memory_map.h
	gcc $(CFLAGS) unit_tests.c
unit_tests: unit_tests.o my_allocator.o memory_map.o
	gcc -o unit_tests unit_tests.o my_allocator.o memory_map.o

.PHONY: clean
clean:
	rm -rf *.o *.out $(TARGET)

