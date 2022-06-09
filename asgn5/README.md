# ASGN4 - Public Key Cryptography

## Short Description

This purpose of these programs are to create a secure way to allow messages to be passed around
so that only specific people can read the messages. The keygen program will be in charge of 
generating keys for the user, one public, the other being private. The encrypt program will 
encrypt some file that we specify using RSA encryption, The decrypt program will be in charge of 
decrypting the file using RSA encryption.

## Building

To build please use the makefile and type "make". This will build the 3 necessary files,
namely: keygen, encrypt, and decrypt.
Alternatively, you can type "make keygen" / "make decrypt" / "make encrypt" to build the files
seperately using the make file.

Additionally if you want to build the test.c file (which I used for testing) type:
```
$ make test
``` 
Note: I didn't note down what the code inside test.c does since it's all random stuff and it's
been through a lot of testing.

## Running

Running the programs will requires some knowledge to what arguments it takes in. typing in:
```
$ ./keygen -h
or
$ ./encrypt -h
or
$ ./decrypt -h
```
will give you all the options and parameters that you need to specify to run the executable
properly.

## Cleaning

To clean, you can type "make clean" and the Makefile will remove all generated files.

## Known Errors

The user should always run ./keygen before running ./encrypt or ./decrypt. There will be many 
bugs that you'll run into if you don't. For some reason, you can also leak memory if you fail to 
do so.

You shouldn't run ./encrypt or ./decrypt with no arguments, this will result in a non terminating 
program.
