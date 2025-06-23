#ifndef BUSCA_COMP_QUEUE
#define BUSCA_COMP_QUEUE

#include <stddef.h>

// SIZE_T_ERROR is equivalent to SIZE_MAX
#define SIZE_T_ERROR ((size_t)-1)

typedef struct queue {
    size_t elem;
    struct queue* prev;
    struct queue* next;
} queue_t;

// appends a new element to the queue
void queue_append(queue_t** queue, size_t val);

// pops firs element of the queue and returns a pointer to its content
size_t queue_pop(queue_t** queue);

// destroys queue and sets its value to NULL
void destroy_queue(queue_t** queue);

#endif
