#include "gtest.h"
#include "tree.h"
#include <string>

#include <random>
#include <cmath>
#include <set>


// Тест на пустом дереве
TEST(AvlTree, EmptyTree) {
    Search_Tree<int, std::string> tree;
    ASSERT_EQ(tree.get_height(), 0);
    ASSERT_EQ(tree.find(10), nullptr);
    ASSERT_NO_THROW(tree.print());
    ASSERT_NO_THROW(tree.erase(10));
    ASSERT_EQ(tree.get_height(), 0);
}

// Тест на вставку одного элемента
TEST(AvlTree, InsertSingle) {
    Search_Tree<int, std::string> tree;
    tree.insert(10, "Apple");
    ASSERT_EQ(tree.get_height(), 1);
    Node<int, std::string>* node = tree.find(10);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->key, 10);
    ASSERT_EQ(node->value, "Apple");
    ASSERT_EQ(node->height, 1);
    ASSERT_EQ(tree.find(5), nullptr);
}

// Тест на базовую вставку без вращений
TEST(AvlTree, InsertMultipleNoRotation) {
    Search_Tree<int, std::string> tree;
    tree.insert(10, "Ten");
    tree.insert(5, "Five");
    tree.insert(15, "Fifteen");

    ASSERT_EQ(tree.get_height(), 2);
    Node<int, std::string>* root = tree.find(10);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->height, 2);

    Node<int, std::string>* left = tree.find(5);
    ASSERT_NE(left, nullptr);
    ASSERT_EQ(left->height, 1);

    Node<int, std::string>* right = tree.find(15);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->height, 1);

    ASSERT_EQ(root->left, left);
    ASSERT_EQ(root->right, right);
}

// Тест на обновление значения при вставке дубликата
TEST(AvlTree, InsertDuplicateKey) {
    Search_Tree<int, std::string> tree;
    tree.insert(10, "First");
    ASSERT_EQ(tree.get_height(), 1);
    Node<int, std::string>* node1 = tree.find(10);
    ASSERT_NE(node1, nullptr);
    ASSERT_EQ(node1->value, "First");

    tree.insert(10, "Second");
    ASSERT_EQ(tree.get_height(), 1);
    Node<int, std::string>* node2 = tree.find(10);
    ASSERT_NE(node2, nullptr);
    ASSERT_EQ(node2->value, "Second");
    ASSERT_EQ(node1, node2);
}


// Тест на малое левое вращение при вставке
TEST(AvlTree, InsertTriggerRR) { // Малое левое вращение
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(20, 20);
    tree.insert(30, 30);

    ASSERT_EQ(tree.get_height(), 2);
    Node<int, int>* root = tree.find(20);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->height, 2);

    Node<int, int>* left = tree.find(10);
    ASSERT_NE(left, nullptr);
    ASSERT_EQ(left->height, 1);

    Node<int, int>* right = tree.find(30);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->height, 1);

    ASSERT_EQ(root->left, left);
    ASSERT_EQ(root->right, right);
}

// Тест на малое правое вращение при вставке
TEST(AvlTree, InsertTriggerLL) { // Малое правое вращение
    Search_Tree<int, int> tree;
    tree.insert(30, 30);
    tree.insert(20, 20);
    tree.insert(10, 10);

    ASSERT_EQ(tree.get_height(), 2);
    Node<int, int>* root = tree.find(20);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->height, 2);

    Node<int, int>* left = tree.find(10);
    ASSERT_NE(left, nullptr);
    ASSERT_EQ(left->height, 1);

    Node<int, int>* right = tree.find(30);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->height, 1);

    ASSERT_EQ(root->left, left);
    ASSERT_EQ(root->right, right);
}

// Тест на большое правое вращение при вставке
TEST(AvlTree, InsertTriggerLR) { // Большое правое вращение
    Search_Tree<int, int> tree;
    tree.insert(30, 30);
    tree.insert(10, 10);
    tree.insert(20, 20);


    ASSERT_EQ(tree.get_height(), 2);
    Node<int, int>* root = tree.find(20);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->height, 2);

    Node<int, int>* left = tree.find(10);
    ASSERT_NE(left, nullptr);
    ASSERT_EQ(left->height, 1);

    Node<int, int>* right = tree.find(30);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->height, 1);

    ASSERT_EQ(root->left, left);
    ASSERT_EQ(root->right, right);
}

