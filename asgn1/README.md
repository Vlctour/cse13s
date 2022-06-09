# ASGN1 - Getting Acquainted with Unix and C

This assignment uses a bash script named plot.sh to create 3 distinct graphs pertaining to the 
collatz conjecture.

## Building

To build manually, type:
```
$ clang -Wall -Wextra -Werror -Wpedantic -o collatz collatz.c
```
Alternatively, you can type:
```
$ make
```
on the command line. 

## Running

Running 
```
$ ./plot.sh
```
will produce 3 graphs pertaining to collatz's conjecture.
Additionally, if you ran
```
$ make DESIGN.pdf
```
it will produce a DESIGN.pdf with the provided DESIGN.tex file and also,
```
$ make WRITEUP.pdf
```
will do the same thing but for the WRITEUP.pdf.
Note: in order to produce the WRITEUP.pdf file, you need to have ran ./plot.sh otherwise you'll
run into an error where it will be missing the necessary collatz graphs.

## Cleaning

To clean, you can type:
```
$ make clean
```
and the Makefile will remove all generated files. Except for DESIGN.pdf and WRITEUP.pdf.
