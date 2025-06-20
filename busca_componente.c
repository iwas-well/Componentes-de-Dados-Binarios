// programa feito para encontrar componentes de tamanho maior que um limiar em
// um grafo representado por um arquivo binario
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO
// trocar "void* elem" por "size_t idx" in queue_append()
#define MIN_COMPONENT_SIZE 80
#define MAX_OUT_NT 30
#define EPS 0.01

typedef struct {
    float* bin;
    size_t nx;
    size_t ny;
    size_t nt;
    float undef;
} data;

typedef struct queue {
    void* elem;
    struct queue* prev;
    struct queue* next;
} queue_t;

typedef struct {
    size_t x;
    size_t y;
} position;

// queue functions
void queue_append(queue_t** queue, void* elem);
void destroy_queue(queue_t** queue);
void* queue_pop(queue_t** queue);

// returns the index of the in.bin array equivalent to i and j position
size_t idx(data* in, size_t i, size_t j) { return j * in->nx + i; }

// sets states of all vertices
void set_states(int* estado, size_t nx, size_t ny, int state)
{
    for (size_t x = 0; x < nx; x++)
        for (size_t y = 0; y < ny; y++)
            estado[idx((data*)&((data) { .nx = nx }), x, y)] = state;
}

// return 1 if x and y are out of data bounds
int pos_out_of_bounds(data* in, size_t x, size_t y)
{
    return x >= in->nx || y >= in->ny;
}

// for each vertex, set the component of the connected subgraph it belongs to
size_t busca_componente(data* in, int* estado, size_t i, size_t j, data* out,
    float out_val)
{
    queue_t* queue = NULL;

    position* quad;
    quad = malloc(sizeof(position));
    quad->x = i;
    quad->y = j;
    // printf("appending %p x:%ld y:%ld\n", quad, quad->x, quad->y);
    queue_append(&queue, quad);

    int sign = (in->bin[idx(in, i, j)] > 0);
    estado[idx(in, i, j)] = 1;
    size_t comp_size = 1;

    // pop quad from queue until it empties
    while ((quad = queue_pop(&queue))) {
        //  each quad has 4 possible neighbors
        //  one in each cardinal direction
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                if (k == 0 && l == 0) // neighbor cant be equal to own quad
                    continue;

                size_t neigh_x = quad->x + k;
                size_t neigh_y = quad->y + l;

                // if quad position is out of bounds, continue
                if (pos_out_of_bounds(in, neigh_x, neigh_y))
                    continue;

                // treat only if quad wasnt visited
                if (estado[idx(in, neigh_x, neigh_y)] == 0) {
                    float w = in->bin[idx(in, neigh_x, neigh_y)];
                    // if sign of neigh quad is different from quad sign,
                    // dont treat as a neighbor
                    if ((w > 0) == sign) {
                        position* pos;
                        pos = malloc(sizeof(position));
                        pos->x = neigh_x;
                        pos->y = neigh_y;
                        // printf("appending %p x:%ld y:%ld\n", pos, pos->x, pos->y);
                        queue_append(&queue, pos);

                        estado[idx(in, neigh_x, neigh_y)] = 1;
                        comp_size++;
                        out->bin[idx(in, neigh_x, neigh_y)] = out_val;
                    }
                }
            }
        }
        free(quad);
    }

    destroy_queue(&queue);
    return comp_size;
}

