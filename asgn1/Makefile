CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: collatz

collatz: collatz.o
	$(CC) -o collatz collatz.o

collatz.o: collatz.c
	$(CC) $(CFLAGS) -c collatz.c

DESIGN.pdf: DESIGN.tex
	latexmk -pdf DESIGN.tex

WRITEUP.pdf: WRITEUP.tex
	latexmk -pdf WRITEUP.tex

clean:
	rm -f collatz collatz.o DESIGN.fdb_latexmk DESIGN.log DESIGN.aux DESIGN.fls collatz.pdf collatz_hist.pdf collatz_max.pdf WRITEUP.fdb_latexmk WRITEUP.log WRITEUP.aux WRITEUP.fls

format:
	clang-format -i -style=file *.{c,h}
