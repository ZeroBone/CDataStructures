#ifndef RBAVLTREES_RBTREE_HPP
#define RBAVLTREES_RBTREE_HPP

using namespace std;

template <class T>
class TracingRbTree;

template <class T>
class TracingRbTreeNode {

    friend class TracingRbTree<T>;

    public:

    T key;

    TracingRbTreeNode* parent;

    private:

    char color;

    TracingRbTreeNode* left;
    TracingRbTreeNode* right;


};

template <class T>
class TracingRbTree {

    TracingRbTreeNode<T>* root;

    TracingRbTreeNode<T>* q;

    public:

    TracingRbTree() {
        q = nullptr;
        root = nullptr;
    }

    size_t getNodeSize() const {
        return sizeof(TracingRbTreeNode<T>);
    }

    void insert(T key);

    bool remove(T key);

    TracingRbTreeNode<T>* search(T key);

    void disp();

    private:

    void insertfix(TracingRbTreeNode<T>*);

    void leftrotate(TracingRbTreeNode<T>*);

    void rightrotate(TracingRbTreeNode<T>*);

    TracingRbTreeNode<T>* successor(TracingRbTreeNode<T>*);

    void delfix(TracingRbTreeNode<T>*);

    void display(TracingRbTreeNode<T>*);

};

template <class T>
void TracingRbTree<T>::insert(T key) {

    TracingRbTreeNode<T>* p, *q;

    auto t = new TracingRbTreeNode<T>();

    t->key = key;

    t->left = nullptr;
    t->right = nullptr;
    t->color = 'r';

    p = root;
    q = nullptr;

    if (root == nullptr) {
        root = t;
        t->parent = nullptr;
    }
    else {

        while (p != nullptr) {

            q = p;

            if (p->key < t->key)
                p = p->right;
            else
                p = p->left;
        }

        t->parent = q;

        if (q->key < t->key)
            q->right = t;
        else
            q->left = t;

    }

    insertfix(t);

}

template <class T>
void TracingRbTree<T>::insertfix(TracingRbTreeNode<T>* t) {

    TracingRbTreeNode<T>* u;

    if (root == t) {
        t->color = 'b';
        return;
    }

    while (t->parent != nullptr && t->parent->color == 'r') {

        TracingRbTreeNode<T>* g = t->parent->parent;

        if (g->left == t->parent) {

            if (g->right != nullptr) {

                u = g->right;

                if (u->color == 'r') {
                    t->parent->color = 'b';
                    u->color = 'b';
                    g->color = 'r';
                    t = g;
                }

            }
            else {

                if (t->parent->right == t) {

                    t = t->parent;
                    leftrotate(t);

                }

                t->parent->color = 'b';
                g->color = 'r';

                rightrotate(g);

            }

        }
        else {

            if (g->left != nullptr) {

                u = g->left;
                if (u->color == 'r') {
                    t->parent->color = 'b';
                    u->color = 'b';
                    g->color = 'r';
                    t = g;
                }
            }
            else {

                if (t->parent->left == t) {
                    t = t->parent;
                    rightrotate(t);
                }

                t->parent->color = 'b';
                g->color = 'r';

                leftrotate(g);

            }

        }

        root->color = 'b';

    }

}

template <class T>
bool TracingRbTree<T>::remove(T key) {

    if (root == nullptr) {
        // empty tree
        return false;
    }

    TracingRbTreeNode<T>* p;

    p = root;

    TracingRbTreeNode<T>* y = nullptr;
    TracingRbTreeNode<T>* q = nullptr;

    while (p != nullptr) {

        if (p->key == key) {

            // delete p

            if (p->left == nullptr || p->right == nullptr) {
                y = p;
            }
            else {
                y = successor(p);
            }

            if (y->left != nullptr) {
                q = y->left;
            }
            else {
                if (y->right != nullptr) {
                    q = y->right;
                }
                else {
                    q = nullptr;
                }
            }

            if (q != nullptr) {
                q->parent = y->parent;
            }

            if (y->parent == nullptr) {
                root = q;
            }
            else {
                if (y == y->parent->left) {
                    y->parent->left = q;
                }
                else {
                    y->parent->right = q;
                }
            }

            if (y != p) {
                p->color = y->color;
                p->key = y->key;
            }

            if (y->color == 'b') {
                delfix(q);
            }

            return true;

        }

        if (p->key < key) {
            p = p->right;
        }
        else {
            p = p->left;
        }

    }

    return false;

}

