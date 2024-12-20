#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <vector>

#pragma once
#include <iostream>
#include <vector>

template <class T>

class Stack {
	std::vector<T> data;
	size_t size;

public:

	//explicit необходим дл€ того чтобы не было не€вного преобразование типов
	//пример если писать без exp... то
	//Stack s = 10;  // Ќе€вное преобразование int в Stack
	// std::cout << s.size << std::endl; // ¬ыведет 10
	// а с exp... вызовет ошибку компили€ции при такой попытке правильно - Stack s2(10);

	Stack() {
		size = 0;
	}
	explicit Stack(size_t n, const T& value = T()) {
		if (n < 0)
			throw std::out_of_range("Size of stack can't be less than zero");
		size = n;
		data = std::vector<T>(n, value);
	}
	Stack(const Stack& other) {
		data.clear();

		size = other.size;

		for (int i = 0; i < size; ++i)
			data.push_back(other.data[i]);

	}
	~Stack() {
		size = 0;
		data.clear();
	}
	Stack& operator=(const Stack& other) {
		data.clear();

		size = other.size;

		for (int i = 0; i < size; ++i) {
			data.push_back(other.data[i]);
		}

		return *this;
	}

	void push(const T& value) {
		data.push_back(value);
		size += 1;
	}

	T pop() {
		if (empty())
			throw std::runtime_error("Can't delete element from empty stack");
		T elem = data.back();
		data.pop_back();
		size--;
		return elem;
	}
	T& top() { // возвращает ссылку, позвол€ет измен€ть верхний элемент
		if (empty()) {
			throw std::runtime_error("Stack is empty");
		}
		return data.back();
	}

	const T& top() const { // константна€ верси€, не измен€ет стек
		if (data.empty()) {
			throw std::runtime_error("Stack is empty");
		}
		return data.back();
	}

	bool empty() {
		if (size == 0)
			return true;
		else
			return false;
	}

	size_t sizes() {
		return size;
	}

	bool operator==(const Stack& other) {
		if ((*this).size != other.size) {
			return 0;
		}

		for (int i = 0; i < size; ++i) {
			if (data[i] != other.data[i])
				return 0;
		}
		return 1;
	}

	bool operator!=(const Stack& other) {
		if ((*this).size != other.size) {
			return 1;
		}

		for (int i = 0; i < size; ++i) {
			if (data[i] != other.data[i])
				return 1;
		}
		return 0;
	}

	void print() const {
		if (data.empty()) {
			std::cout << "Stack is empty" << std::endl;
			return;
		}
		for (size_t i = 0; i < data.size(); ++i) {
			std::cout << data[i] << (i == data.size() - 1 ? "" : " ");
		}
		std::cout << std::endl;
	}
};

#endif
