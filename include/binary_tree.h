#include <iostream>
#include <string>

enum class Color { RED, BLACK };

template <class TKey, class TValue>
struct Node {
    TKey key;
    TValue value;
    Node<TKey, TValue>* left;
    Node<TKey, TValue>* right;
    Node<TKey, TValue>* parent;
    Color color;
};

template <class TKey, class TValue>
class BinaryTree {
public:
    Node<TKey, TValue>* NIL;

    BinaryTree() {
        NIL = new Node<TKey, TValue>{ TKey(), TValue(), nullptr, nullptr, nullptr, Color::BLACK };
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }
    BinaryTree(const BinaryTree& other) {
        NIL = new Node<TKey, TValue>{ TKey(), TValue(), nullptr, nullptr, nullptr, Color::BLACK };
        NIL->left = NIL->right = NIL->parent = NIL;

        root = copyTree(other.root, other.NIL, NIL);
    }
    bool empty() const {
        return root == NIL; 
    }
    ~BinaryTree() {
        clear(root);
        delete NIL;
    }

    Node<TKey, TValue>* getRoot() const {
        return root;
    }

    std::string nodeColor(Node<TKey, TValue>* node) {
        if (!node) return "nil";
        return node->color == Color::RED ? "red" : "black";
    }

    void printTree(Node<TKey, TValue>* node, int indent = 0) {
        if (node == NIL) return;
        printTree(node->right, indent + 4);
        std::cout << std::string(indent, ' ')
            << "[" << node->key << ":" << (node->color == Color::RED ? "R" : "B") << "]\n";
        printTree(node->left, indent + 4);
    }

    BinaryTree& operator=(const BinaryTree& other) {
        if (this == &other)
            return *this;
        clear(root);
        root = copyTree(other.root, other.NIL, nullptr);
        return *this;
    }

    void rotateLeft(Node<TKey, TValue>* x) {
        Node<TKey, TValue>* y = x->right;

        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node<TKey, TValue>* y) {
        Node<TKey, TValue>* x = y->left;

        y->left = x->right;
        if (x->right != NIL) x->right->parent = y;

        x->parent = y->parent;
        if (y->parent == NIL) {
            root = x;
        }
        else if (y == y->parent->right) {
            y->parent->right = x;
        }
        else {
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }

    void balanceInsert(Node<TKey, TValue>* x) {
        // Случай 1: x — корень
        if (x == root) {
            x->color = Color::BLACK;
            return;
        }

        Node<TKey, TValue>* parent = P(x);
        Node<TKey, TValue>* grandparent = G(x);
        Node<TKey, TValue>* uncle = U(x);

        // Случай 2: родитель черный — ничего не делаем
        if (parent->color == Color::BLACK) {
            return;
        }

        // Случай 3: дядя красный
        if (uncle->color == Color::RED) {
            parent->color = Color::BLACK;
            uncle->color = Color::BLACK;
            grandparent->color = Color::RED;
            balanceInsert(grandparent); // Рекурсивная обработка деда
            return;
        }

        // Случай 4: дядя черный (повороты)
        if (parent == grandparent->left) {
            // Подслучай 4.1: x — правый потомок (LR-случай)
            if (x == parent->right) {
                rotateLeft(parent);
                parent = x; // После поворота x становится новым родителем
            }
            // Подслучай 4.1.1: x — левый потомок (LL-случай)
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            rotateRight(grandparent);
        }
        else {
            // Подслучай 4.2: x — левый потомок (RL-случай)
            if (x == parent->left) {
                rotateRight(parent);
                parent = x; // После поворота x становится новым родителем
            }
            // Подслучай 4.2.1: x — правый потомок (RR-случай)
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            rotateLeft(grandparent);
        }
    }

    void balanceDelete(Node<TKey, TValue>* x) {
        // Случай 1: x - корень
        if (x == root) return;

        Node<TKey, TValue>* sibling = S(x);
        Node<TKey, TValue>* parent = P(x);

        // Случай 2: брат красный
        if (sibling->color == Color::RED) {
            parent->color = Color::RED;
            sibling->color = Color::BLACK;
            if (x == parent->left) {
                rotateLeft(parent);
                sibling = parent->right; // Обновляем брата после поворота
            }
            else {
                rotateRight(parent);
                sibling = parent->left; // Обновляем брата после поворота
            }
        }

        // Случай 3: все узлы черные
        if (sibling->color == Color::BLACK &&
            sibling->left->color == Color::BLACK &&
            sibling->right->color == Color::BLACK)
        {
            sibling->color = Color::RED;
            if (parent->color == Color::BLACK) {
                balanceDelete(parent); // Рекурсивный вызов
            }
            else {
                parent->color = Color::BLACK;
            }
            return;
        }

        // Случай 4: брат черный, родитель красный, дети брата черные
        if (sibling->color == Color::BLACK &&
            parent->color == Color::RED &&
            sibling->left->color == Color::BLACK &&
            sibling->right->color == Color::BLACK)
        {
            sibling->color = Color::RED;
            parent->color = Color::BLACK;
            return;
        }

        // Случай 5: подготовка к финальному повороту
        if (x == parent->left) {
            if (sibling->right->color == Color::BLACK) {
                sibling->left->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateRight(sibling);
                sibling = parent->right;
            }
        }
        else {
            if (sibling->left->color == Color::BLACK) {
                sibling->right->color = Color::BLACK;
                sibling->color = Color::RED;
                rotateLeft(sibling);
                sibling = parent->left;
            }
        }

        // Финальные перекрашивания и поворот
        sibling->color = parent->color;
        parent->color = Color::BLACK;
        if (x == parent->left) {
            sibling->right->color = Color::BLACK;
            rotateLeft(parent);
        }
        else {
            sibling->left->color = Color::BLACK;
            rotateRight(parent);
        }
    }


    void insert(TKey key, TValue value) {
        Node<TKey, TValue>* newNode = createNode(key, value);
        Node<TKey, TValue>* y = NIL;
        Node<TKey, TValue>* x = root;

        while (x != NIL) {
            y = x;
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else {
                delete newNode;
                x->value = value;
                return;
            }
        }

        newNode->parent = y;
        if (y == NIL) {
            root = newNode;
        }
        else if (key < y->key) {
            y->left = newNode;
        }
        else {
            y->right = newNode;
        }

        balanceInsert(newNode);
    }

    void remove(TKey key) {
        Node<TKey, TValue>* z = searchRecursive(root, key);
        if (z == NIL) return;

        Node<TKey, TValue>* y = z;
        Node<TKey, TValue>* x;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == Color::BLACK)
            balanceDelete(x);
        if (root->color != Color::BLACK)
            root->color = Color::BLACK;
    }

