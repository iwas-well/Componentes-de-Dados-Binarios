#include "data.h"
#include <stdio.h>
#include <stdlib.h>

size_t idx(data* in, size_t i, size_t j) { return j * in->nx + i; }

int pos_out_of_bounds(data* in, size_t x, size_t y)
{
    return x >= in->nx || y >= in->ny;
}

int write_data(FILE* out, data* dado)
{
    size_t dims = dado->nx * dado->ny * dado->nt;

    // printf("dims: %ld\n", dims);
    if (fwrite(dado->bin, sizeof(float), dims, out) < dims) {
        fprintf(stderr, "Erro write_data: nao foi possivel escrever binario\n");
        return 1;
    }

    return 0;
}

int read_data(FILE* in, data* dado)
{
    if (!(dado->bin = malloc(sizeof(float) * dado->nx * dado->ny * dado->nt))) {
        fprintf(stderr,
            "Erro read_data: nao foi possivel alocar matriz de dados\n");
        exit(1);
    }

    fread(dado->bin, sizeof(float), dado->nx * dado->ny * dado->nt, in);
    return 0;
}

data* create_data(size_t nx, size_t ny, size_t nt, float undef)
{
    data* dado;
    if (!(dado = malloc(sizeof(data)))) {
        fprintf(stderr,
            "Erro create_data: nao foi possivel alocar estrutura de dados\n");
        exit(1);
    }

    if (!(dado->bin = malloc(sizeof(float) * nx * ny * nt))) {
        fprintf(stderr,
            "Erro create_data: nao foi possivel alocar matriz de dados\n");
        exit(1);
    }

    dado->nx = nx;
    dado->ny = ny;
    dado->nt = nt;
    dado->undef = undef;
    return dado;
}

void fill_data(data* in, float val)
{
    for (size_t x = 0; x < in->nx; x++)
        for (size_t y = 0; y < in->ny; y++)
            in->bin[idx(in, x, y)] = val;
}