// Тест на большое левое вращение при вставке
TEST(AvlTree, InsertTriggerRL) { // Большое левое вращение
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(30, 30);
    tree.insert(20, 20);

    ASSERT_EQ(tree.get_height(), 2);
    Node<int, int>* root = tree.find(20);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->height, 2);

    Node<int, int>* left = tree.find(10);
    ASSERT_NE(left, nullptr);
    ASSERT_EQ(left->height, 1);

    Node<int, int>* right = tree.find(30);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(right->height, 1);

    ASSERT_EQ(root->left, left);
    ASSERT_EQ(root->right, right);
}


// Тест на удаление несуществующего элемента
TEST(AvlTree, EraseNonExisting) {
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(5, 5);
    int initialHeight = tree.get_height();

    tree.erase(100);
    ASSERT_EQ(tree.get_height(), initialHeight);
    ASSERT_NE(tree.find(10), nullptr);
    ASSERT_NE(tree.find(5), nullptr);
}

// Тест на удаление листа
TEST(AvlTree, EraseLeaf) {
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(5, 5);
    ASSERT_NE(tree.find(5), nullptr);
    ASSERT_EQ(tree.get_height(), 2);

    tree.erase(5);
    ASSERT_EQ(tree.find(5), nullptr);
    ASSERT_NE(tree.find(10), nullptr);
    ASSERT_EQ(tree.get_height(), 1);

    tree.erase(10);
    ASSERT_EQ(tree.find(10), nullptr);
    ASSERT_EQ(tree.get_height(), 0);
}

// Тест на удаление листа (сложный случай после LR-вращения)
TEST(AvlTree, EraseLeafNode_WasRightChildBeforeBalancing) {
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(5, 5);
    tree.insert(7, 7);
    ASSERT_EQ(tree.get_height(), 2);

    tree.erase(5);
    ASSERT_EQ(tree.find(5), nullptr);

    Node<int, int>* currentRoot = tree.find(7);
    ASSERT_NE(currentRoot, nullptr);
    ASSERT_EQ(currentRoot->key, 7);

    Node<int, int>* rightChild = tree.find(10);
    ASSERT_NE(rightChild, nullptr);
    ASSERT_EQ(rightChild->key, 10);

    ASSERT_EQ(currentRoot->left, nullptr);
    ASSERT_EQ(currentRoot->right, rightChild);
    ASSERT_EQ(rightChild->left, nullptr);
    ASSERT_EQ(rightChild->right, nullptr);

    ASSERT_EQ(tree.get_height(), 2);
}

// Тест на удаление листа (сложный случай после RL-вращения)
TEST(AvlTree, EraseLeafNode_WasLeftChildBeforeBalancing) {
    Search_Tree<int, int> tree;
    tree.insert(10, 10);
    tree.insert(15, 15);
    tree.insert(12, 12);
    ASSERT_EQ(tree.get_height(), 2);

    tree.erase(15);
    ASSERT_EQ(tree.find(15), nullptr);

    Node<int, int>* currentRoot = tree.find(12);
    ASSERT_NE(currentRoot, nullptr);
    ASSERT_EQ(currentRoot->key, 12);

    Node<int, int>* leftChild = tree.find(10);
    ASSERT_NE(leftChild, nullptr);
    ASSERT_EQ(leftChild->key, 10);

    ASSERT_EQ(currentRoot->left, leftChild);
    ASSERT_EQ(currentRoot->right, nullptr);
    ASSERT_EQ(leftChild->left, nullptr);
    ASSERT_EQ(leftChild->right, nullptr);

    ASSERT_EQ(tree.get_height(), 2);
}


