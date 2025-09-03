#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <iostream>
#include <vector>
#include <list>     
#include <string>


template <class Tkey, class Tvalue>

class Hash_table {
private:
	struct Node {
		Tkey key;
		Tvalue value;
	};

	std::vector<std::list<Node>> table; //вектор цепочек узла
	size_t curr_size; //текущее количество элементов в таблице
	size_t capacity; //размер вектора table
	double DEFAULT_MAX_LOAD_FACTOR = 0.75; // порог рехеширования

	//хэш-функция
	size_t hash_func(const Tkey& key) const {
		std::hash<Tkey> hasher;
		// Применяем оператор % для получения индекса в пределах размера таблицы
		return hasher(key) % capacity;
	}

	void rehash() {
		size_t old_capacity = capacity;
		size_t new_capacity = (capacity == 0) ? 10 : capacity * 2;
		std::vector < std::list <Node >> new_table(new_capacity);
		capacity = new_capacity;

		for (size_t i = 0; i < old_capacity; i++) {
			for (auto it = table[i].begin(); it != table[i].end(); ++it) {
				const Node& current_node = *it;
				size_t new_index = hash_func(current_node.key);
				new_table[new_index].push_back(current_node); // Копируем узел
			}
		}
		table.swap(new_table);
	}

public:
	Hash_table(int n) {
		curr_size = 0;
		capacity = n;
		table.resize(capacity);
	}

	Hash_table() {
		curr_size = 0;
		capacity = 1;
		table.resize(capacity);

	}

	~Hash_table() = default;

	void insert(const Tkey& key, const Tvalue& value) {
		if (capacity == 0 || (double)(curr_size + 1) / capacity > DEFAULT_MAX_LOAD_FACTOR)
			rehash();

		size_t index = hash_func(key);

		//по цепочке
		for (auto it = table[index].begin(); it != table[index].end(); ++it) {
			if ((*it).key == key) {// Сравниваем ключ текущего узла с искомым ключом.
				(*it).value = value; //Ключ найден. Обновляем значение в этом узле
				return;
			} 
		}

		//если не нашли ключ
		table[index].push_back({ key, value});//добавляем новый узел
		curr_size++;
	}

	Tvalue find(const Tkey& key) {
		size_t index = hash_func(key);

		for (auto it = table[index].begin(); it != table[index].end(); ++it) {
			if ((*it).key == key) {
				return (*it).value;
			}
		}

		//не нашли ключ
		throw "key not found";
	}

	void erase(const Tkey& key) {
		if (capacity == 0) {
			return; 
		}
		size_t index = hash_func(key);
		for (auto it = table[index].begin(); it != table[index].end(); ++it) {
			if ((*it).key == key) {
				table[index].erase(it);
				curr_size--;
				return;
			}
		}
		//throw "key not found";
	}

	

	size_t size() {
		return curr_size;
	}

	bool empty() {
		return curr_size == 0;
	}

	void clear() {
		for (size_t i = 0; i < capacity; i++) {
			table[i].clear();
		}
		curr_size = 0;
	}

	size_t get_capacity() {
		return capacity;
	}
};
#endif