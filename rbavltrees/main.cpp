#include <iostream>
#include <fstream>
#include "TracingAvlTree.hpp"
#include "TracingRbTree.hpp"

int main() {

    std::cout << "Loading data structures..." << std::endl;

    auto avlTree = new TracingAvlTree<unsigned long>();
    auto rbTree = new TracingRBTree<unsigned long>();

    std::cout << "Loading data into data structures..." << std::endl;

    std::fstream file("f4.txt");

    unsigned long d;
    while (file >> d) {

        avlTree->insert(d);
        rbTree->insert(d, &d);

    }

    // avlTree->print();

    std::cout << "Data loaded." << std::endl;
    std::cout << "RB Memory usage: Nodes: " << rbTree->nodesCount << " Bytes: " << rbTree->getMemoryUsage() << std::endl;
    std::cout << "AVL Memory usage: Nodes: " << avlTree->nodesCount << " Bytes: " << avlTree->getMemoryUsage() << std::endl;

    /*avlTree->remove(3);
    avlTree->print();
    avlTree->remove(6);
    avlTree->print();*/

    unsigned long avlCompares = 0;
    unsigned long rbCompares = 0;

    for (unsigned long i = 0; i < 100; i++) {

        unsigned long key = i + 1;

        // avl
        unsigned long currentAvlCompares = 0;

        avlTree->find(key, currentAvlCompares);

        avlCompares += currentAvlCompares;

        // rb
        unsigned long currentRbCompares = 0;

        rbTree->lookup(key, currentRbCompares);

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