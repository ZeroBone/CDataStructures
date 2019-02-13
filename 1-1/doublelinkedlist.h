#ifndef INC_1_1_DOUBLELINKEDLIST_H
#define INC_1_1_DOUBLELINKEDLIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct doublelinkedlist_node_s {
    struct doublelinkedlist_node_s* previous;
    struct doublelinkedlist_node_s* next;
} doublelinkedlist_node_t;

typedef struct {
    const size_t _payloadSize;
    doublelinkedlist_node_t* firstNode;
    doublelinkedlist_node_t* lastNode;
} doublelinkedlist_t;

#define DLL_INIT(payloadSize) \
    { \
        ._payloadSize = payloadSize, \
        .firstNode = NULL, \
        .lastNode = NULL \
    };

#endif //INC_1_1_DOUBLELINKEDLIST_H