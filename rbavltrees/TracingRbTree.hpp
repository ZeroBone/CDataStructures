#ifndef RBAVLTREES_RBTREE_HPP
#define RBAVLTREES_RBTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cassert>
#define INDENT_STEP  4

using namespace std;
enum color { RED, BLACK };
/*
 * Node RBTree Declaration
 */
typedef struct rbtree_node
{
    enum color color;
    void *key;
    void *value;
    rbtree_node *left, *right, *parent;
}*node;

typedef struct rbtree_t {
    node root;
}*rbtree;

/*
 * Class RBTree Declaration
 */
class RBTree
{
    public:
    typedef int (*compare_func)(void* left, void* right);
    rbtree rbtree_create();
    void* rbtree_lookup(rbtree t, void* , compare_func compare);
    void rbtree_insert(rbtree t, void* , void* , compare_func compare);
    void rbtree_delete(rbtree t, void* , compare_func compare);
    node grandparent(node n);
    node sibling(node n);
    node uncle(node n);
    void verify_properties(rbtree t);
    void verify_property_1(node root);
    void verify_property_2(node root);
    color node_color(node n);
    void verify_property_4(node root);
    void verify_property_5(node root);
    void verify_property_5_helper(node n, int , int*);
    node new_node(void* key, void* , color , node , node);
    node lookup_node(rbtree t, void* , compare_func compare);
    void rotate_left(rbtree t, node n);
    void rotate_right(rbtree t, node n);
    void replace_node(rbtree t, node oldn, node newn);
    void insert_case1(rbtree t, node n);
    void insert_case2(rbtree t, node n);
    void insert_case3(rbtree t, node n);
    void insert_case4(rbtree t, node n);
    void insert_case5(rbtree t, node n);
    node maximum_node(node root);
    void delete_case1(rbtree t, node n);
    void delete_case2(rbtree t, node n);
    void delete_case3(rbtree t, node n);
    void delete_case4(rbtree t, node n);
    void delete_case5(rbtree t, node n);
    void delete_case6(rbtree t, node n);
};
/*
 * Return Grandparent of Node
 */
node RBTree::grandparent(node n)
{
    assert (n != nullptr);
    assert (n->parent != nullptr);
    assert (n->parent->parent != nullptr);
    return n->parent->parent;
}

/*
 * Return Sibling of Node
 */
node RBTree::sibling(node n)
{
    assert (n != nullptr);
    assert (n->parent != nullptr);
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}

/*
 * Return Uncle of Node
 */
node RBTree::uncle(node n)
{
    assert (n != nullptr);
    assert (n->parent != nullptr);
    assert (n->parent->parent != nullptr);
    return sibling(n->parent);
}

/*
 * Verifying Properties of Red black Tree
 */
void RBTree::verify_properties(rbtree t)
{
    verify_property_1 (t->root);
    verify_property_2 (t->root);
    verify_property_4 (t->root);
    verify_property_5 (t->root);
}
/*
 * Verifying Property 1
 */
void RBTree::verify_property_1(node n)
{
    assert (node_color(n) == RED || node_color(n) == BLACK);
    if (n == nullptr)
        return;
    verify_property_1(n->left);
    verify_property_1(n->right);
}
/*
 * Verifying Property 2
 */
void RBTree::verify_property_2(node root)
{
    assert (node_color(root) == BLACK);
}
/*
 * Returns color of a node
 */
color RBTree::node_color(node n)
{
    return n == nullptr ? BLACK : n->color;
}
/*
 * Verifying Property 4
 */
void RBTree::verify_property_4(node n)
{
    if (node_color(n) == RED)
    {
        assert (node_color(n->left) == BLACK);
        assert (node_color(n->right) == BLACK);
        assert (node_color(n->parent) == BLACK);
    }
    if (n == nullptr)
        return;
    verify_property_4(n->left);
    verify_property_4(n->right);
}
/*
 * Verifying Property 5
 */
void RBTree::verify_property_5(node root)
{
    int black_count_path = -1;
    verify_property_5_helper(root, 0, &black_count_path);
}

