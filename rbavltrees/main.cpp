#include <iostream>
#include <fstream>
#include "TracingAvlTree.hpp"
// #include "TracingRbTree.hpp"
// #include "RbTree.hpp"
#include "rb.hpp"

using namespace std;

int main() {

    std::cout << "Loading data structures..." << std::endl;

    auto avlTree = new TracingAvlTree<long>();
    // auto rbTree = new TracingRbTree<long>();
    // auto rbTree = new RedBlackTree<long>();

    NIL.left = NIL.right = NIL.p = NILPTR;
    NIL.color = BLACK;
    NIL.size = 0;
    NODEPTR tree = NILPTR;

    std::cout << "Loading data into data structures..." << std::endl;

    std::fstream file("f4.txt");

    int searchIndex = 2;
    long toSearch[8];
    toSearch[0] = 99999999; // min
    toSearch[1] = 0; // max
    // 2 - 3 - 4 - random existing
    // 5 - 6 - 7 - random not existing
    toSearch[5] = 69224884;
    toSearch[6] = 32302172;
    toSearch[7] = 41473356;

    long d;
    while (file >> d) {

        if (d < toSearch[0]) {
            toSearch[0] = d;
        }

        if (d > toSearch[1]) {
            toSearch[1] = d;
        }

        // cout << "Inserting " << d << endl;

        avlTree->insert(d);
        // rbTree->insert(d);
        // rbTree->insert(d);
        rbinsert(&tree, d);

        if (searchIndex < 5) {
            toSearch[searchIndex++] = d;
        }

    }

    // avlTree->print();
    // rbTree->print();

    std::cout << "Data loaded." << std::endl;
    // std::cout << "RB node size: " << rbTree->getNodeSize() << std::endl;
    std::cout << "RB node size: " << sizeof(node) << std::endl;
    std::cout << "AVL node size: " << avlTree->getNodeSize() << std::endl;

    /*avlTree->remove(3);
    avlTree->print();
    avlTree->remove(6);
    avlTree->print();*/

    unsigned long avlCompares = 0;
    unsigned long rbCompares = 0;

    for (unsigned long i = 0; i < 8; i++) {
        // 3 несуществующих, минимальное, максимальное и 3 произвольных
        long key = toSearch[i];

        // avl
        unsigned long currentAvlCompares = 0;

        avlTree->find(key, currentAvlCompares);

        avlCompares += currentAvlCompares;

        // rb
        int currentRbCompares = 0;

        // rbTree->search(key);
        // rbTree->find(key, currentRbCompares);
        // search(tree, key);
        node* n = searchCountCompares(tree, key, &currentRbCompares);
        if (n != NILPTR && n->key != key) {
            cout << "ASSERT FAILED" << endl;
            return 1;
        }

        switch (i) {

            case 0:
                cout << "MIN: ";
                break;

            case 1:
                cout << "MAX: ";
                break;

            case 2:
            case 3:
            case 4:
                cout << "EXISTING: ";
                break;

            case 5:
            case 6:
            case 7:
                cout << "NOT EXISTING: ";
                break;

        }

        std::cout << "Key: " << key << " AVL Compares: " << currentAvlCompares << " RB compares: " << currentRbCompares << std::endl;

        rbCompares += currentRbCompares;

    }

    avlCompares /= 8;
    rbCompares /= 8;

    std::cout << "--------------" << std::endl;
    std::cout << "Average AVL compares: " << avlCompares << std::endl;
    std::cout << "Average RB compares: " << rbCompares << std::endl;

    file.close();

    system("pause");

    return 0;
}