# ASGN6 - Huffman Coding

## Short Description

The purpose of these programs is to be able to create a lossless compression algorithm. Uses for 
these algorithms can be useful for transferring large files in smaller sizes while also allowing 
us to not lose any data.

## Building

To build please use the makefile and type "make". This will build the 2 necessary files,
namely: encode and decode.
Alternatively, you can type "make encode" / "make decode" to build the files
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
$ ./encode -h
or
$ ./decode -h
```
will give you all the options and parameters that you need to specify to run the executable
properly.

## Cleaning

To clean, you can type "make clean" and the Makefile will remove all generated files.

## Known Errors

The user can only decode files that were encoded by either my own encoder or the resource encoder.

When you run scan-build there will be an error that shows up in my decode on line 134. It says
that the access to the field 'left' will result in a dereference of a null pointer. This will
never be the case when running the file since I have checks for a null pointer in my node, pq, 
and rebuild tree. If the user also tried feeding a non encoded file to my decoder, I have a check 
that checks to see if the header is the right name. So there should be no chances of my temp node 
being a null pointer at the executable time.