void RBTree::verify_property_5_helper(node n, int black_count, int* path_black_count)
{
    if (node_color(n) == BLACK)
    {
        black_count++;
    }
    if (n == nullptr)
    {
        if (*path_black_count == -1)
        {
            *path_black_count = black_count;
        }
        else
        {
            assert (black_count == *path_black_count);
        }
        return;
    }
    verify_property_5_helper(n->left,  black_count, path_black_count);
    verify_property_5_helper(n->right, black_count, path_black_count);
}

/*
 * Create Red Black Tree
 */
rbtree RBTree::rbtree_create()
{
    rbtree t = new rbtree_t;
    t->root = nullptr;
    verify_properties(t);
    return t;
}

/*
 * Creating New Node of Reb Black Tree
 */
node RBTree::new_node(void* k, void* v, color n_color, node left, node right)
{
    node result = new rbtree_node;
    result->key = k;
    result->value = v;
    result->color = n_color;
    result->left = left;
    result->right = right;
    if (left  != nullptr)
        left->parent = result;
    if (right != nullptr)
        right->parent = result;
    result->parent = nullptr;
    return result;
}
/*
 * Look Up through Node
 */
node RBTree::lookup_node(rbtree t, void* key, compare_func compare)
{
    node n = t->root;
    while (n != nullptr)
    {
        int comp_result = compare(key, n->key);
        if (comp_result == 0)
        {
            return n;
        }
        else if (comp_result < 0)
        {
            n = n->left;
        }
        else
        {
            assert(comp_result > 0);
            n = n->right;
        }
    }
    return n;
}
/*
 * RbTree Look Up
 */
void* RBTree::rbtree_lookup(rbtree t, void* key, compare_func compare)
{
    node n = lookup_node(t, key, compare);
    return n == nullptr ? nullptr : n->value;
}

/*
 * Rotate left
 */
void RBTree::rotate_left(rbtree t, node n)
{
    node r = n->right;
    replace_node(t, n, r);
    n->right = r->left;
    if (r->left != nullptr)
    {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
}
/*
 * Rotate right
 */
void RBTree::rotate_right(rbtree t, node n)
{
    node L = n->left;
    replace_node(t, n, L);
    n->left = L->right;
    if (L->right != nullptr)
    {
        L->right->parent = n;
    }
    L->right = n;
    n->parent = L;
}
/*
 * Replace a node
 */
void RBTree::replace_node(rbtree t, node oldn, node newn)
{
    if (oldn->parent == nullptr)
    {
        t->root = newn;
    }
    else
    {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != nullptr)
    {
        newn->parent = oldn->parent;
    }
}
/*
 * Insert node into RBTree
 */
void RBTree::rbtree_insert(rbtree t, void* key, void* value, compare_func compare)
{
    node inserted_node = new_node(key, value, RED, nullptr, nullptr);
    if (t->root == nullptr)
    {
        t->root = inserted_node;
    }
    else
    {
        node n = t->root;
        while (1)
        {
            int comp_result = compare(key, n->key);
            if (comp_result == 0)
            {
                n->value = value;
                return;
            }
            else if (comp_result < 0)
            {
                if (n->left == nullptr)
                {
                    n->left = inserted_node;
                    break;
                }
                else
                {
                    n = n->left;
                }
            }
            else
            {
                assert (comp_result > 0);
                if (n->right == nullptr)
                {
                    n->right = inserted_node;
                    break;
                }
                else
                {
                    n = n->right;
                }
            }
        }
        inserted_node->parent = n;
    }
    insert_case1(t, inserted_node);
    verify_properties(t);
}

/*
 * Inserting Case 1
 */
void RBTree::insert_case1(rbtree t, node n)
{
    if (n->parent == nullptr)
        n->color = BLACK;
    else
        insert_case2(t, n);
}

/*
 * Inserting Case 2
 */
void RBTree::insert_case2(rbtree t, node n)
{
    if (node_color(n->parent) == BLACK)
        return;
    else
        insert_case3(t, n);
}

/*
 * Inserting Case 3
 */
void RBTree::insert_case3(rbtree t, node n)
{
    if (node_color(uncle(n)) == RED)
    {
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insert_case1(t, grandparent(n));
    }
    else
    {
        insert_case4(t, n);
    }
}

/*
 * Inserting Case 4
 */
void RBTree::insert_case4(rbtree t, node n)
{
    if (n == n->parent->right && n->parent == grandparent(n)->left)
    {
        rotate_left(t, n->parent);
        n = n->left;
    }
    else if (n == n->parent->left && n->parent == grandparent(n)->right)
    {
        rotate_right(t, n->parent);
        n = n->right;
    }
    insert_case5(t, n);
}

/*
 * Inserting Case 5
 */
void RBTree::insert_case5(rbtree t, node n)
{
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left)
    {
        rotate_right(t, grandparent(n));
    }
    else
    {
        assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotate_left(t, grandparent(n));
    }
}

