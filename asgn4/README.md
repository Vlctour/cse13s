# ASGN4 - The Game of Life

## Short Description

This program allows the user to insert a file to play the game of life. This file should only 
contain two positive numbers per line, namely rows and columns. The program will take the input 
and set the playing grid to be whatever was specified. The remaining rows and column pairs will be 
used to populate the grid and act as the starting generation.

## Building

To build please use the makefile and type:
```
$ make
```
Alternatively, you can type:
```
$ make life
```
on the command line to specify that you want to make life.

Additionally if you want to build the test.c file (which I used for testing) type:
```
$ make test
``` 
Note: I didn't note down what the code inside test.c does since it's all random stuff.

## Running

Running .life requires some knowledge to what arguments it takes in. typing in:
```
./life -H

```
will give you all the options and parameters that you need to specify to run the executable
properly.

An example of what ./life takes is:
```
./life -n 10 -i lines.txt
```
What that above command will do is set the number of generations to 10 (so it'll only go through 
10 generations) and will read from the input file lines.txt. (If it exists in the current 
directory)

## Cleaning

To clean, you can type:
```
$ make clean
```
and the Makefile will remove all generated files.

## Known Errors

The input file should contain only positive intergers. Otherwise an error message will pop up 
telling you that you have a malformed input. 

I also noticed that a segfault error can occur when you specify ./life to read from a file that 
doesn't exists. To accommodate for this, I printed out an error message to the user that the 
program failed to read the file.

If the file we open ever contains a negative number, or strings I'll return a malformed input.
Though there is an error that I couldn't fix which was if the file contained irregular formatting.
You could read my EXTRA\_TESTS.pdf for more info.

There's an error that I know of that I cannot fix. This error occurs when the user doesn't 
input any files to read from, resulting in the program running forever.

I've also provided additional testings for any errors or memory leaks. To test for errors
you can type:
```
$ make scan-build
```
This will find any additional errors in the code. To test for memory leaks, you can run:
```
$ make valgrind
```
to make sure that I've freed the used memory on the heap. Due note that there is memory leakage
under the category of reachable memory. This is caused by ncurses, which I was not able to fix.
Also a thing about valgrind, you should also have a directory called lists that contains a bunch 
of .txt files. My valgrind does a test based off of the gospergun.txt file.
