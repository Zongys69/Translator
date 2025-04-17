#include "stack.h"
#include "binary_tree.h"
#include <gtest.h>


TEST(Stack, can_create_empty_stack)
{
	ASSERT_NO_THROW(Stack<int> s);
}

TEST(Stack, can_create_stack_with_specifed_values)
{
	ASSERT_NO_THROW(Stack<int> s(3, 5));
}

TEST(Stack, can_create_stack_by_const_link)
{
	Stack<int> s;
	s.push(1);

	Stack<int> s1(s);

	ASSERT_EQ(s1.pop(), s.pop());
}

TEST(Stack, can_pop_element)
{
	Stack<int> s;
	s.push(1);

	int a = 1;

	ASSERT_EQ(s.pop(), a);
}

TEST(Stack, cant_pop_element_from_empty_stack)
{
	Stack<int> s;

	ASSERT_ANY_THROW(s.pop());
}

TEST(Stack, can_get_size_of_stack)
{
	Stack<int> s(15, 3);

	int a = 15;

	ASSERT_EQ(s.sizes(), a);
}

TEST(Stack, can_get_size_after_pop)
{
	Stack<int> s(15, 1);
	s.pop();

	int a = 14;

	ASSERT_EQ(s.sizes(), a);
}

TEST(Stack, can_watch_element_by_top)
{
	Stack<int> s;
	s.push(1);
	s.push(2);

	int a = 2;

	ASSERT_EQ(s.top(), a);
}

TEST(Stack, can_modify_element_by_top)
{
	Stack<int> s;
	s.push(1);
	s.push(2);

	int a = 3;

	s.top() = 3;

	ASSERT_EQ(s.top(), a);
}

TEST(Stack, can_get_size_after_top)
{
	Stack<int> s;
	s.push(1);
	s.push(2);

	int a = 2;

	s.top() = 3;

	ASSERT_EQ(s.sizes(), a);
}

TEST(Stack, can_use_empty)
{
	Stack<int> s;

	ASSERT_EQ(true, s.empty());
}

TEST(Stack, cant_use_empty_from_not_empty_stack)
{
	Stack<int> s(3);

	ASSERT_EQ(s.empty(), false);
}


TEST(RedBlackTreeTest, InsertAndSearch) {
	BinaryTree<int, std::string> tree;

	tree.insert(10, "a");
	tree.insert(20, "b");
	tree.insert(30, "c");
	tree.insert(15, "d");
	tree.insert(5, "e");

	EXPECT_EQ(*tree.search(10), "a");
	EXPECT_EQ(*tree.search(20), "b");
	EXPECT_EQ(*tree.search(30), "c");
	EXPECT_EQ(*tree.search(15), "d");
	EXPECT_EQ(*tree.search(5), "e");
	EXPECT_EQ(tree.search(100), nullptr); // элемент не существует
}

TEST(RedBlackTreeTest, RemoveExistingElement) {
	BinaryTree<int, std::string> tree;

	tree.insert(10, "a");
	tree.insert(20, "b");
	tree.insert(30, "c");

	tree.remove(20);

	EXPECT_EQ(tree.search(20), nullptr);
	EXPECT_NE(tree.search(10), nullptr);
	EXPECT_NE(tree.search(30), nullptr);
}

TEST(RedBlackTreeTest, RemoveNonExistingElement) {
	BinaryTree<int, std::string> tree;

	tree.insert(10, "a");
	tree.insert(5, "b");

	tree.remove(42); // несуществующий элемент

	EXPECT_NE(tree.search(10), nullptr);
	EXPECT_NE(tree.search(5), nullptr);
}

TEST(RedBlackTreeTest, InsertDuplicateKey) {
	BinaryTree<int, std::string> tree;

	tree.insert(10, "a");
	tree.insert(10, "updated");

	EXPECT_EQ(*tree.search(10), "updated");
}
TEST(RedBlackTreeTest, LeftRotationOccurs) {
	BinaryTree<int, std::string> tree;

	tree.insert(10, "a");
	tree.insert(20, "b");
	tree.insert(30, "c");

	auto root = tree.getRoot();

	EXPECT_EQ(root->key, 20);
	EXPECT_EQ(tree.nodeColor(root), "black");
	EXPECT_EQ(root->left->key, 10);
	EXPECT_EQ(root->right->key, 30);
}
TEST(RedBlackTreeTest, RightRotationOccurs) {
	BinaryTree<int, std::string> tree;

	tree.insert(30, "a");
	tree.insert(20, "b");
	tree.insert(10, "c");

	auto root = tree.getRoot();

	EXPECT_EQ(root->key, 20);
	EXPECT_EQ(tree.nodeColor(root), "black");
	EXPECT_EQ(root->left->key, 10);
	EXPECT_EQ(root->right->key, 30);
}

TEST(RedBlackTreeTest, RootIsAlwaysBlack) {
	BinaryTree<int, std::string> tree;

	for (int i = 1; i <= 100; ++i) {
		tree.insert(i, std::to_string(i));
		auto root = tree.getRoot();
		ASSERT_NE(root, nullptr);
		EXPECT_EQ(tree.nodeColor(root), "black");
	}
}
TEST(RedBlackTreeTest, RightLeftRotationOccurs) {
	BinaryTree<int, std::string> tree;

	// Вставки вызывают правый поворот у левого поддерева + левый поворот
	tree.insert(10, "a");
	tree.insert(30, "b");
	tree.insert(20, "c");

	auto root = tree.getRoot();

	EXPECT_EQ(root->key, 20);
	EXPECT_EQ(tree.nodeColor(root), "black");
	EXPECT_EQ(root->left->key, 10);
	EXPECT_EQ(root->right->key, 30);
}
TEST(RedBlackTreeTest, LeftRightRotationOccurs) {
	BinaryTree<int, std::string> tree;

	// Вставки вызывают левый поворот у правого поддерева + правый поворот
	tree.insert(30, "a");
	tree.insert(10, "b");
	tree.insert(20, "c");

	auto root = tree.getRoot();

	EXPECT_EQ(root->key, 20);
	EXPECT_EQ(tree.nodeColor(root), "black");
	EXPECT_EQ(root->left->key, 10);
	EXPECT_EQ(root->right->key, 30);
}

TEST(RedBlackTreeTest, InsertManyAndSearch) {
	BinaryTree<int, std::string> tree;
	for (int i = 0; i < 1000000; i++) {
		tree.insert(i, "a");
	}
	for (int i = 0; i < 1000000; i++) {
		EXPECT_EQ(*tree.search(i), "a");
	}
	
	
}

TEST(RedBlackTreeTest, Insert2ManyAndSearch) {
	BinaryTree<int, std::string> tree;
	for (int i = 999999; i >= 0; i--) {
		tree.insert(i, "a");
	}
	for (int i = 999999; i >= 0; i--) {
		EXPECT_EQ(*tree.search(i), "a");
	}


}
TEST(RedBlackTreeTest, RemoveManyAndSearch) {
	BinaryTree<int, std::string> tree;
	for (int i = 0; i < 1000000; i++) {
		tree.insert(i, "a");
	}
	for (int i = 0; i < 1000000; i++) {
		tree.remove(i);
	}
	EXPECT_EQ(tree.empty(), true);


}