/*
 * Delete Node from RBTree
 */
void RBTree::rbtree_delete(rbtree t, void* key, compare_func compare)
{
    node child;
    node n = lookup_node(t, key, compare);
    if (n == nullptr)
        return;
    if (n->left != nullptr && n->right != nullptr)
    {
        node pred = maximum_node(n->left);
        n->key   = pred->key;
        n->value = pred->value;
        n = pred;
    }
    assert(n->left == nullptr || n->right == nullptr);
    child = n->right == nullptr ? n->left  : n->right;
    if (node_color(n) == BLACK)
    {
        n->color = node_color(child);
        delete_case1(t, n);
    }
    replace_node(t, n, child);
    free(n);
    verify_properties(t);
}

/*
 * Returns Maximum node
 */
node RBTree::maximum_node(node n)
{
    assert (n != nullptr);
    while (n->right != nullptr)
    {
        n = n->right;
    }
    return n;
}

/*
 * Deleting Case 1
 */
void RBTree::delete_case1(rbtree t, node n)
{
    if (n->parent == nullptr)
        return;
    else
        delete_case2(t, n);
}

/*
 * Deleting Case 2
 */
void RBTree::delete_case2(rbtree t, node n)
{
    if (node_color(sibling(n)) == RED)
    {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotate_left(t, n->parent);
        else
            rotate_right(t, n->parent);
    }
    delete_case3(t, n);
}

/*
 * Deleting Case 3
 */
void RBTree::delete_case3(rbtree t, node n)
{
    if (node_color(n->parent) == BLACK && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        delete_case1(t, n->parent);
    }
    else
        delete_case4(t, n);
}

/*
 * Deleting Case 4
 */