    TValue* search(TKey key) {
        Node<TKey, TValue>* result = searchRecursive(root, key);
        return result != NIL ? &(result->value) : nullptr;
    }

    void printInOrder() {
        inOrderTraversal(root);
        std::cout << "\n";
    }

private:
    Node<TKey, TValue>* root;

    Node<TKey, TValue>* S(Node<TKey, TValue>* x) {
        if (x == nullptr || x->parent == nullptr)
            return nullptr;

        return (x == x->parent->left)
            ? x->parent->right
            : x->parent->left;
    }
    Node<TKey, TValue>* P(Node<TKey, TValue>* x) { return x->parent; }
    Node<TKey, TValue>* G(Node<TKey, TValue>* x) {
        return (x != nullptr && x->parent != nullptr) ? x->parent->parent : nullptr;
    }
    Node<TKey, TValue>* U(Node<TKey, TValue>* x) {
        Node<TKey, TValue>* grandparent = G(x);
        if (grandparent == nullptr) return nullptr;
        return (x->parent == grandparent->left) ? grandparent->right : grandparent->left;
    }

    void clear(Node<TKey, TValue>* node) {
        if (node != NIL) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node<TKey, TValue>* copyTree(const Node<TKey, TValue>* srcNode, Node<TKey, TValue>* srcNil, Node<TKey, TValue>* parent) {
        if (srcNode == srcNil)
            return NIL; 

        Node<TKey, TValue>* newNode = new Node<TKey, TValue>{
            srcNode->key,
            srcNode->value,
            NIL, 
            NIL, 
            parent,
            srcNode->color
        };

      
        newNode->left = copyTree(srcNode->left, srcNil, newNode);
        newNode->right = copyTree(srcNode->right, srcNil, newNode);

        return newNode;
    }

    Node<TKey, TValue>* createNode(TKey key, TValue value) {
        Node<TKey, TValue>* node = new Node<TKey, TValue>{ key, value, NIL, NIL, NIL, Color::RED };
        return node;
    }

    Node<TKey, TValue>* searchRecursive(Node<TKey, TValue>* node, TKey key) {
        if (node == NIL || key == node->key) return node;
        return key < node->key ? searchRecursive(node->left, key) : searchRecursive(node->right, key);
    }

    Node<TKey, TValue>* minValueNode(Node<TKey, TValue>* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    void transplant(Node<TKey, TValue>* u, Node<TKey, TValue>* v) {
        if (u->parent == NIL) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v != NIL) {
            v->parent = u->parent;
        }
    }

    void inOrderTraversal(Node<TKey, TValue>* node) {
        if (node != NIL) {
            inOrderTraversal(node->left);
            std::cout << "[" << node->key << ": " << node->value << "] ";
            inOrderTraversal(node->right);
        }
    }
};