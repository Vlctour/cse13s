CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LDFLAGS = -lm
OBJS = sorting.o insert.o heap.o quick.o batcher.o stats.o
OBJS2 = test.o insert.o stats.o
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean valgrind scan-build

all: sorting

sorting: $(OBJS)
	$(CC) -o sorting $(OBJS)

test: $(OBJS2)
	$(CC) -o test $(OBJS2)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

DESIGN.pdf: DESIGN.tex
	latexmk -pdf DESIGN.tex

clean:
	rm -f test.o test sorting.o sorting insert.o heap.o quick.o batcher.o stats.o

valgrind: clean all
	valgrind $(VFLAGS) ./sorting -a -p 0 -n 100

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]
