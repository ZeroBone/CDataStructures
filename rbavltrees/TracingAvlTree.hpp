#ifndef RBAVLTREES_TRACINGAVLTREE_HPP
#define RBAVLTREES_TRACINGAVLTREE_HPP

#include <iostream>

template <class T>
class TracingAvlTree;

template <class T>
class TracingAvlTreeNode {

    friend class TracingAvlTree<T>;

    public:

    T data;

    private:

    explicit TracingAvlTreeNode() = default;
    
    TracingAvlTreeNode* left;
    TracingAvlTreeNode* right;
    
    int mHeight;

    static void print(TracingAvlTreeNode* node, int &indent);

    static void destroy(TracingAvlTreeNode<T>* t);

    static TracingAvlTreeNode<T>* insert(T x, TracingAvlTreeNode<T>* node);

    static TracingAvlTreeNode<T>* singleRightRotate(TracingAvlTreeNode<T>*& node);

    static TracingAvlTreeNode<T>* singleLeftRotate(TracingAvlTreeNode<T>*& node);

    static TracingAvlTreeNode<T>* doubleLeftRotate(TracingAvlTreeNode<T>*& node);

    static TracingAvlTreeNode<T>* doubleRightRotate(TracingAvlTreeNode<T>*& node);

    static TracingAvlTreeNode<T>* findMin(TracingAvlTreeNode<T>* node);

    static TracingAvlTreeNode<T>* findMax(TracingAvlTreeNode<T>* node);

    static TracingAvlTreeNode<T>* findValue(TracingAvlTreeNode<T>* node, T& value, unsigned long& traceCompares);

    static TracingAvlTreeNode<T>* remove(T x, TracingAvlTreeNode<T>* node);

    static int height(TracingAvlTreeNode<T>* node);

    static int getBalance(TracingAvlTreeNode<T>* node);
    
};

template <class T>
class TracingAvlTree {

    private:

    TracingAvlTreeNode<T>* root;

    public:

    int nodesCount = 0;

    explicit TracingAvlTree() : root(nullptr) {}
    explicit TracingAvlTree(TracingAvlTreeNode<T>* root) : root(root) {}

    ~TracingAvlTree() {

        if (root != nullptr) {
            TracingAvlTreeNode<T>::destroy(root);
        }

    }

    void insert(T value);

    void remove(T value);

    TracingAvlTreeNode<T>* find(T value, unsigned long& traceCompares) {

        if (root == nullptr) {
            return nullptr;
        }

        return TracingAvlTreeNode<T>::findValue(root, value, traceCompares);

    }

    void print();

    size_t getMemoryUsage() const {

        return nodesCount * sizeof(TracingAvlTreeNode<T>);

    }

};

// implementation

template <class T>
void TracingAvlTreeNode<T>::print(TracingAvlTreeNode<T>* node, int &indent) {

    for (int i = 0; i < indent; i++) {
        std::cout << "    ";
    }

    std::cout << node->data << std::endl;

    if (node->left != nullptr) {
        indent++;
        TracingAvlTreeNode::print(node->left, indent);
        indent--;
    }
    else {

        for (int i = 0; i <= indent; i++) {
            std::cout << "    ";
        }

        std::cout << "-" << std::endl;

    }

    if (node->right != nullptr) {

        indent++;
        TracingAvlTreeNode::print(node->right, indent);
        indent--;

    }
    else {

        for (int i = 0; i <= indent; i++) {
            std::cout << "    ";
        }

        std::cout << "-" << std::endl;

    }

}

