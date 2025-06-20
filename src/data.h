#ifndef BUSCA_COMP_DATA
#define BUSCA_COMP_DATA
#include <stddef.h>
#include <stdio.h>

typedef struct {
    float* bin;
    size_t nx;
    size_t ny;
    size_t nt;
    float undef;
} data;

// return 1 if x and y are out of data bounds
int pos_out_of_bounds(data* in, size_t x, size_t y);

// returns the index of the in.bin array equivalent to i and j position
size_t idx(data* in, size_t i, size_t j);

// writes data to file
int write_data(FILE* out, data* dado);

// reads file into data
int read_data(FILE* in, data* dado);

// creates and return a data variable
data* create_data(size_t nx, size_t ny, size_t nt, float undef);

// files data with given value
void fill_data(data* in, float val);

#endif
