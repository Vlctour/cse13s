CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = $(shell pkg-config --libs ncurses)
OBJS = life.o universe.o
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean valgrind scan-build

all: life

life: $(OBJS)
	$(CC) $(LFLAGS) -o life $(OBJS)

test: test.o universe.o
	$(CC) -o test test.o universe.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f life test *.o

valgrind: clean life
	valgrind $(VFLAGS) ./life -s -i ./lists/gospergun.txt

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