template <class T>
void TracingAvlTreeNode<T>::destroy(TracingAvlTreeNode<T>* t) {

    if (t == nullptr) {
        return;
    }

    destroy(t->left);
    destroy(t->right);

    delete t;

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::insert(T x, TracingAvlTreeNode<T>* node) {

    if (node == nullptr) {

        node = new TracingAvlTreeNode<T>();

        node->data = x;
        node->mHeight = 0;
        node->left = node->right = nullptr;

    }
    else if (x < node->data) {

        // got to the left
        node->left = insert(x, node->left);

        // balance
        if (height(node->left) - height(node->right) == 2) {

            if (x < node->left->data) {
                node = singleRightRotate(node);
            }
            else {
                node = doubleRightRotate(node);
            }

        }

    }
    else if (x > node->data) {

        // got to the right
        node->right = insert(x, node->right);

        // balance
        if (height(node->right) - height(node->left) == 2) {

            if (x > node->right->data) {
                node = singleLeftRotate(node);
            }
            else {
                node = doubleLeftRotate(node);
            }

        }

    }

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;

    return node;

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::singleRightRotate(TracingAvlTreeNode<T>*& node) {

    TracingAvlTreeNode<T>* leftNode = node->left;

    node->left = leftNode->right;
    leftNode->right = node;

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;
    leftNode->mHeight = std::max(height(leftNode->left), node->mHeight) + 1;

    return leftNode;

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::singleLeftRotate(TracingAvlTreeNode<T>*& node) {

    TracingAvlTreeNode<T>* rightNode = node->right;

    node->right = rightNode->left;
    rightNode->left = node;

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;
    rightNode->mHeight = std::max(height(node->right), node->mHeight) + 1;

    return rightNode;

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::doubleLeftRotate(TracingAvlTreeNode<T>*& node) {

    node->right = singleRightRotate(node->right);

    return singleLeftRotate(node);

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::doubleRightRotate(TracingAvlTreeNode<T>*& node) {

    node->left = singleLeftRotate(node->left);

    return singleRightRotate(node);

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::findMin(TracingAvlTreeNode<T>* node) {
    if (node == nullptr)
        return nullptr;
    else if (node->left == nullptr)
        return node;
    else
        return findMin(node->left);
}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::findMax(TracingAvlTreeNode<T>* node) {
    if (node == nullptr)
        return nullptr;
    else if (node->right == nullptr)
        return node;
    else
        return findMax(node->right);
}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::findValue(TracingAvlTreeNode<T>* node, T& value, unsigned long& traceCompares) {

    if (traceCompares++, value == node->data) {
        return node;
    }

    if (traceCompares++, value < node->data) {

        return node->left == nullptr ? nullptr : findValue(node->left, value, traceCompares);

    }

    return traceCompares++, node->right == nullptr ? nullptr : findValue(node->right, value, traceCompares);

}

template <class T>
TracingAvlTreeNode<T>* TracingAvlTreeNode<T>::remove(T x, TracingAvlTreeNode<T>* node) {

    TracingAvlTreeNode<T>* temp;

    // Element not found
    if (node == nullptr) {
        return nullptr;
    }
    else if (x < node->data) { // searching for element
        node->left = remove(x, node->left);
    }
    else if (x > node->data) {
        node->right = remove(x, node->right);
    }
    else if (node->left && node->right) { // element found with 2 children

        temp = findMin(node->right);
        node->data = temp->data;
        node->right = remove(node->data, node->right);

    }
    else { // one or no children

        temp = node;

        if (node->left == nullptr) {
            node = node->right;
        }
        else if (node->right == nullptr) {
            node = node->left;
        }

        delete temp;

    }

    if (node == nullptr) {
        return node;
    }

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;

    // If node is unbalanced
    // If left node is deleted, right case
    if (height(node->left) - height(node->right) == 2) {
        // right right case
        if (height(node->left->left) - height(node->left->right) == 1)
            return singleLeftRotate(node);
            // right left case
        else
            return doubleLeftRotate(node);
    }
        // If right node is deleted, left case
    else if (height(node->right) - height(node->left) == 2) {
        // left left case
        if (height(node->right->right) - height(node->right->left) == 1)
            return singleRightRotate(node);
            // left right case
        else
            return doubleRightRotate(node);
    }

    return node;

}

template <class T>
int TracingAvlTreeNode<T>::height(TracingAvlTreeNode<T>* node) {

    return (node == nullptr ? -1 : node->mHeight);

}

template <class T>
int TracingAvlTreeNode<T>::getBalance(TracingAvlTreeNode<T>* node) {

    if (node == nullptr) {
        return 0;
    }
    else {
        return height(node->left) - height(node->right);
    }

}

// TracingAvlTree implementation

template <class T>
void TracingAvlTree<T>::insert(T value) {

    nodesCount++;

    root = TracingAvlTreeNode<T>::insert(value, root);

}

template <class T>
void TracingAvlTree<T>::remove(T value) {

    root = TracingAvlTreeNode<T>::remove(value, root);

}

template <class T>
void TracingAvlTree<T>::print() {

    if (root == nullptr) {
        return;
    }

    int indent = 0;
    TracingAvlTreeNode<T>::print(root, indent);

}

#endif