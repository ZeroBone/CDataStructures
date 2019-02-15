#include "queue.h"
#include <stdlib.h>
#include <string.h>

#define ARR_DEFAULT_SCALING_FACTOR 2

class Queue {

    protected:

    const size_t payloadSize;

    explicit Queue(const size_t payloadSize): payloadSize(payloadSize) {}

    public:

    virtual bool empty() = 0;

    virtual void* push() = 0;

    virtual void* poll() = 0;

    virtual bool pollApply() = 0;

};

class LinkedListQueue : public Queue {



};

class VectorQueue : public Queue {

    private:

    void* memory;
    size_t length;
    size_t allocated;

    public:

    VectorQueue(const size_t payloadSize, const size_t capacity): Queue(payloadSize), allocated(capacity), length(0) {

        if (allocated != 0) {

            memory = malloc(payloadSize * allocated);

        }

    }

    bool empty() override {
        return length == 0;
    }

    void* push() override {

        return ((char*)memory) + (payloadSize * length++);

    }

    void* poll() override {

        return memory;

    }

    bool pollApply() override {

        memmove(
            memory,
            ((char*)memory) + payloadSize,
            --length
        );

        return arraySnapped();

    }

    private:

    bool arraySnapped() {

        return length <= allocated / ARR_DEFAULT_SCALING_FACTOR ?
        resize(allocated / ARR_DEFAULT_SCALING_FACTOR) : true;

    }

    bool resize(const size_t newCapacity) {

        void* newMemory;

        if (allocated == 0) {

            // the array was initialized with initial capacity 0
            newMemory = malloc(payloadSize * newCapacity);

        }
        else {

            newMemory = realloc(memory, payloadSize * newCapacity);

        }

        if (newMemory == nullptr) return false;

        memory = newMemory;

        allocated = newCapacity;

        return true;

    }

};