void RBTree::delete_case4(rbtree t, node n)
{
    if (node_color(n->parent) == RED && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(t, n);
}

/*
 * Deleting Case 5
 */
void RBTree::delete_case5(rbtree t, node n)
{
    if (n == n->parent->left && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == RED && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotate_right(t, sibling(n));
    }
    else if (n == n->parent->right && node_color(sibling(n)) == BLACK &&
             node_color(sibling(n)->right) == RED && node_color(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotate_left(t, sibling(n));
    }
    delete_case6(t, n);
}

/*
 * Deleting Case 6
 */
void RBTree::delete_case6(rbtree t, node n)
{
    sibling(n)->color = node_color(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left)
    {
        assert (node_color(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotate_left(t, n->parent);
    }
    else
    {
        assert (node_color(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotate_right(t, n->parent);
    }
}

/*
 * Compare two nodes
 */
int compare_int(void* leftp, void* rightp)
{
    int left = (int)leftp;
    int right = (int)rightp;
    if (left < right)
        return -1;
    else if (left > right)
        return 1;
    else
    {
        assert (left == right);
        return 0;
    }
}
/*
 * Print RBTRee
 */
void print_tree_helper(node n, int indent)
{
    int i;
    if (n == nullptr)
    {
        fputs("<empty tree>", stdout);
        return;
    }
    if (n->right != nullptr)
    {
        print_tree_helper(n->right, indent + INDENT_STEP);
    }
    for(i = 0; i < indent; i++)
        fputs(" ", stdout);
    if (n->color == BLACK)
        cout<<(int)n->key<<endl;
    else
        cout<<"<"<<(int)n->key<<">"<<endl;
    if (n->left != nullptr)
    {
        print_tree_helper(n->left, indent + INDENT_STEP);
    }
}

void print_tree(rbtree t)
{
    print_tree_helper(t->root, 0);
    puts("");
}

//#include <queue>
//
//using namespace std;
//
//template<class T>
//class TracingRbTree;
//
//enum Color {
//    RED, BLACK
//};
//
//template <class T>
//class TracingRbTreeNode {
//
//    friend class TracingRbTree<T>;
//
//    public:
//
//    T data;
//
//    private:
//
//    bool color;
//
//    TracingRbTreeNode* left;
//    TracingRbTreeNode* right;
//
//    public:
//    TracingRbTreeNode* parent;
//
//    private:
//
//    // Constructor
//    explicit TracingRbTreeNode(T data) {
//
//        this->data = data;
//
//        left = right = parent = nullptr;
//
//    }
//
//};
//
//template<class T>
//class TracingRbTree {
//
//    private:
//    TracingRbTreeNode<T>* root;
//
//    protected:
//
//    void rotateLeft(TracingRbTreeNode<T>*&, TracingRbTreeNode<T>*&);
//
//    void rotateRight(TracingRbTreeNode<T>*&, TracingRbTreeNode<T>*&);
//
//    void fixViolation(TracingRbTreeNode<T>*&, TracingRbTreeNode<T>*&);
//
//    public:
//    // Constructor
//    TracingRbTree() { root = nullptr; }
//
//    void insert(const T& data);
//
//    void inorder();
//
//    void levelOrder();
//
//    private:
//
//    // A recursive function to do level order traversal
//    static void inorderHelper(TracingRbTreeNode<T>* root) {
//
//        if (root->left != nullptr) {
//            inorderHelper(root->left);
//        }
//
//        cout << root->data << "  ";
//
//        if (root->right != nullptr) {
//            inorderHelper(root->right);
//        }
//
//    }
//
//    /* A utility function to insert a new node with given key
//   in BST */
//    static TracingRbTreeNode<T>* BSTInsert(TracingRbTreeNode<T>* root, TracingRbTreeNode<T>* pt) {
//        /* If the tree is empty, return a new node */
//        if (root == nullptr)
//            return pt;
//
//        /* Otherwise, recur down the tree */
//        if (pt->data < root->data) {
//            root->left = BSTInsert(root->left, pt);
//            root->left->parent = root;
//        }
//        else if (pt->data > root->data) {
//            root->right = BSTInsert(root->right, pt);
//            root->right->parent = root;
//        }
//
//        /* return the (unchanged) node pointer */
//        return root;
//    }
//
//    // Utility function to do level order traversal
//    static void levelOrderHelper(TracingRbTreeNode<T>* root) {
//
//        if (root == nullptr) {
//            return;
//        }
//
//        std::queue<TracingRbTreeNode<T>*> q;
//        q.push(root);
//
//        while (!q.empty()) {
//            TracingRbTreeNode<T>* temp = q.front();
//            cout << temp->data << "  ";
//            q.pop();
//
//            if (temp->left != nullptr) {
//                q.push(temp->left);
//            }
//
//            if (temp->right != nullptr) {
//                q.push(temp->right);
//            }
//
//        }
//
//    }
//
//};
//
//template <class T>
//void TracingRbTree<T>::rotateLeft(TracingRbTreeNode<T>*& root, TracingRbTreeNode<T>*& pt) {
//
//    TracingRbTreeNode<T>* pt_right = pt->right;
//
//    pt->right = pt_right->left;
//
//    if (pt->right != nullptr) {
//        pt->right->parent = pt;
//    }
//
//    pt_right->parent = pt->parent;
//
//    if (pt->parent == nullptr) {
//        root = pt_right;
//    }
//    else if (pt == pt->parent->left) {
//        pt->parent->left = pt_right;
//    }
//    else {
//        pt->parent->right = pt_right;
//    }
//
//    pt_right->left = pt;
//    pt->parent = pt_right;
//
//}
//
//template <class T>
//void TracingRbTree<T>::rotateRight(TracingRbTreeNode<T>*& root, TracingRbTreeNode<T>*& pt) {
//
//    TracingRbTreeNode<T>* pt_left = pt->left;
//
//    pt->left = pt_left->right;
//
//    if (pt->left != nullptr)
//        pt->left->parent = pt;
//
//    pt_left->parent = pt->parent;
//
//    if (pt->parent == nullptr)
//        root = pt_left;
//    else if (pt == pt->parent->left)
//        pt->parent->left = pt_left;
//    else
//        pt->parent->right = pt_left;
//
//    pt_left->right = pt;
//    pt->parent = pt_left;
//
//}
//
//template <class T>
//void TracingRbTree<T>::fixViolation(TracingRbTreeNode<T>*& root, TracingRbTreeNode<T>*& pt) {
//
//    TracingRbTreeNode<T>* parent_pt = nullptr;
//    TracingRbTreeNode<T>* grand_parent_pt = nullptr;
//
//    while ((pt != root) && (pt->color != BLACK) &&
//           (pt->parent->color == RED)) {
//
//        parent_pt = pt->parent;
//        grand_parent_pt = pt->parent->parent;
//
//        /*  Case : A
//            Parent of pt is left child of Grand-parent of pt */
//        if (parent_pt == grand_parent_pt->left) {
//
//            TracingRbTreeNode<T>* uncle_pt = grand_parent_pt->right;
//
//            /* Case : 1
//               The uncle of pt is also red
//               Only Recoloring required */
//
//            if (uncle_pt != nullptr && uncle_pt->color == RED) {
//
//                grand_parent_pt->color = RED;
//
//                parent_pt->color = BLACK;
//                uncle_pt->color = BLACK;
//
//                pt = grand_parent_pt;
//
//            }
//            else {
//
//                /* Case : 2
//                   pt is right child of its parent
//                   Left-rotation required */
//
//                if (pt == parent_pt->right) {
//                    rotateLeft(root, parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                   pt is left child of its parent
//                   Right-rotation required */
//                rotateRight(root, grand_parent_pt);
//                swap(parent_pt->color, grand_parent_pt->color);
//                pt = parent_pt;
//
//            }
//
//        }
//
//            /* Case : B
//               Parent of pt is right child of Grand-parent of pt */
//        else {
//            TracingRbTreeNode<T>* uncle_pt = grand_parent_pt->left;
//
//            /*  Case : 1
//                The uncle of pt is also red
//                Only Recoloring required */
//            if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
//                grand_parent_pt->color = RED;
//                parent_pt->color = BLACK;
//                uncle_pt->color = BLACK;
//                pt = grand_parent_pt;
//            }
//            else {
//                /* Case : 2
//                   pt is left child of its parent
//                   Right-rotation required */
//                if (pt == parent_pt->left) {
//                    rotateRight(root, parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                   pt is right child of its parent
//                   Left-rotation required */
//                rotateLeft(root, grand_parent_pt);
//                swap(parent_pt->color, grand_parent_pt->color);
//                pt = parent_pt;
//            }
//
//        }
//
//    }
//
//    root->color = BLACK;
//}
//
//// Function to insert a new node with given data
//template <class T>
//void TracingRbTree<T>::insert(const T& data) {
//
//    auto pt = new TracingRbTreeNode<T>(data);
//
//    // Do a normal BST insert
//    root = TracingRbTree<T>::BSTInsert(root, pt);
//
//    // fix Red Black Tree violations
//    fixViolation(root, pt);
//
//}
//
//// Function to do inorder and level order traversals
//template <class T>
//void TracingRbTree<T>::inorder() { inorderHelper(root); }
//
//template <class T>
//void TracingRbTree<T>::levelOrder() { levelOrderHelper(root); }

#endif //RBAVLTREES_RBTREE_HPP