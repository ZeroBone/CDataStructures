#ifndef INC_1_2_QUEUE_H
#define INC_1_2_QUEUE_H

#define DEBUG 0

#if DEBUG == 1
#include <iostream>
#endif

#include <stdlib.h>
#include <string.h>

#define ARR_DEFAULT_SCALING_FACTOR 2

class Queue {

    protected:

    const size_t payloadSize;

    explicit Queue(const size_t payloadSize): payloadSize(payloadSize) {}

    public:
    virtual ~Queue() = default;

    public:

    virtual bool empty() = 0;

    virtual void* push() = 0;

    virtual void* poll() = 0;

    virtual bool pollApply() = 0;

};

typedef struct linkedlistqueue_node_s {
    struct linkedlistqueue_node_s* next;
} linkedlistqueue_node_t;

class LinkedListQueue : public Queue {

    private:

    linkedlistqueue_node_t* top;
    linkedlistqueue_node_t* bottom;

    public:

    explicit LinkedListQueue(const size_t payloadSize): Queue(payloadSize) {
        top = nullptr;
        bottom = nullptr;
    }

    virtual ~LinkedListQueue() {

        while (!empty()) {
            pollApply();
        }

    }

    bool empty() override {
        return top == nullptr; // could also check bottom, but it doesn't matter
    }

    void* push() override {

#if DEBUG == 1
        std::cout << "Pushing... (payload size: " << payloadSize << ")" << std::endl;
#endif
        linkedlistqueue_node_t* memory = (linkedlistqueue_node_t*)malloc(sizeof(linkedlistqueue_node_t) + payloadSize);

        if (memory == nullptr) {
            return nullptr;
        }

        memory->next = nullptr;

        if (top == nullptr) {
            // top == null, the queue is empty, we are pushing the first element
            // bottom is null
            bottom = memory;

        }
        else {

            top->next = memory;

        }

        top = memory;

        return ((char*)memory) + sizeof(linkedlistqueue_node_t);

    }

    void* poll() override {

        return ((char*)bottom) + sizeof(linkedlistqueue_node_t);

    }

    bool pollApply() override {

        linkedlistqueue_node_t* bottomMemory = bottom;

        bottom = bottom->next;

        free(bottomMemory);

        if (bottom == nullptr) {
            // we've deleted the last element
            top = nullptr;
        }

        return true;

    }

};

class VectorQueue : public Queue {

    private:

    char* memory;
    size_t capacity;
    int top;
    int bottom;

    public:

    explicit VectorQueue(const size_t payloadSize, const size_t capacity):
        Queue(payloadSize), capacity(capacity), top(0), bottom(0) {

        memory = new char[payloadSize * capacity];

        // memory = (char*)malloc(payloadSize * capacity);

    }

    virtual ~VectorQueue() {

        delete memory;

    }

    bool empty() override {
        return top == bottom;
    }

    void* push() override {

        int oldTop = top;

        int newTop = (top + 1) % capacity;

        if (newTop == bottom) {
            return nullptr;
        }

        top = newTop;

        return memory + (payloadSize * oldTop);

    }

    void* poll() override {

        return memory + (payloadSize * bottom);

    }

    bool pollApply() override {

        /*length--;

        for (size_t i = 0; i < length; i++) {

            memcpy(
                memory + (i * payloadSize),
                memory + ((i + 1) * payloadSize),
                payloadSize
            );

            // *(memory + (i * payloadSize)) = *(memory + ((i + 1) * payloadSize));

        }*/

        /*memmove(
            memory,
            ((char*)memory) + payloadSize,
            --length
        );*/

        // return arraySnapped();
        // return true;

        bottom = (bottom + 1) % capacity;

        return true;

    }

    private:

    /*bool arraySnapped() {

        return length <= allocated / ARR_DEFAULT_SCALING_FACTOR ?
               resize(allocated / ARR_DEFAULT_SCALING_FACTOR) : true;

    }

    bool resize(size_t newCapacity) {

        if (newCapacity == 0) {
            newCapacity = length + 1;
        }

        char* newMemory;

        if (allocated == 0) {

            // the array was initialized with initial capacity 0
            newMemory = (char*)malloc(payloadSize * newCapacity);

        }
        else {

            newMemory = (char*)realloc(memory, payloadSize * newCapacity);

        }

        if (newMemory == nullptr) return false;

        memory = newMemory;

        allocated = newCapacity;

        return true;

    }*/

};

#endif //INC_1_2_QUEUE_H