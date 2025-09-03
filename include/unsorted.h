#ifndef __UNSORTED_H__
#define __UNSORTED_H__

#include <iostream>
#include <vector>
#include <utility> 
template <class Tkey, class Tvalue>
class Unsorted_table {
private:
	std::vector<std::pair<Tkey, Tvalue>> data;

public:
	class Iterator { 
	protected:
		typename std::vector<std::pair<Tkey, Tvalue>>::iterator current;

	public:
		Iterator(typename std::vector<std::pair<Tkey, Tvalue>>::iterator it) : current(it) {}

		Iterator& operator++() {
			++current;
			return *this;
		}

		Iterator operator++(int) {
			Iterator copy = *this;
			++current;
			return copy;
		}

		std::pair<Tkey, Tvalue>& operator*() {
			return *current;
		}

		std::pair<Tkey, Tvalue>* operator->() {
			return &(*current);
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2) {
			return it1.current != it2.current;
		}

		friend bool operator==(const Iterator& it1, const Iterator& it2) {
			return it1.current == it2.current;
		}

		friend class Unsorted_table<Tkey, Tvalue>;
	};


	Iterator insert(const Tkey& key, const Tvalue& value) {
		data.push_back(std::make_pair(key, value));
		return Iterator(data.end() - 1);
	}

	Iterator erase(const Tkey& key) {
		for (Iterator it = begin(); it != end(); ++it) {
			if (it->first == key) {
				return Iterator(data.erase(it.current)); 
			}
		}
		return end();
	}

	Iterator find(const Tkey& key) {
		for (Iterator it = begin(); it != end(); ++it) {
			if (it->first == key) {
				return it;
			}
		}
		return end();
	}

	Iterator begin() {
		return Iterator(data.begin());
	}

	Iterator end() {
		return Iterator(data.end());
	}
};

#endif