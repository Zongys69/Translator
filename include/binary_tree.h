#include <iostream>
#include <string>

template <class TKey, class TValue>
struct Node {
    TKey key;
    TValue value;
    Node<TKey, TValue>* left;
    Node<TKey, TValue>* right;
    
};

template <class TKey, class TValue>
class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        clear(root);
    }
    BinaryTree& operator=(const BinaryTree& other) {
        if (this == &other)
            return *this;

        clear(root);  
        root = copyTree(other.root);  
        return *this;
    }

    BinaryTree(const BinaryTree& other) {
        root = copyTree(other.root);
    }

    void insert(TKey key, TValue value) {
        root = insertRecursive(root, key, value);
    }

    void remove(TKey key) {
        root = deleteRecursive(root, key);
    }
    
    TValue* search(TKey key) {
        Node<TKey, TValue>* result = searchRecursive(root, key);
        if (result != nullptr)
            return &(result->value);

        else
            return nullptr;
    }

    void printInOrder() {
        inOrderTraversal(root);
        std::cout << "\n";
    }
 
private:
    Node<TKey, TValue>* root;

    void clear(Node<TKey, TValue>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node<TKey, TValue>* copyTree(const Node<TKey, TValue>* node) {
        if (!node) return nullptr;
        Node<TKey, TValue>* newNode = createNode(node->key, node->value);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    Node<TKey, TValue>* createNode(TKey key, TValue value) {
        return new Node<TKey, TValue>{ key, value, nullptr, nullptr };
    }

    Node<TKey, TValue>* insertRecursive(Node<TKey, TValue>* node, TKey key, TValue value) {
        if (node == nullptr)
            return createNode(key, value);

        if (key < node->key) {
            node->left = insertRecursive(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insertRecursive(node->right, key, value);
        }
        else {
            std::cerr << "Key " << key << " already exists. Updating value.\n";
            node->value = value;
        }
        return node;
    }

    Node<TKey, TValue>* searchRecursive(Node<TKey, TValue>* node, TKey key) {
        if (node == nullptr || node->key == key) 
            return node;
        
        if (node->key < key) 
            return searchRecursive(node->right, key);
        
          return searchRecursive(node->left, key);
    
    }

    Node<TKey, TValue>* deleteRecursive(Node<TKey, TValue>* node, TKey key) {
        if (node == nullptr)
            return node;

        if (key < node->key) {
            node->left = deleteRecursive(node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteRecursive(node->right, key);
        }
        else {
            if (node->left == nullptr) {
                Node<TKey, TValue>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node<TKey, TValue>* temp = node->left;
                delete node;
                return temp;
            }

            Node<TKey, TValue>* successor = minValueNode(node->right);
            if (!successor) return node;

            TKey succKey = successor->key;
            TValue succValue = successor->value;

            node->key = succKey;
            node->value = succValue;

            node->right = deleteRecursive(node->right, succKey);
        }
        return node;
    }

    Node<TKey, TValue>* minValueNode(Node<TKey, TValue>* node) {
        if (node == nullptr)
            return nullptr;
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void inOrderTraversal(Node<TKey, TValue>* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            std::cout << "[" << node->key << ": " << node->value << "] ";
            inOrderTraversal(node->right);
        }
    }
};