template <class T>
void TracingRbTree<T>::delfix(TracingRbTreeNode<T>* p) {

    TracingRbTreeNode<T>* s;

    while (p != root && p->color == 'b') {
        if (p->parent->left == p) {
            s = p->parent->right;
            if (s->color == 'r') {
                s->color = 'b';
                p->parent->color = 'r';
                leftrotate(p->parent);
                s = p->parent->right;
            }
            if (s->right->color == 'b' && s->left->color == 'b') {
                s->color = 'r';
                p = p->parent;
            }
            else {
                if (s->right->color == 'b') {
                    s->left->color == 'b';
                    s->color = 'r';
                    rightrotate(s);
                    s = p->parent->right;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->right->color = 'b';
                leftrotate(p->parent);
                p = root;
            }
        }
        else {
            s = p->parent->left;
            if (s->color == 'r') {
                s->color = 'b';
                p->parent->color = 'r';
                rightrotate(p->parent);
                s = p->parent->left;
            }
            if (s->left->color == 'b' && s->right->color == 'b') {
                s->color = 'r';
                p = p->parent;
            }
            else {
                if (s->left->color == 'b') {
                    s->right->color = 'b';
                    s->color = 'r';
                    leftrotate(s);
                    s = p->parent->left;
                }
                s->color = p->parent->color;
                p->parent->color = 'b';
                s->left->color = 'b';
                rightrotate(p->parent);
                p = root;
            }
        }
        p->color = 'b';
        root->color = 'b';
    }
}

template <class T>
void TracingRbTree<T>::leftrotate(TracingRbTreeNode<T>* p) {
    if (p->right == nullptr)
        return;
    else {
        TracingRbTreeNode<T>* y = p->right;
        if (y->left != nullptr) {
            p->right = y->left;
            y->left->parent = p;
        }
        else
            p->right = nullptr;
        if (p->parent != nullptr)
            y->parent = p->parent;
        if (p->parent == nullptr)
            root = y;
        else {
            if (p == p->parent->left)
                p->parent->left = y;
            else
                p->parent->right = y;
        }
        y->left = p;
        p->parent = y;
    }
}

template <class T>
void TracingRbTree<T>::rightrotate(TracingRbTreeNode<T>* p) {
    if (p->left == nullptr)
        return;
    else {
        TracingRbTreeNode<T>* y = p->left;
        if (y->right != nullptr) {
            p->left = y->right;
            y->right->parent = p;
        }
        else
            p->left = nullptr;
        if (p->parent != nullptr)
            y->parent = p->parent;
        if (p->parent == nullptr)
            root = y;
        else {
            if (p == p->parent->left)
                p->parent->left = y;
            else
                p->parent->right = y;
        }
        y->right = p;
        p->parent = y;
    }
}

template <class T>
TracingRbTreeNode<T>* TracingRbTree<T>::successor(TracingRbTreeNode<T>* p) {
    TracingRbTreeNode<T>* y = nullptr;
    if (p->left != nullptr) {
        y = p->left;
        while (y->right != nullptr)
            y = y->right;
    }
    else {
        y = p->right;
        while (y->left != nullptr)
            y = y->left;
    }
    return y;
}

template <class T>
void TracingRbTree<T>::disp() {
    display(root);
}

template <class T>
void TracingRbTree<T>::display(TracingRbTreeNode<T>* p) {
    if (root == nullptr) {
        cout << "\nEmpty Tree.";
        return;
    }
    if (p != nullptr) {
        cout << "\n\t NODE: ";
        cout << "\n Key: " << p->key;
        cout << "\n Colour: ";
        if (p->color == 'b')
            cout << "Black";
        else
            cout << "Red";
        if (p->parent != nullptr)
            cout << "\n Parent: " << p->parent->key;
        else
            cout << "\n There is no parent of the node.  ";
        if (p->right != nullptr)
            cout << "\n Right Child: " << p->right->key;
        else
            cout << "\n There is no right child of the node.  ";
        if (p->left != nullptr)
            cout << "\n Left Child: " << p->left->key;
        else
            cout << "\n There is no left child of the node.  ";
        cout << endl;
        if (p->left) {
            cout << "\n\nLeft:\n";
            display(p->left);
        }
        /*else
         cout<<"\nNo Left Child.\n";*/
        if (p->right) {
            cout << "\n\nRight:\n";
            display(p->right);
        }
        /*else
         cout<<"\nNo Right Child.\n"*/
    }
}

template <class T>
TracingRbTreeNode<T>* TracingRbTree<T>::search(T key) {

    if (root == nullptr) {
        // empty tree
        return nullptr;
    }

    TracingRbTreeNode<T>* p = root;

    while (p != nullptr) {

        if (p->key == key) {
            return p;
        }

        if (p->key < key) {
            p = p->right;
        }
        else {
            p = p->left;
        }

    }

    return nullptr;

}

#endif //RBAVLTREES_RBTREE_HPP