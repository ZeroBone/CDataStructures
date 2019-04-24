#ifndef RBAVLTREES_AVLTREE_HPP
#define RBAVLTREES_AVLTREE_HPP

#include <iostream>

template <class T>
class AvlTree;

template <class T>
class AvlTreeNode {

    friend class AvlTree<T>;

    public:

    T data;

    private:
    
    AvlTreeNode* left;
    AvlTreeNode* right;
    
    int mHeight;

    static void print(AvlTreeNode* node, int &indent);

    static void destroy(AvlTreeNode<T>* t);

    static AvlTreeNode<T>* insert(T x, AvlTreeNode<T>* node);

    static AvlTreeNode<T>* singleRightRotate(AvlTreeNode<T>*& node);

    static AvlTreeNode<T>* singleLeftRotate(AvlTreeNode<T>*& node);

    static AvlTreeNode<T>* doubleLeftRotate(AvlTreeNode<T>*& node);

    static AvlTreeNode<T>* doubleRightRotate(AvlTreeNode<T>*& node);

    static AvlTreeNode<T>* findMin(AvlTreeNode<T>* node);

    static AvlTreeNode<T>* findMax(AvlTreeNode<T>* node);

    static AvlTreeNode<T>* findValue(AvlTreeNode<T>* node, T& value);

    static AvlTreeNode<T>* remove(T x, AvlTreeNode<T>* node);

    static int height(AvlTreeNode<T>* node);

    static int getBalance(AvlTreeNode<T>* node);
    
};

template <class T>
class AvlTree {

    private:

    AvlTreeNode<T>* root;

    public:

    explicit AvlTree() : root(nullptr) {}
    explicit AvlTree(AvlTreeNode<T>* root) : root(root) {}

    ~AvlTree() {

        if (root != nullptr) {
            AvlTreeNode<T>::destroy(root);
        }

    }

    void insert(T value);

    void remove(T value);

    AvlTreeNode<T>* find(T value) {

        if (root == nullptr) {
            return nullptr;
        }

        return AvlTreeNode<T>::findValue(root, value);

    }

    void print();

};

// implementation

template <class T>
void AvlTreeNode<T>::print(AvlTreeNode<T>* node, int &indent) {

    for (int i = 0; i < indent; i++) {
        std::cout << "    ";
    }

    std::cout << node->data << std::endl;

    if (node->left != nullptr) {
        indent++;
        AvlTreeNode::print(node->left, indent);
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
        AvlTreeNode::print(node->right, indent);
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
void AvlTreeNode<T>::destroy(AvlTreeNode<T>* t) {

    if (t == nullptr) {
        return;
    }

    destroy(t->left);
    destroy(t->right);

    delete t;

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::insert(T x, AvlTreeNode<T>* node) {

    if (node == nullptr) {

        node = new AvlTreeNode<T>();

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
AvlTreeNode<T>* AvlTreeNode<T>::singleRightRotate(AvlTreeNode<T>*& node) {

    AvlTreeNode<T>* leftNode = node->left;

    node->left = leftNode->right;
    leftNode->right = node;

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;
    leftNode->mHeight = std::max(height(leftNode->left), node->mHeight) + 1;

    return leftNode;

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::singleLeftRotate(AvlTreeNode<T>*& node) {

    AvlTreeNode<T>* rightNode = node->right;

    node->right = rightNode->left;
    rightNode->left = node;

    node->mHeight = std::max(height(node->left), height(node->right)) + 1;
    rightNode->mHeight = std::max(height(node->right), node->mHeight) + 1;

    return rightNode;

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::doubleLeftRotate(AvlTreeNode<T>*& node) {

    node->right = singleRightRotate(node->right);

    return singleLeftRotate(node);

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::doubleRightRotate(AvlTreeNode<T>*& node) {

    node->left = singleLeftRotate(node->left);

    return singleRightRotate(node);

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::findMin(AvlTreeNode<T>* node) {
    if (node == nullptr)
        return nullptr;
    else if (node->left == nullptr)
        return node;
    else
        return findMin(node->left);
}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::findMax(AvlTreeNode<T>* node) {
    if (node == nullptr)
        return nullptr;
    else if (node->right == nullptr)
        return node;
    else
        return findMax(node->right);
}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::findValue(AvlTreeNode<T>* node, T& value) {

    if (value == node->data) {
        return node;
    }

    if (value < node->data) {

        return node->left == nullptr ? nullptr : findValue(node->left, value);

    }

    return node->right == nullptr ? nullptr : findValue(node->right, value);

}

template <class T>
AvlTreeNode<T>* AvlTreeNode<T>::remove(T x, AvlTreeNode<T>* node) {

    AvlTreeNode<T>* temp;

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
int AvlTreeNode<T>::height(AvlTreeNode<T>* node) {

    return (node == nullptr ? -1 : node->mHeight);

}

template <class T>
int AvlTreeNode<T>::getBalance(AvlTreeNode<T>* node) {

    if (node == nullptr) {
        return 0;
    }
    else {
        return height(node->left) - height(node->right);
    }

}

// AvlTree implementation

template <class T>
void AvlTree<T>::insert(T value) {

    root = AvlTreeNode<T>::insert(value, root);

}

template <class T>
void AvlTree<T>::remove(T value) {

    root = AvlTreeNode<T>::remove(value, root);

}

template <class T>
void AvlTree<T>::print() {

    if (root == nullptr) {
        return;
    }

    int indent = 0;
    AvlTreeNode<T>::print(root, indent);

}

#endif