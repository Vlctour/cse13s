# ASGN7 - Author Identification

## Short Description

Purpose of this program is to detect how closely related some anonymous author is compared to 
a database of known authors. It's like a type of plagiarism checker that uses what is considered 
simple machine learning algorithm known as k-Nearest Neighbors. It utilizes a SPECK Cipher for 
the hashing for our hash table and also a priority queue (using heap insertion) to give us the 
top k likely authors of a given file and data base.
## Building

To build please use the makefile and type "make". This will build the identify executable.
Alternatively, you can type "make identify" to build the files
seperately using the make file.

Additionally if you want to build the test.c file (which I used for testing) type:
```
$ make test
``` 
Note: I didn't note down what the code inside test.c does since it's all random stuff and it's
been through a lot of testing. It's missing most of what I tested in the recent git push but you 
can look back on my previous git commits to look at all the changes if you wanted.

## Running

Running the programs will requires some knowledge to what arguments it takes in. typing in:
```
$ ./identify -h
```
will give you all the options and parameters that you need to specify to run the executable
properly.
An example program that you could run could look like (assuming that you have the necessary
texts directory):
```
$ ./identify -k 10 < texts/william-shakespeare.txt
```

## Cleaning

To clean, you can type "make clean" and the Makefile will remove all generated files.

## Known Errors

Feeding a negative number for specifying how many items we want to dequeue will default to 
dequeuing however many items there are in the data base.

Feeding in invalid noise files, or data bases will result in an error. I provided a helpful 
message to denote that.

Feeding in all three metrics will only compute the last specified metric.
