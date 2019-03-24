#include <iostream>

template <class T>
class ReducedEntryDeck {

protected:

    explicit ReducedEntryDeck() = default;

public:

    virtual ~ReducedEntryDeck() = default;

    virtual void push(T* data) = 0;

    virtual T popLeft() = 0;

    virtual T popRight() = 0;

    virtual T peekLeft() = 0;

    virtual T peekRight() = 0;

    virtual bool empty() = 0;

};

struct linkedlistdeck_node_header {
    struct linkedlistdeck_node_header* previous;
    struct linkedlistdeck_node_header* next;
};

template <class T>
class LinkedListReducedEntryDeck : public ReducedEntryDeck<T> {

private:
    struct linkedlistdeck_node_header* top;
    struct linkedlistdeck_node_header* bottom;

public:

    LinkedListReducedEntryDeck(): top(nullptr), bottom(nullptr) {}

    ~LinkedListReducedEntryDeck() override {
        while (!empty()) {
            popLeft();
        }
    }

    void push(T* data) override;

    T popLeft() override; // get the left element and pop it

    T popRight() override; // get the right element and pop it

    T peekLeft() override; // get the left element

    T peekRight() override; // get the right element

    bool empty() override {
        return top == nullptr;
    }

};

template <class T>
void LinkedListReducedEntryDeck<T>::push(T *data) {

    linkedlistdeck_node_header* memory =
        (linkedlistdeck_node_header*)malloc(sizeof(struct linkedlistdeck_node_header) + sizeof(T));

    memory->next = nullptr;
    memory->previous = top;

    if (top == nullptr) {
        // deck is empty
        bottom = memory;
    }
    else {
        top->next = memory;
    }

    top = memory;

    // write payload

    *((T*)&memory[1]) = *data;

}

template <class T>
T LinkedListReducedEntryDeck<T>::peekLeft() {

    return *((T*)&bottom[1]);

}

template <class T>
T LinkedListReducedEntryDeck<T>::peekRight() {

    return *((T*)&top[1]);

}

template <class T>
T LinkedListReducedEntryDeck<T>::popLeft() {

    linkedlistdeck_node_header* bottomMemory = bottom;

    T element = *((T*)&bottom[1]);

    bottom = bottom->next;

    free(bottomMemory);

    if (bottom == nullptr) {
        // we've deleted the last element
        top = nullptr;
    }

    return element;

}

template <class T>
T LinkedListReducedEntryDeck<T>::popRight() {

    linkedlistdeck_node_header* topMemory = top;

    T element = *((T*)&top[1]);

    top = top->previous;

    free(topMemory);

    if (top == nullptr) {
        // we've deleted the last element
        bottom = nullptr;
    }

    return element;

}

//////////////////////////////////////

template <class T>
class VectorReducedEntryDeck : public ReducedEntryDeck<T> {

private:

    const size_t capacity;
    T* array;
    size_t top, bottom;

public:

    explicit VectorReducedEntryDeck(const size_t capacity): capacity(capacity), top(0), bottom(0) {

        // array = malloc(sizeof(T) * capacity);
        array = new T[capacity];

    }

    ~VectorReducedEntryDeck() override {

        delete[] array;

    }

    void push(T* data) override;

    T popLeft() override; // get the left element and pop it

    T popRight() override; // get the right element and pop it

    T peekLeft() override; // get the left element

    T peekRight() override; // get the right element

    bool empty() override {

        return top == bottom;

    }

};

template <class T>
void VectorReducedEntryDeck<T>::push(T *data) {

    const size_t oldTop = top;
    const size_t newTop = (top + 1) % capacity;

    if (newTop == bottom) {
        // TODO: deck is full
        return;
    }

    top = newTop;

    array[oldTop] = *data;

}

template <class T>
T VectorReducedEntryDeck<T>::peekLeft() {

    return array[bottom];

}

template <class T>
T VectorReducedEntryDeck<T>::peekRight() {

    return array[top - 1];

}

template <class T>
T VectorReducedEntryDeck<T>::popLeft() {

    const size_t oldBottom = bottom;

    bottom = (bottom + 1) % capacity;

    return array[oldBottom];

}

template <class T>
T VectorReducedEntryDeck<T>::popRight() {

    top--;

    return array[top];

}

//////////////////////////////////////

template <class T>
void testDeck(ReducedEntryDeck<T>* deck) {

    while (true) {

        std::cout << "0 - exit: " << std::endl;
        std::cout << "1 - push(): " << std::endl;
        std::cout << "2 - popLeft(): " << std::endl;
        std::cout << "3 - popRight(): " << std::endl;
        std::cout << "4 - peekLeft(): " << std::endl;
        std::cout << "5 - peekRight(): " << std::endl;
        std::cout << "6 - empty(): " << std::endl;

        std::cout << "Item:";
        int choice;
        std::cin >> choice;

        system("cls");

        switch (choice) {

            case 0:
                return;

            case 1: {

                std::cout << "Enter value to push: ";

                T value;
                std::cin >> value;
                deck->push(&value);

                break;

            }

            case 2:

                if (deck->empty()) {
                    std::cout << "Cannot pop: deck is empty." << std::endl;
                    break;
                }

                std::cout << "Popped value: " << deck->popLeft() << std::endl;

                break;

            case 3:

                if (deck->empty()) {
                    std::cout << "Cannot pop: deck is empty." << std::endl;
                    break;
                }

                std::cout << "Popped value: " << deck->popRight() << std::endl;

                break;

            case 4:

                if (deck->empty()) {
                    std::cout << "Cannot pop: deck is empty." << std::endl;
                    break;
                }

                std::cout << "Peeked value: " << deck->peekLeft() << std::endl;

                break;

            case 5:

                if (deck->empty()) {
                    std::cout << "Cannot pop: deck is empty." << std::endl;
                    break;
                }

                std::cout << "Peeked value: " << deck->peekRight() << std::endl;

                break;

            case 6:

                std::cout << "Deck is empty: " << (deck->empty() ? "true" : "false") << std::endl;

                break;

            default:
                std::cout << "Unknown menu item, try again." << std::endl;
                break;

        }

    }

}

int main() {

    ReducedEntryDeck<unsigned int>* uintDeck;
    ReducedEntryDeck<float>* floatDeck;

    std::cout << "Linked list or stack based deck?" << std::endl;
    std::cout << "Linked list - 0" << std::endl;
    std::cout << "Vector based - 1" << std::endl;

    int deckImpl;
    std::cin >> deckImpl;

    if (deckImpl == 0) {
        uintDeck = new LinkedListReducedEntryDeck<unsigned int>();
        floatDeck = new LinkedListReducedEntryDeck<float>();
    }
    else {
        uintDeck = new VectorReducedEntryDeck<unsigned int>(100);
        floatDeck = new VectorReducedEntryDeck<float>(100);
    }

    while (true) {

        std::cout << "Which template type to test?" << std::endl;
        std::cout << "Exit - 0" << std::endl;
        std::cout << "Unsigned int - 1" << std::endl;
        std::cout << "Float - 2" << std::endl;

        int test;
        std::cin >> test;

        system("cls");

        if (test == 0) {
            break;
        }

        switch (test) {

            case 1:
                testDeck<unsigned int>(uintDeck);
                break;

            case 2:
                testDeck<float>(floatDeck);
                break;

            default:
                std::cout << "Unknown menu item, try again." << std::endl;
                break;

        }


    }

    delete uintDeck;
    delete floatDeck;

    return 0;

}