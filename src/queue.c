#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void queue_append(queue_t** queue, size_t elem)
{
    if (!queue) {
        fprintf(stderr, "Erro queue_append: fila nao existe\n");
        exit(EXIT_FAILURE);
    }

    if (elem == SIZE_T_ERROR) {
        fprintf(stderr, "Erro queue_append: elemento invalido\n");
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

size_t queue_pop(queue_t** queue)
{
    // void* elem;
    size_t elem;

    if (*queue == NULL)
        return SIZE_T_ERROR;

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
