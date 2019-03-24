#include <iostream>
#include <fstream>

class BinaryTree;

class BinaryTreeNode {

    friend class BinaryTree;

public:
    int value;

private:
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    explicit BinaryTreeNode(int value)
        : value(value), left(nullptr), right(nullptr) {}

    // methods

    static void print(BinaryTreeNode* node, int &indent);
    void printSim();
    void printStraight();
    static BinaryTreeNode* insert(BinaryTreeNode* root, int value);
    static bool balanced(BinaryTreeNode* root, int &height);

};

bool BinaryTreeNode::balanced(BinaryTreeNode *root, int &height) {

    if (root == nullptr) {
        height = 0;
        return true;
    }

    int leftSubtreeHeight = 0, rightSubtreeHeight = 0;

    bool leftSubtreeBalanced = BinaryTreeNode::balanced(root->left, leftSubtreeHeight);

    bool rightSubtreeBalanced = BinaryTreeNode::balanced(root->right, rightSubtreeHeight);

    height = (leftSubtreeHeight > rightSubtreeHeight ? leftSubtreeHeight : rightSubtreeHeight) + 1;

    if (abs(leftSubtreeHeight - rightSubtreeHeight) > 1) {
        // node is not balanced
        return false;
    }

    // for the node to be balanced the left subtree and the right subtree should be balanced
    return leftSubtreeBalanced && rightSubtreeBalanced;

}

void BinaryTreeNode::print(BinaryTreeNode* node, int &indent) {

    indent++;

    for (int i = 0; i < indent - 1; i++) {
        std::cout << "    ";
    }

    if (node == nullptr) {
        std::cout << "null" << std::endl;
        indent--;
        return;
    }
    else {
        std::cout << node->value << std::endl;
    }

    BinaryTreeNode::print(node->left, indent);
    BinaryTreeNode::print(node->right, indent);

    indent--;

}

void BinaryTreeNode::printSim() {

    if (left != nullptr) {
        left->printSim();
    }
    else {
        std::cout << ". ";
    }

    std::cout << value << ' ';

    if (right != nullptr) {
        right->printSim();
    }
    else {
        std::cout << ". ";
    }

}

void BinaryTreeNode::printStraight() {

    std::cout << value << ' ';

    if (left != nullptr) {
        left->printStraight();
    }
    else {
        std::cout << ". ";
    }

    if (right != nullptr) {
        right->printStraight();
    }
    else {
        std::cout << ". ";
    }

}

BinaryTreeNode* BinaryTreeNode::insert(BinaryTreeNode* root, int value) {

    if (root == nullptr) {
        return new BinaryTreeNode(value);
    }

    if (value < root->value) {
        root->left = BinaryTreeNode::insert(root->left, value);
    }
    else {
        root->right = BinaryTreeNode::insert(root->right, value);
    }

    return root;

}

class BinaryTree {

private:
    BinaryTreeNode* top;

    static void deleteTree(BinaryTreeNode* node);

public:

    explicit BinaryTree(): top(nullptr) {}

    ~BinaryTree() {
        if (top != nullptr) {
            deleteTree(top);
        }
    }

    void print() {

        int indent = 0;

        BinaryTreeNode::print(top, indent);

    }

    void printSim() {
        if (top != nullptr) {
            top->printSim();
        }
    }

    void printStraight() {
        if (top != nullptr) {
            top->printStraight();
        }
    }

    void insert(int value) {
        // std::cout << top << std::endl;

        top = BinaryTreeNode::insert(top, value);

    }

    bool balanced() {

        int h;
        return BinaryTreeNode::balanced(top, h);

    }

};

void BinaryTree::deleteTree(BinaryTreeNode* node) {

    if (node->left != nullptr) {
        BinaryTree::deleteTree(node->left);
    }

    if (node->right != nullptr) {
        BinaryTree::deleteTree(node->right);
    }

    delete node;

}

int main() {

    auto tree = BinaryTree();

    std::ifstream file("tree.txt");

    {
        int v;
        while (file >> v) {
            tree.insert(v);
        }
    }

    file.close();

    std::cout << "Tree:" << std::endl;
    tree.print();

    std::cout << "Symmetric:" << std::endl;

    tree.printSim();
    std::cout << std::endl;

    std::cout << "Straight:" << std::endl;

    tree.printStraight();
    std::cout << std::endl;

    std::cout << "Balanced: ";
    std::cout << (tree.balanced() ? "true" : "false") << std::endl;

    system("pause");

    return 0;

}