// Тест на удаление узла с двумя потомками (корень)
TEST(AvlTree, EraseNodeWithTwoChildren_Root) {
    Search_Tree<int, int> tree;
    tree.insert(20, 20);
    tree.insert(10, 10);
    tree.insert(30, 30);
    tree.insert(5, 5);
    tree.insert(15, 15);
    tree.insert(25, 25);
    tree.insert(35, 35);
    ASSERT_EQ(tree.get_height(), 3);

    tree.erase(20);
    ASSERT_EQ(tree.find(20), nullptr);

    Node<int, int>* newRoot = tree.find(15);
    ASSERT_NE(newRoot, nullptr);
    ASSERT_EQ(newRoot->key, 15);
    ASSERT_EQ(newRoot->value, 15);

    Node<int, int>* node10 = tree.find(10);
    ASSERT_NE(node10, nullptr);
    ASSERT_EQ(node10->key, 10);
    ASSERT_EQ(newRoot->left, node10);

    Node<int, int>* node5 = tree.find(5);
    ASSERT_NE(node5, nullptr);
    ASSERT_EQ(node5->key, 5);
    ASSERT_EQ(node10->left, node5);
    ASSERT_EQ(node10->right, nullptr);

    Node<int, int>* node30 = tree.find(30);
    ASSERT_NE(node30, nullptr);
    ASSERT_EQ(node30->key, 30);
    ASSERT_EQ(newRoot->right, node30);

    Node<int, int>* node25 = tree.find(25);
    ASSERT_NE(node25, nullptr);
    ASSERT_EQ(node25->key, 25);
    ASSERT_EQ(node30->left, node25);

    Node<int, int>* node35 = tree.find(35);
    ASSERT_NE(node35, nullptr);
    ASSERT_EQ(node35->key, 35);
    ASSERT_EQ(node30->right, node35);

    ASSERT_EQ(tree.get_height(), 3);
}


// Тест: Удаление триггерит LL-вращение (правое вращение)
TEST(AvlTree, EraseTriggerLL) {
    Search_Tree<int, std::string> tree;
    tree.insert(40, "Forty");
    tree.insert(20, "Twenty");
    tree.insert(50, "Fifty");
    tree.insert(10, "Ten");
    tree.insert(30, "Thirty");
    ASSERT_EQ(tree.get_height(), 3);

    tree.erase(50);

    int finalHeight = tree.get_height();
    ASSERT_EQ(finalHeight, 3);

    ASSERT_EQ(tree.find(50), nullptr);
    Node<int, std::string>* root = tree.find(20);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->key, 20);
    ASSERT_EQ(root->height, 3);

    Node<int, std::string>* node10 = tree.find(10);
    ASSERT_NE(node10, nullptr);
    ASSERT_EQ(root->left, node10);
    ASSERT_EQ(node10->height, 1);

    Node<int, std::string>* node40 = tree.find(40);
    ASSERT_NE(node40, nullptr);
    ASSERT_EQ(root->right, node40);
    ASSERT_EQ(node40->height, 2);

    Node<int, std::string>* node30 = tree.find(30);
    ASSERT_NE(node30, nullptr);
    ASSERT_EQ(node40->left, node30);
    ASSERT_EQ(node40->right, nullptr);
    ASSERT_EQ(node30->height, 1);
}

// Тест: Удаление триггерит RR-вращение (левое вращение)
TEST(AvlTree, EraseTriggerRR) {
    Search_Tree<int, std::string> tree;
    tree.insert(20, "Twenty");
    tree.insert(10, "Ten");
    tree.insert(40, "Forty");
    tree.insert(30, "Thirty");
    tree.insert(50, "Fifty");
    ASSERT_EQ(tree.get_height(), 3);

    tree.erase(10);

    int finalHeight = tree.get_height();
    ASSERT_EQ(finalHeight, 3);

    ASSERT_EQ(tree.find(10), nullptr);
    Node<int, std::string>* root = tree.find(40);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->key, 40);
    ASSERT_EQ(root->height, 3);

    Node<int, std::string>* node20 = tree.find(20);
    ASSERT_NE(node20, nullptr);
    ASSERT_EQ(root->left, node20);
    ASSERT_EQ(node20->height, 2);
    ASSERT_EQ(node20->left, nullptr);

    Node<int, std::string>* node50 = tree.find(50);
    ASSERT_NE(node50, nullptr);
    ASSERT_EQ(root->right, node50);
    ASSERT_EQ(node50->height, 1);

    Node<int, std::string>* node30 = tree.find(30);
    ASSERT_NE(node30, nullptr);
    ASSERT_EQ(node20->right, node30);
    ASSERT_EQ(node30->height, 1);
}


