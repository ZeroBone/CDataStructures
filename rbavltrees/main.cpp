#include <iostream>
#include <fstream>
#include "TracingAvlTree.hpp"
#include "TracingRbTree.hpp"

int main() {

    std::cout << "Loading data structures..." << std::endl;

    auto avlTree = new TracingAvlTree<long>();
    auto rbTree = new TracingRbTree<long>();

    std::cout << "Loading data into data structures..." << std::endl;

    std::fstream file("test.txt");

    int searchIndex = 0;
    long toSearch[100];

    long d;
    while (file >> d) {

        cout << "Inserting " << d << endl;

        avlTree->insert(d);
        rbTree->insert(d);

        if (searchIndex < 100) {
            toSearch[searchIndex++] = d;
        }

    }

    // avlTree->print();

    std::cout << "Data loaded." << std::endl;
    std::cout << "RB node size: " << rbTree->getNodeSize() << std::endl;
    std::cout << "AVL node size: " << avlTree->getNodeSize() << std::endl;

    /*avlTree->remove(3);
    avlTree->print();
    avlTree->remove(6);
    avlTree->print();*/

    unsigned long avlCompares = 0;
    unsigned long rbCompares = 0;

    for (unsigned long i = 0; i < 100; i++) {
        // 3 несуществующих, минимальное, максимальное и 3 произвольных
        long key = toSearch[i];

        // avl
        unsigned long currentAvlCompares = 0;

        avlTree->find(key, currentAvlCompares);

        avlCompares += currentAvlCompares;

        // rb
        unsigned long currentRbCompares = 0;

        rbTree->search(key);

        std::cout << "Key: " << key << " AVL Compares: " << currentAvlCompares << " RB compares: " << currentRbCompares << std::endl;

        rbCompares += currentRbCompares;

    }

    avlCompares /= 100;
    rbCompares /= 100;

    std::cout << "--------------" << std::endl;
    std::cout << "Average AVL compares: " << avlCompares << std::endl;
    std::cout << "Average RB compares: " << rbCompares << std::endl;

    file.close();

    system("pause");

    return 0;
}