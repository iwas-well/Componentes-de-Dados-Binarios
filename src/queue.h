#ifndef BUSCA_COMP_QUEUE
#define BUSCA_COMP_QUEUE

typedef struct queue {
    void* elem;
    struct queue* prev;
    struct queue* next;
} queue_t;

// appends a new element to the queue
void queue_append(queue_t** queue, void* elem);

// pops firs element of the queue and returns a pointer to its content
void* queue_pop(queue_t** queue);

// destroys queue and sets its value to NULL
void destroy_queue(queue_t** queue);

#endif
