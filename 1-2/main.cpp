#include <iostream>
#include <windows.h>
#include "queue.h"

typedef struct {
    int c;
    int e;
} polynom_node_t;

inline void inputNode(polynom_node_t* node) {

    std::cout << "¬ведите коеффициент при x: ";
    std::cin >> node->c;

    std::cout << "¬ведите степень: ";
    std::cin >> node->e;

}

inline bool inputPolynom(Queue* queue) {

    int previousE;
    polynom_node_t node;

    inputNode(&node);

    previousE = node.e;

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

    while (1) {

        std::cout << ((polynom_node_t*)polynom->poll())->c;
        std::cout << "x^" << ((polynom_node_t*)polynom->poll())->e;

        polynom->pollApply();

        if (polynom->empty()) {

            break;

        }

        std::cout << " + ";

    }

    std::cout << std::endl;

}

int main() {

    setlocale(LC_ALL, "Russian");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "¬ведите первый многочлен:" << std::endl;

    Queue* firstPolynom = new LinkedListQueue(sizeof(polynom_node_t));

    if (!inputPolynom(firstPolynom)) {
        system("pause");
        return 1;
    }

    std::cout << "ѕервый многочлен:" << std::endl;
    printPolynom(firstPolynom);
    std::cout << "¬ведите второй многочлен:" << std::endl;

    Queue* secondPolynom = new LinkedListQueue(sizeof(polynom_node_t));

    if (!inputPolynom(secondPolynom)) {
        system("pause");
        return 1;
    }

    Queue* biggerPolynom;
    Queue* smallerPolynom;

    if (((polynom_node_t*)secondPolynom->poll())->e > ((polynom_node_t*)firstPolynom->poll())->e) {
        biggerPolynom = secondPolynom;
        smallerPolynom = firstPolynom;
    }
    else {
        smallerPolynom = secondPolynom;
        biggerPolynom = firstPolynom;
    }

    Queue* resultPolynom = new LinkedListQueue(sizeof(polynom_node_t));

    while (((polynom_node_t*)biggerPolynom->poll())->e > ((polynom_node_t*)smallerPolynom->poll())->e) {

        *((polynom_node_t*)(resultPolynom->push())) = *((polynom_node_t*)biggerPolynom->poll());

        biggerPolynom->pollApply();

    }

    delete firstPolynom;
    delete secondPolynom;
    delete resultPolynom;

    system("pause");

    return 0;

}