int write_data(FILE* out, data* dado)
{
    size_t dims = dado->nx * dado->ny * dado->nt;

    printf("dims: %ld\n", dims);
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

    fread(dado->bin, sizeof(float), dado->nx * dado->ny, in);
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

int main(int argc, char* argv[])
{
    size_t nx, ny, nt;

    if (argc != 6) {
        fprintf(stderr, "Uso: %s <input name> <nx> <ny> <nt> <output name>\n",
            argv[0]);
        return 0;
    }

    char* input_name = argv[1];
    nx = atoi(argv[2]);
    ny = atoi(argv[3]);
    nt = atoi(argv[4]);
    char* output_name = argv[5];

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

    float undef = 777.7;
    data* dado = create_data(nx, ny, nt, undef);
    data* output = create_data(nx, ny, 1, undef);

    // for (size_t x = 0; x < nx; x++) {
    //     for (size_t y = 0; y < ny; y++) {
    //         size_t i = idx(output, x, y);
    //         printf("bin[%ld]\n", i);
    //         if (y < ny / 3) {
    //             output->bin[i] = 1.0;
    //         } else if (y < (2 * (ny / 3)))
    //             output->bin[i] = -1.0;
    //         else
    //             output->bin[i] = 1.0;
    //     }
    // }
    // printf("output nx:%ld ny:%ld nt:%d\n", output->nx, output->ny, 1);
    // write_data(output_file, output);

    read_data(input_file, dado);
    int estado[nx * ny];

    // initiallize state of every vertex
    set_states(estado, dado->nx, dado->ny, 0);

    // set all undef vertices state as 1
    for (size_t x = 0; x < nx; x++)
        for (size_t y = 0; y < ny; y++)
            if (fabs(dado->bin[idx(dado, x, y)] - dado->undef) <= EPS)
                estado[idx(dado, x, y)] = 1;

    size_t comp_size;
    size_t output_nt = 0;
    for (size_t x = 0; x < nx; x++) {
        for (size_t y = 0; y < ny; y++) {
            if (estado[idx(dado, x, y)] != 0)
                continue;

            fill_data(output, undef);
            comp_size = busca_componente(dado, estado, x, y, output, 1);

            if (comp_size >= MIN_COMPONENT_SIZE) {
                write_data(output_file, output);
                output_nt++;
            }

            if (output_nt > MAX_OUT_NT) {
                fclose(output_file);
                fprintf(stderr, "Erro: numero de tempos do arquivo de saida ultrapassa "
                                "numero maximo\n");
                exit(1);
            }
        }
    }

    printf("output_nt %ld\n", output_nt);
    fclose(input_file);
    fclose(output_file);
}

// appends a new element to the queue
void queue_append(queue_t** queue, void* elem)
{
    if (!queue) {
        fprintf(stderr, "Erro queue_append: fila nao existe\n");
        exit(EXIT_FAILURE);
    }

    if (!elem) {
        fprintf(stderr, "Erro queue_append: elemento nao existe\n");
        exit(EXIT_FAILURE);
    }

    queue_t* node;
    if (!(node = malloc(sizeof(queue_t)))) {
        fprintf(stderr, "Erro queue_append: nao foi possivel alocar nodo\n");
        exit(EXIT_FAILURE);
    }

    if (!(*queue)) {
        // caso a lista esteja vazia, adiciona o elemento
        node->elem = elem;
        node->next = node->prev = node;
        (*queue) = node;
        return;
    }

    queue_t* prev_node = (*queue)->prev;

    // insere elemento no final da fila
    node->elem = elem;
    node->prev = prev_node;
    node->next = (*queue);

    (*queue)->prev = node;
    prev_node->next = node;

    // printf("QUEUE\n");
    // queue_t* aux = (*queue);
    // do {
    //     printf("%p (%ld, %ld)->", aux->elem, ((position*)aux->elem)->x, ((position*)aux->elem)->y);
    //     aux = aux->next;
    // } while (aux != (*queue));
    // printf("\n");
}

// pops firs element of the queue and returns a pointer to its content
void* queue_pop(queue_t** queue)
{
    void* elem;

    if (*queue == NULL)
        return NULL;

    elem = (*queue)->elem;
    if ((*queue)->next == *queue) {
        free(*queue);
        *queue = NULL;
    } else {
        (*queue)->next->prev = (*queue)->prev;
        (*queue)->prev->next = (*queue)->next;
        queue_t* aux = *queue;
        *queue = (*queue)->next;
        free(aux);
    }

    return elem;
}

// destroys queue and sets its value to NULL
void destroy_queue(queue_t** queue)
{
    if (*queue == NULL)
        return;

    queue_t *node, *aux;
    node = *queue;

    while (node->next != node) {
        node->next->prev = node->prev;
        node->prev->next = node->next;

        aux = node->next;
        free(node);
        node = aux;
    }

    free(node);
    *queue = NULL;
}
