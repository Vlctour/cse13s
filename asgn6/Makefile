CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g
OBJS = encode.o node.o pq.o code.o io.o stack.o huffman.o
OBJS2 = decode.o node.o pq.o code.o io.o stack.o huffman.o
OBJS3 = node.o test.o pq.o code.o io.o stack.o
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean valgrind scan-build debug

all: encode decode

encode: $(OBJS)
	$(CC) -o encode $(OBJS)

decode: $(OBJS2)
	$(CC) -o decode $(OBJS2)

test: $(OBJS3)
	$(CC) -o test $(OBJS3)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f test encode decode *.o

valgrind: clean decode
	valgrind $(VFLAGS) ./decode -i this -o that

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

