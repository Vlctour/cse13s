#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "io.h"
#include "defines.h"

#define BYTE 8

// Initialize global variables.
// Used for statistics.
uint64_t bytes_read;
uint64_t bytes_written;

// Global buffer and pointer for write code.
static uint8_t w_buffer[BLOCK];
static int w_pointer;

// Gets the bit of a specific byte in a buffer.
// Returns true if bit is 1, else returns 0.
bool get_bit(uint8_t *buffer, int pointer) {
    uint8_t temp = buffer[pointer / BYTE];
    return (temp >> (pointer % BYTE)) & 0x1;
}

// Sets a bit inside a specific byte in a buffer.
void set_code(uint8_t bit) {
    if (bit) {
        w_buffer[w_pointer / BYTE] |= (1 << (w_pointer % BYTE));
    } else {
        w_buffer[w_pointer / BYTE] &= ~(1 << (w_pointer % BYTE));
    }

    return;
}

// Reads in bytes from an infile into a buffer.
// Returns the number of bytes read in.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    ssize_t count = 1, counter = 0;
    int mbytes = nbytes, index = 0;
    while (count > 0 && counter < nbytes) {
        count = read(infile, buf + index, mbytes);
        counter += count;
        index += count;
        bytes_read += count;
        mbytes -= count;
    }

    return counter;
}

// Writes a buffer to the outfile.
// Returns the number of bytes written.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    ssize_t count = 0, counter = 0;
    int mbytes = nbytes, index = 0;
    while (count > -1 && counter < nbytes) {
        count = write(outfile, buf + index, mbytes);
        counter += count;
        index += count;
        bytes_written += count;
        mbytes -= count;
    }

    return counter;
}

// Reads in bytes of data into a buffer.
// Stores a bit into an argument and increments pointer.
//
// Returns true if you can still read bits, false if no more bits
// can be read.
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer[BLOCK];
    static int pointer;
    static int readed;
    if (pointer == (BLOCK * 8) || pointer == 0) {
        readed = read_bytes(infile, buffer, BLOCK);
        pointer = 0;
    }

    if (readed == 0) {
        return false;
    }

    *bit = get_bit(buffer, pointer) ? 1 : 0;
    pointer += 1;

    return true;
}

// Writes an entire code into a buffer.
// When buffer is full, write the buffer out via flush code.
void write_code(int outfile, Code *c) {
    uint32_t code_s = code_size(c);
    uint32_t topp = 0;
    uint8_t bit;
    while (topp < code_s) {
        bit = code_get_bit(c, topp) ? 1 : 0;
        set_code(bit);
        w_pointer++;
        topp++;
        if (w_pointer == (8 * BLOCK)) {
            flush_codes(outfile);
        }
    }

    return;
}

// Writes a buffer full of bits to outfile.
// Make sure to the last byte to make sure it lines up nicely with a byte.
// Memset code gotten from eugene.
void flush_codes(int outfile) {
    int bytes_to_write = (w_pointer + 7) / 8;
    if (w_pointer > 0) {
        write_bytes(outfile, w_buffer, bytes_to_write);
        memset(w_buffer, 0, sizeof(w_buffer));
        w_pointer = 0;
    }

    return;
}
