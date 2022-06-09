CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g -Ofast
LFLAGS = -lm
OBJS = identify.o node.o speck.o bv.o bf.o parser.o text.o pq.o ht.o
OBJS3 = test.o node.o ht.o speck.o bv.o bf.o parser.o text.o pq.o
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean valgrind scan-build debug

all: identify

identify: $(OBJS)
	$(CC) $(LFLAGS) -o identify $(OBJS)

test: $(OBJS3)
	$(CC) $(LFLAGS) -o test $(OBJS3)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f identify test *.o

valgrind: clean identify
	valgrind $(VFLAGS) ./identify -d small.db < texts/william-shakespeare.txt

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

