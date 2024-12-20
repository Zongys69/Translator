#include "stack.h"
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