// Тест: Удаление триггерит LR-вращение (левое, затем правое)
TEST(AvlTree, EraseTriggerLR) {
    Search_Tree<int, std::string> tree;
    tree.insert(50, "Fifty");
    tree.insert(20, "Twenty");
    tree.insert(60, "Sixty");
    tree.insert(30, "Thirty");
    ASSERT_EQ(tree.get_height(), 3);
    ASSERT_EQ(tree.find(20)->height, 2);
    ASSERT_EQ(tree.find(50)->height, 3);

    tree.erase(60);

    int finalHeight = tree.get_height();
    ASSERT_EQ(finalHeight, 2);

    ASSERT_EQ(tree.find(60), nullptr);
    Node<int, std::string>* root = tree.find(30);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->key, 30);
    ASSERT_EQ(root->height, 2);

    Node<int, std::string>* node20 = tree.find(20);
    ASSERT_NE(node20, nullptr);
    ASSERT_EQ(root->left, node20);
    ASSERT_EQ(node20->height, 1);
    ASSERT_EQ(node20->left, nullptr);
    ASSERT_EQ(node20->right, nullptr);

    Node<int, std::string>* node50 = tree.find(50);
    ASSERT_NE(node50, nullptr);
    ASSERT_EQ(root->right, node50);
    ASSERT_EQ(node50->height, 1);
    ASSERT_EQ(node50->left, nullptr);
    ASSERT_EQ(node50->right, nullptr);
}

// Тест: Удаление триггерит RL-вращение (правое, затем левое)
TEST(AvlTree, EraseTriggerRL) {
    Search_Tree<int, std::string> tree;
    tree.insert(20, "Twenty");
    tree.insert(10, "Ten");
    tree.insert(40, "Forty");
    tree.insert(30, "Thirty");
    ASSERT_EQ(tree.get_height(), 3);
    ASSERT_EQ(tree.find(40)->height, 2);
    ASSERT_EQ(tree.find(20)->height, 3);

    tree.erase(10);

    int finalHeight = tree.get_height();
    ASSERT_EQ(finalHeight, 2);

    ASSERT_EQ(tree.find(10), nullptr);
    Node<int, std::string>* root = tree.find(30);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->key, 30);
    ASSERT_EQ(root->height, 2);

    Node<int, std::string>* node20 = tree.find(20);
    ASSERT_NE(node20, nullptr);
    ASSERT_EQ(root->left, node20);
    ASSERT_EQ(node20->height, 1);
    ASSERT_EQ(node20->left, nullptr);
    ASSERT_EQ(node20->right, nullptr);

    Node<int, std::string>* node40 = tree.find(40);
    ASSERT_NE(node40, nullptr);
    ASSERT_EQ(root->right, node40);
    ASSERT_EQ(node40->height, 1);
    ASSERT_EQ(node40->left, nullptr);
    ASSERT_EQ(node40->right, nullptr);
}

// Тест на вставку большого количества последовательных элементов
TEST(AvlTree, StressInsertSequential) {
    Search_Tree<int, int> tree;
    const int count = 1000;
    for (int i = 0; i < count; ++i) {
        tree.insert(i, i);
    }

    int height = tree.get_height();
    int maxExpectedHeight = static_cast<int>(1.44 * log2(count + 2)) + 1;
    ASSERT_GT(height, 0);
    ASSERT_LT(height, count);
    ASSERT_LE(height, maxExpectedHeight);

    ASSERT_NE(tree.find(0), nullptr);
    ASSERT_NE(tree.find(count / 2), nullptr);
    ASSERT_NE(tree.find(count - 1), nullptr);
}

