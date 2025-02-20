#pragma once
#include <iostream>


using namespace std;

template <class T>
class List {
public:
	struct Node {
		T data;
		Node* next;
		Node() {
			data = 0;
			next = nullptr;
		}
		Node(T value, Node* current) {
			data = value;
			next = current;
		}
	};
private:
	Node* first;
public:

	List() : first(nullptr) {}
	List(int n, T deflt = T()) {
		if (n == 0)
			return;
		if (n < 0)
			throw "List cant have a negative size";
		first = new Node(deflt, nullptr);
		Node* current = first;

		for (int i = 1; i < n; i++) {
			Node* tmp = new Node(deflt, nullptr);
			current->next = tmp;
			current = current->next;
		}
	}

	~List() {
		clear();
	}

	List(const List& other) {
		if (other.first == nullptr)
			throw "size of list = zero";

		first = new Node(other.first->data, nullptr);
		Node* current = first;
		Node* ocurent = other.first;

		while (ocurent->next) {
			current->next = new Node(ocurent->data, nullptr);
			current = current->next;
			ocurent = ocurent->next;
		}
	}

	List<T>& operator=(const List<T>& other) noexcept {
		if (this == &other)
			return *this;

		clear();

		if (other.first == nullptr)
			return *this;

		first = new Node(other.first->data, nullptr);
		Node* current = first;
		Node* ocurent = other.first->next;

		while (ocurent != nullptr) {
			current->next = new Node(ocurent->data, nullptr);
			current = current->next;
			ocurent = ocurent->next;
		}

		return *this;
	}

	void clear() {
		Node* current = first;
		while (current != nullptr) {
			Node* next = current->next;
			delete current;
			current = next;
		}
		first = nullptr;
	}

	void print() {
		Node* current = first;
		while (current) {
			cout << current->data << "->";
			current = current->next;
		}
		cout << endl;
	}
	//the task for list - 10
	void printReverse() {

		Node* prev = nullptr;
		Node* current = first;
		Node* next = nullptr;

		while (current != nullptr) {
			next = current->next;

			current->next = prev;

			prev = current;
			current = next;
		}

		current = prev;

		while (current != nullptr) {
			cout << current->data << "->";
			current = current->next;
		}
		cout << endl;
	}

	bool operator==(const List<T>& other) const noexcept {
		Node* current1 = first;
		Node* current2 = other.first;

		while (current1 != nullptr && current2 != nullptr) {
			if (current1->data != current2->data)
				return false;
			current1 = current1->next;
			current2 = current2->next;
		}
		return (current1 == nullptr && current2 == nullptr);
	}

	bool operator!=(const List<T>& other) const noexcept {
		Node* current1 = first;
		Node* current2 = other.first;

		while (current1 != nullptr && current2 != nullptr) {
			if (current1->data == current2->data)
				return false;
			current1 = current1->next;
			current2 = current2->next;
		}
		return (current1 != nullptr || current2 != nullptr);
	}

	T& operator[](int index) {
		if (index < 0 || index >= size())
			throw out_of_range("Index out of range");

		Node* current = first;
		for (int i = 0; i < index; ++i)
			current = current->next;

		return current->data;
	}

	int size() const {
		int count = 0;
		Node* current = first;

		while (current != nullptr) {
			count++;
			current = current->next;
		}

		return count;
	}

	inline List<T>::Node* get_index(int index) {
		if (index < 0 || index >= size())
			throw out_of_range("Index out of range");


		Node* current = first;
		for (int i = 0; i < index; ++i) {
			current = current->next;
		}

		return current;
	}

	inline List<T>::Node* insert(T value, Node* prev) {
		Node* tmp = new Node;

		tmp->next = prev; //от предыдущего элемента получили ссылку на след
		tmp->data = value;
		prev->next = tmp;

		return tmp;
		//либо так
		//return prev->next = new temp Node(prev->next, value);
	}


	inline List<T>::Node* insert_front(T value) {
		Node* first_first = new Node;

		first_first->next = first;
		first_first->data = value;
		first = first_first;

		return first;
	}


	inline List<T>::Node* erase(Node* prev)
	{
		Node* tmp = prev->next;

		if (!(prev->next)) throw;

		prev->next = tmp->next;
		delete tmp;

		return prev->next;
	}

	inline List<T>::Node* erase_front()
	{
		Node* tmp = first;
		first = tmp->next;
		delete tmp;

		return first;
	}

	inline List<T>::Node* find(T value) const {
		Node* current = first;

		while (current != nullptr) {
			if (current->data == value) {
				return current;
			}
			current = current->next;
		}

		return nullptr;
	}

	inline List<T>::Node* get_first() noexcept
	{
		return first;
	}

	friend istream& operator>>(istream& istr, List& other)
	{
		Node* current = other.first;

		while (current) {
			istr >> current->data;
			current = current->next;
		}

		return istr;
	}

	friend ostream& operator<<(ostream& ostr, const List& other)
	{
		Node* current = other.first;

		while (current) {
			ostr << current->data << ' ';
			current = current->next;
		}

		return ostr;
	}
	//Iterator
	class Iterator {
	private:
		Node* current;

	public:

		Iterator(Node* node) : current(node) {}

		Iterator& operator++() {
			if (!current || !current->next)
				throw "Out of range";

			current = current->next;
			return *this;
		}


		Iterator operator++(int) {
			if (!current || !current->next)
				throw "Out of range";

			Iterator copy(*this);
			current = current->next;
			return copy;
		}

		T& operator*() {
			if (!current)
				throw "Out of range";

			return current->data;
		}

		T* operator->() {
			if (!current)
				throw "Out of range";

			return &(current->data);
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2) {
			return it1.current != it2.current;
		}

		friend bool operator==(const Iterator& it1, const Iterator& it2) {
			return it1.current == it2.current;
		}
	};

	Iterator begin() {
		return Iterator(first);
	}

	Iterator end() {
		return Iterator(nullptr);
	}

};


























