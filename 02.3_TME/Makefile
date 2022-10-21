#CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS =-g -Wno-unused-parameter
CC = gcc

PROGRAMS = tests_ecosys tests_ecosys2 ecosys

.PHONY:	all clean re

re: clean all

all: $(PROGRAMS)

tests_ecosys: ecosys.o main_tests.o
	$(CC) -o $@ $(CFLAGS) $^

tests_ecosys2: ecosys.o main_tests2.o
	$(CC) -o $@ $(CFLAGS) $^

ecosys: ecosys.o main_ecosys.o
	$(CC) -o $@ $(CFLAGS) $^

ecosys.o: ecosys.c
	$(CC) $(CFLAGS) -c ecosys.c

main_tests.o: main_tests.c
	$(CC) $(CFLAGS) -c main_tests.c

main_tests2.o: main_tests2.c
	$(CC) $(CFLAGS) -c main_tests2.c

main_ecosys.o: main_ecosys.c 
	$(CC) $(CFLAGS) -c main_ecosys.c

clean:
	rm -f *.o *~ $(PROGRAMS)
