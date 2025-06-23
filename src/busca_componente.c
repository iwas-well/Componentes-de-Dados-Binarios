// Programa feito para encontrar componentes de tamanho maior que um limiar em
// um grafo representado por um arquivo binario.
// Componentes são tidos como quadriculas adjacentes de mesmo sinal.
// Atualmente, 0 é considerado valor negativo.
#include "data.h"
#include "queue.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// TODO
// trocar "void* elem" por "size_t idx" in queue_append()
#define MIN_COMPONENT_SIZE 80
#define MAX_COMP_NUM 6
#define EPS 0.01

typedef struct {
    size_t x;
    size_t y;
} position;

// for each vertex, set the component of the connected subgraph it belongs to
// and returns the size of the component. Only considers vertices with state 0.
size_t busca_componente(data* in, int* estado, size_t i, size_t j, data* out,
    float out_val);

int main(int argc, char* argv[])
{
    size_t nx, ny, nt;
    float undef;

    if (argc != 7) {
        fprintf(stderr, "Uso: %s <input name> <nx> <ny> <nt> <undef> <output name>\n",
            argv[0]);
        return 0;
    }
    char* input_name = argv[1];
    nx = atoi(argv[2]);
    ny = atoi(argv[3]);
    nt = atoi(argv[4]);
    undef = atof(argv[5]);
    char* output_name = argv[6];

    FILE* input_file;
    FILE* output_file;

    if (!(input_file = fopen(input_name, "r"))) {
        fprintf(stderr, "Erro ao abrir arquivo de entrada\n");
        exit(1);
    }
    if (!(output_file = fopen(output_name, "wb"))) {
        fprintf(stderr, "Erro ao abrir arquivo de saida\n");
        exit(1);
    }

    data* dado = create_data(nx, ny, nt, undef);
    data* output = create_data(nx, ny, 1, undef);

    read_data(input_file, dado);

    data aux;
    int estado[nx * ny];
    size_t comp_size;
    size_t comp_num;
    size_t max_comp_num = 0;
    size_t t_max_comp_num = 0;
    for (size_t t = 0; t < nt; t++) {

        aux.bin = dado->bin + nx * ny * t;
        aux.nx = dado->nx;
        aux.ny = dado->ny;
        aux.undef = dado->undef;

        // set vertices state as 0 (undef vertices are set as 1)
        for (size_t x = 0; x < nx; x++)
            for (size_t y = 0; y < ny; y++) {
                estado[idx(&aux, x, y)] = 0;
                if (fabs(aux.bin[idx(&aux, x, y)] - aux.undef) <= EPS)
                    estado[idx(&aux, x, y)] = 1;
            }

        comp_num = 0;
        for (size_t x = 0; x < nx; x++) {
            for (size_t y = 0; y < ny; y++) {
                if (estado[idx(&aux, x, y)] != 0)
                    continue;

                // fill_data(output, undef);
                fill_data(output, 0);
                comp_size = busca_componente(&aux, estado, x, y, output, 1);

                if (comp_size >= MIN_COMPONENT_SIZE) {
                    fseek(output_file, sizeof(float) * ((nx * ny * t) + (12 * nx * ny * comp_num)), SEEK_SET);
                    write_data(output_file, output);
                    comp_num++;
                    printf("t: %ld comp_size: %ld\n", t, comp_size);
                }

                if (comp_num > MAX_COMP_NUM) {
                    fclose(output_file);
                    fprintf(stderr, "Erro: numero de componentes do arquivo de saida ultrapassa "
                                    "numero maximo\n");
                    exit(1);
                }
                if (comp_num >= max_comp_num) {
                    max_comp_num = comp_num;
                    t_max_comp_num = t;
                }
            }
        }
    }

    // escreve no ultimo byte para completar 12 meses do ultimo "ano"
    if (t_max_comp_num != 12) {
        fseek(output_file, sizeof(float) * (nx * ny * 12) * max_comp_num, SEEK_SET);
        float zero = 0;
        fwrite(&zero, sizeof(float), 1, output_file);
    }

    printf("output nt %ld\n", max_comp_num * 12);
    fclose(input_file);
    fclose(output_file);
    return 0;
}

size_t busca_componente(data* in, int* estado, size_t i, size_t j, data* out,
    float out_val)
{
    queue_t* queue = NULL;

    position* cell;
    cell = malloc(sizeof(position));
    cell->x = i;
    cell->y = j;
    queue_append(&queue, cell);

    int sign = (in->bin[idx(in, i, j)] > 0);
    estado[idx(in, i, j)] = 1;
    size_t comp_size = 1;

    // pop cell from queue until it empties
    while ((cell = queue_pop(&queue))) {
        //  each cell has 4 possible neighbors
        //  one in each cardinal direction
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                if (k == 0 && l == 0) // neighbor cant be equal to own cell
                    continue;

                size_t neigh_x = cell->x + k;
                size_t neigh_y = cell->y + l;

                // if cell position is out of bounds, continue
                if (pos_out_of_bounds(in, neigh_x, neigh_y))
                    continue;

                // treat only if cell wasnt visited
                if (estado[idx(in, neigh_x, neigh_y)] == 0) {
                    float w = in->bin[idx(in, neigh_x, neigh_y)];
                    // if sign of neigh cell is different from root cell sign,
                    // dont treat as a neighbor
                    if ((w > 0) == sign) {
                        position* pos;
                        pos = malloc(sizeof(position));
                        pos->x = neigh_x;
                        pos->y = neigh_y;
                        queue_append(&queue, pos);

                        // set state of processed neighbor
                        estado[idx(in, neigh_x, neigh_y)] = 1;
                        out->bin[idx(in, neigh_x, neigh_y)] = out_val;
                        comp_size++;
                    }
                }
            }
        }
        free(cell);
    }

    destroy_queue(&queue);
    return comp_size;
}