// Тест на вставку и удаление большого количества случайных элементов
TEST(AvlTree, StressInsertDeleteRandom) {
    Search_Tree<int, int> tree;
    const int count = 2000;
    std::vector<int> keys;
    keys.reserve(count);
    std::set<int> keys_set;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, count * 10);

    for (int i = 0; i < count; ++i) {
        int key = dist(rng);
        if (keys_set.find(key) == keys_set.end()) {
            keys.push_back(key);
            keys_set.insert(key);
            tree.insert(key, key * 2);
        }
    }

    std::shuffle(keys.begin(), keys.end(), rng);

    for (int key : keys) {
        ASSERT_NE(tree.find(key), nullptr);
        tree.erase(key);
        ASSERT_EQ(tree.find(key), nullptr);
    }

    ASSERT_EQ(tree.get_height(), 0);
}

//тест с удалением корня
TEST(AvlTree, CreateVerifyDeleteRootVerify) {
    // 1. Создание дерева
    Search_Tree<int, int> tree;

    // 2. Вставка элементов для создания нужной структуры
    // Корень 50, левое поддерево высота 1 (узел 25),
    // правое поддерево (60 L:55 R:75)
    tree.insert(50, 500);
    tree.insert(25, 250); // Левое поддерево высоты 1
    tree.insert(75, 750);
    tree.insert(60, 600);
    tree.insert(55, 550); // Вызовет балансировку правого поддерева
    // Ожидаемая структура перед удалением: (50 L:25 R:(60 L:55 R:75))
    // Ожидаемая высота: 3

    // 3. Проверка начального состояния дерева
    ASSERT_NE(nullptr, tree.find(50));
    ASSERT_NE(nullptr, tree.find(25));
    ASSERT_NE(nullptr, tree.find(75));
    ASSERT_NE(nullptr, tree.find(60));
    ASSERT_NE(nullptr, tree.find(55));
    ASSERT_EQ(3, tree.get_height());

    // Максимальный элемент в *исходном* дереве (до удаления) - это самый правый узел.
    // В дереве (50 L:25 R:(60 L:55 R:75)) это узел с ключом 75.
    // Мы не будем явно его искать, но проверим его наличие после удаления корня.

    // 4. Удаление корня (ключа 50)
    tree.erase(50);

    // 5. Проверка состояния после удаления и балансировки
    // Ожидаемая структура после удаления 50 и балансировки: (60 L:(25 R:55) R:75)

    // 5a. Проверяем, что узел 50 действительно удален
    ASSERT_EQ(nullptr, tree.find(50));

    // 5b. Проверяем высоту после удаления и балансировки
    ASSERT_EQ(3, tree.get_height());

    // 5c. Проверяем, что новый корень - это 60
    Node<int, int>* new_root = tree.find(60);
    ASSERT_NE(nullptr, new_root);
    ASSERT_EQ(600, new_root->value);

    // 5d. Проверяем наличие и значения остальных узлов
    Node<int, int>* node25 = tree.find(25);
    ASSERT_NE(nullptr, node25);
    ASSERT_EQ(250, node25->value);

    Node<int, int>* node55 = tree.find(55);
    ASSERT_NE(nullptr, node55);
    ASSERT_EQ(550, node55->value);

    Node<int, int>* node75 = tree.find(75); // Исходный максимальный элемент
    ASSERT_NE(nullptr, node75);
    ASSERT_EQ(750, node75->value);

    // 5e. Дополнительные проверки структуры (необязательно, но полезно)
    ASSERT_NE(nullptr, new_root->left);
    ASSERT_EQ(25, new_root->left->key);
    ASSERT_NE(nullptr, new_root->right);
    ASSERT_EQ(75, new_root->right->key);

    ASSERT_EQ(nullptr, node25->left);
    ASSERT_NE(nullptr, node25->right);
    ASSERT_EQ(55, node25->right->key);

    ASSERT_EQ(nullptr, node55->left);
    ASSERT_EQ(nullptr, node55->right);
    ASSERT_EQ(nullptr, node75->left);
    ASSERT_EQ(nullptr, node75->right);
}