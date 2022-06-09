CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)
OBJS = randstate.o numtheory.o rsa.o
KEYOBJ = $(OBJS) keygen.o
ENCOBJ = $(OBJS) encrypt.o
DECOBJ = $(OBJS) decrypt.o
TESTOBJ = $(OBJS) test.o
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean valgrind scan-build debug

all: keygen encrypt decrypt

test: $(TESTOBJ)
	$(CC) -o test $(TESTOBJ) $(LFLAGS)

keygen: $(KEYOBJ)
	$(CC) -o keygen $(KEYOBJ) $(LFLAGS)

encrypt: $(ENCOBJ)
	$(CC) -o encrypt $(ENCOBJ) $(LFLAGS)

decrypt: $(DECOBJ)
	$(CC) -o decrypt $(DECOBJ) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f test keygen encrypt decrypt rsa.priv rsa.pub *.o

valgrind: clean keygen
	valgrind $(VFLAGS) ./keygen

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

