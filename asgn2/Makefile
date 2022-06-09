CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LDFLAGS = -lm
OBJS = test.o mathlib.o
OBJS2 = integrate.o functions.o mathlib.o

.PHONY: all clean

all: integrate

test: $(OBJS)
	$(CC) $(LDFLAGS) -o test $(OBJS)

integrate: $(OBJS2)
	$(CC) $(LDFLAGS) -o integrate $(OBJS2)

mathlib.o: mathlib.c
	$(CC) $(CFLAGS) -c mathlib.c

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c 

integrate.o: integrate.c
	$(CC) $(CFLAGS) -c integrate.c

clean:
	rm -f mathlib mathlib.o test test.o integrate integrate.o functions functions.o

format:
	clang-format -i -style=file *.[ch]
