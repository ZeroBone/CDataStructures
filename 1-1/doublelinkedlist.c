#include "doublelinkedlist.h"

inline void* dll_addNodeToBeginning(doublelinkedlist_t* list) {

    doublelinkedlist_node_t* node =
        (doublelinkedlist_node_t*)malloc(sizeof(doublelinkedlist_node_t) + list->_payloadSize);

    if (node == NULL) {
        return NULL;
    }

    node->previous = NULL;
    node->next = list->firstNode;

    if (list->firstNode == NULL) {
        // empty linked list
        // the first node is the last one
        list->lastNode = node;
    }
    else {
        // there is already at least one element
        list->firstNode->previous = node;
    }

    list->firstNode = node;

    return ((char*)node) + list->_payloadSize;

}

inline void* dll_insertNodeAfter(doublelinkedlist_t* list, doublelinkedlist_node_t* afterNode) {

    doublelinkedlist_node_t* node =
        (doublelinkedlist_node_t*)malloc(sizeof(doublelinkedlist_node_t) + list->_payloadSize);

    if (node == NULL) {
        return NULL;
    }

    node->previous = afterNode;

    if (afterNode->next == NULL) {
        // we are adding to the end of the linked list
        // node->next should be NULL
        list->lastNode = node;
    }
    else {
        // we are adding this node between some nodes
        afterNode->previous = node;
    }

    node->next = afterNode->next;

    afterNode->next = node;

    return ((char*)node) + list->_payloadSize;

}

inline void* dll_insertNodeBefore(doublelinkedlist_t* list, doublelinkedlist_node_t* beforeNode) {

    doublelinkedlist_node_t* node =
        (doublelinkedlist_node_t*)malloc(sizeof(doublelinkedlist_node_t) + list->_payloadSize);

    if (node == NULL) {
        return NULL;
    }



}