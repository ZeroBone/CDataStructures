#include <iostream>
#include <windows.h>
#include "queue.h"

#define DEBUG 0

typedef struct {
    int c;
    int e;
} polynom_node_t;

inline void inputNode(polynom_node_t* node) {

    std::cout << "¬ведите коэффициент при x: ";
    std::cin >> node->c;

    std::cout << "¬ведите степень: ";
    std::cin >> node->e;

}

inline bool inputPolynom(Queue* queue) {

    int previousE;
    polynom_node_t node;

    inputNode(&node);

    previousE = node.e;

    *((polynom_node_t*)queue->push()) = node;

    while (node.e > 0) {

        inputNode(&node);

        if (node.e >= previousE) {
            std::cerr << "Ёлементы многочлена введены в неправильном пор€дке." << std::endl;
            return false;
        }

        *((polynom_node_t*)queue->push()) = node;

        previousE = node.e;

    }

    return true;

}

inline void printPolynom(Queue* polynom) {

    if (polynom->empty()) return;

    if (((polynom_node_t*)polynom->poll())->c < 0) {
        std::cout << "-";
    }

    while (1) {

        std::cout << abs(((polynom_node_t*)polynom->poll())->c);
        std::cout << "x^" << ((polynom_node_t*)polynom->poll())->e;

        polynom->pollApply();

        if (polynom->empty()) {
            break;
        }

        if (((polynom_node_t*)polynom->poll())->c < 0) {
            std::cout << " - ";
        }
        else {
            std::cout << " + ";
        }

    }

    std::cout << std::endl;

}

inline Queue* emptyPolynom(const bool linkedList) {

    if (linkedList) {
        return new LinkedListQueue(sizeof(polynom_node_t));
    }

    return new VectorQueue(sizeof(polynom_node_t), 10);

}

int main() {

    setlocale(LC_ALL, "Russian");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "”кажите структуру хранени€:" << std::endl;
    std::cout << "0 - векторна€" << std::endl;
    std::cout << "1 - спискова€" << std::endl;

    bool useLinkedList;
    std::cin >> useLinkedList;

    std::cout << "¬ведите первый многочлен:" << std::endl;

    Queue* firstPolynom = emptyPolynom(useLinkedList);

    if (!inputPolynom(firstPolynom)) {
        system("pause");
        return 1;
    }

    std::cout << "=================" << std::endl;
    /*std::cout << "ѕервый многочлен:" << std::endl;
    printPolynom(firstPolynom);*/
    std::cout << "¬ведите второй многочлен:" << std::endl;

    Queue* secondPolynom = emptyPolynom(useLinkedList);

    if (!inputPolynom(secondPolynom)) {
        system("pause");
        return 1;
    }

    std::cout << "=================" << std::endl;
    /*std::cout << "¬торой многочлен:" << std::endl;
    printPolynom(secondPolynom);*/

    Queue* resultPolynom = emptyPolynom(useLinkedList);

    // do the polynom subtraction

    while (!firstPolynom->empty() && !secondPolynom->empty()) {
#if DEBUG == 1
        std::cout << "First e: " << ((polynom_node_t*)firstPolynom->poll())->e << "Second e: " << ((polynom_node_t*)secondPolynom->poll())->e << std::endl;
#endif
        if (((polynom_node_t*)firstPolynom->poll())->e == ((polynom_node_t*)secondPolynom->poll())->e) {

            polynom_node_t* newNode = (polynom_node_t*)resultPolynom->push();

            newNode->e = ((polynom_node_t*)firstPolynom->poll())->e;
            newNode->c = ((polynom_node_t*)firstPolynom->poll())->c - ((polynom_node_t*)secondPolynom->poll())->c;

            firstPolynom->pollApply();
            secondPolynom->pollApply();

        }
        else if (((polynom_node_t*)firstPolynom->poll())->e > ((polynom_node_t*)secondPolynom->poll())->e) {

            *((polynom_node_t*)resultPolynom->push()) = *((polynom_node_t*)firstPolynom->poll());

            firstPolynom->pollApply();

        }
        else {

            polynom_node_t* newNode = (polynom_node_t*)resultPolynom->push();

            newNode->c = -((polynom_node_t*)secondPolynom->poll())->c;
            newNode->e = ((polynom_node_t*)secondPolynom->poll())->e;

            secondPolynom->pollApply();

        }

    }

    // output result

    std::cout << "–езультат вычитани€:" << std::endl;
    printPolynom(resultPolynom);

    delete firstPolynom;
    delete secondPolynom;
    delete resultPolynom;

    system("pause");

    return 0;

}