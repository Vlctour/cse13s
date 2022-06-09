# ASGN2 - Numerical Integration

## Short Description

This assignment numerically integrates certain functions given from the professor.

## Building

To build please use the makefile and type:
```
$ make
```
Alternatively, you can type:
```
$ make integrate
```
on the command line to specify that you want to make integrate.

Additionally if you want to build the test.c file (which I used for testing my math library) type:
```
$ make test
``` 

## Running

Running ./integrate requires some knowledge to what arguments it takes in. typing in:
```
./integrate -H

```
will give you all the options and parameters that you need to specify to run the executable
properly.

An example of what ./integrate can compute is:
```
./integrate -a -p 0 -q 1 -n 50
```

## Cleaning

To clean, you can type:
```
$ make clean
```
and the Makefile will remove all generated files.

## Known Errors

You will get unexpected errors if you don't specify the lower bound (p) and upper bound (q) of 
integrate. The lower and upper bound should always be initialized.

If you decide to integrate with the -g or -h option, you'll get an error if the bound you give it
includes 0, since that would eventually mean that you're dividing by 0.

Try not to make my math library compute negative square roots, logarithms of zero and negative
numbers. I did not build them to handle complex/imaginary numbers.
