#ifndef INC_1_1_DOUBLELINKEDLIST_H
#define INC_1_1_DOUBLELINKEDLIST_H

/**
 * @copyright Alexander Mayorov
 */

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

#define DLL_EMPTY(dll) ((dll).firstNode == NULL)

#define DLL_NOT_EMPTY(dll) ((dll).firstNode != NULL)

#define DDL_PAYLOAD(list, node) (((char*)(node)) + (list)->_payloadSize)

inline void* dll_addNodeToBeginning(doublelinkedlist_t* list);
inline void* dll_insertNodeAfter(doublelinkedlist_t* list, doublelinkedlist_node_t* afterNode);
inline void* dll_insertNodeBefore(doublelinkedlist_t* list, doublelinkedlist_node_t* beforeNode);
inline void dll_deleteFirstNode(doublelinkedlist_t* list);
inline void* dll_nodePayload(doublelinkedlist_t* list, doublelinkedlist_node_t* node);

#endif //INC_1_1_DOUBLELINKEDLIST_H