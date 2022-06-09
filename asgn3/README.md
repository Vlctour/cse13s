# ASGN3 - Sorting: Putting your affairs in order

## Short Description

This program allows the user to use and collect data on 4 sorting algorithms. These sorting 
algorithms are called insertion, heap, quick, and batcher.

## Building

To build please use the makefile and type:
```
$ make
```
Alternatively, you can type:
```
$ make sorting
```
on the command line to specify that you want to make sorting.

Additionally if you want to build the test.c file (which I used for testing my math library) type:
```
$ make test
``` 

## Running

Running ./sorting requires some knowledge to what arguments it takes in. typing in:
```
./sorting -H

```
will give you all the options and parameters that you need to specify to run the executable
properly.

An example of what ./sorting can compute is:
```
./sorting -a -p 0
```
What that above command will do is use all 4 sorting algorithms and give you the amount of moves 
and comparisons that each algorithm used.

## Cleaning

To clean, you can type:
```
$ make clean
```
and the Makefile will remove all generated files.

## Known Errors

Don't insert a negative value for n, it will result in a segmentation fault.

I've also provided additional testings for any errors or memory leaks. To test for errors
you can type:
```
$ make scan-build
```
This will find any additional errors in the code. To test for memory leaks, you can run:
```
$ make valgrind
```
to make sure that I've freed the used memory on the heap.
