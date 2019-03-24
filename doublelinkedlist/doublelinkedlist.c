#include "doublelinkedlist.h"

#include <stdio.h>

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

    return ((char*)node) + sizeof(doublelinkedlist_node_t);

}

inline void* dll_addNodeToEnd(doublelinkedlist_t* list) {

    doublelinkedlist_node_t* node =
        (doublelinkedlist_node_t*)malloc(sizeof(doublelinkedlist_node_t) + list->_payloadSize);

    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->previous = list->lastNode;

    if (list->lastNode == NULL) {
        // empty linked list
        // the first node is the last one
        list->firstNode = node;
    }
    else {
        // there is already at least one element
        list->lastNode->next = node;
    }

    list->lastNode = node;

    return ((char*)node) + sizeof(doublelinkedlist_node_t);

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
        afterNode->next->previous = node;
    }

    node->next = afterNode->next;

    afterNode->next = node;

    return ((char*)node) + sizeof(doublelinkedlist_node_t);

}

inline void* dll_insertNodeBefore(doublelinkedlist_t* list, doublelinkedlist_node_t* beforeNode) {

    doublelinkedlist_node_t* node =
        (doublelinkedlist_node_t*)malloc(sizeof(doublelinkedlist_node_t) + list->_payloadSize);

    if (node == NULL) {
        return NULL;
    }

    node->next = beforeNode;

    if (beforeNode->previous == NULL) {
        // we are adding to the start of the linked list
        list->firstNode = node;
    }
    else {
        beforeNode->previous->next = node;
    }

    node->previous = beforeNode->previous;

    beforeNode->previous = node;

    return ((char*)node) + sizeof(doublelinkedlist_node_t);

}

inline void dll_deleteFirstNode(doublelinkedlist_t* list) {

    if (list->firstNode == NULL) {
        return;
    }

    // if (list->firstNode->next == NULL) {
    if (list->firstNode == list->lastNode) {
        // we are deleting the last and only node

        free(list->firstNode);

        list->firstNode = NULL;
        list->lastNode = NULL;

        return;

    }

    list->firstNode->next->previous = NULL;

    doublelinkedlist_node_t* oldFirstNode = list->firstNode;
    list->firstNode = list->firstNode->next;

    free(oldFirstNode);

}

inline void dll_deleteLastNode(doublelinkedlist_t* list) {

    if (list->firstNode == NULL) {
        return;
    }

    // if (list->firstNode->next == NULL) {
    if (list->firstNode == list->lastNode) {
        // we are deleting the last and only node

        free(list->lastNode); // doesn't matter first node or last node

        list->firstNode = NULL;
        list->lastNode = NULL;

        return;

    }

    list->lastNode->previous->next = NULL;

    doublelinkedlist_node_t* oldLastNode = list->lastNode;
    list->lastNode = list->lastNode->previous;

    free(oldLastNode);

}

inline void dll_deleteNode(doublelinkedlist_t* list, doublelinkedlist_node_t* node) {

    if (node->previous == NULL) {
        // this is the first node

        dll_deleteFirstNode(list);
        return;

    }

    if (node->next == NULL) {
        // last node

        dll_deleteLastNode(list);
        return;

    }

    node->previous->next = node->next;
    node->next->previous = node->previous;

    free(node);

}

inline void* dll_nodePayload(doublelinkedlist_t* list, doublelinkedlist_node_t* node) {

    return ((char*)node) + sizeof(doublelinkedlist_node_t);

}