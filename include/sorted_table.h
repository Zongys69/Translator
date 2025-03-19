#include <iostream>
#include <utility>
#include <vector>
template <class TKey, class TValue>

class SortedTable 
{
	std::vector<std::pair<TKey, TValue>> data;
public:
	class Iterator {
	protected:
		typename std::vector<std::pair<TKey, TValue>>::iterator current;

	public:
		Iterator(typename std::vector<std::pair<TKey, TValue>>::iterator it) : current(it) {}

		Iterator& operator++() 
		{
			++current;
			return *this;
		}

		Iterator operator++(int) 
		{
			Iterator copy = *this;
			++current;
			return copy;
		}

		std::pair<TKey, TValue>& operator*()
		{
			return *current;
		}

		std::pair<TKey, TValue>* operator->() 
		{
			return &(*current);
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2)
		{
			return it1.current != it2.current;
		}

		friend bool operator==(const Iterator& it1, const Iterator& it2)
		{
			return it1.current == it2.current;
		}

		friend class SortedTable<TKey, TValue>;
	};

	Iterator find(const TKey& key) 
	{
		int left = 0;
		int right = data.size() - 1;
		while (left <= right)
		{
			int middle = (left + right) / 2;
			if (key < data[middle].first)
				right = middle - 1;
			else if (key < data[middle].first)
				left = middle + 1;
			else
				return Iterator(data.begin() + middle);
		}
		return end();
	}
	Iterator erase(const TKey& key)
	{
		auto it = find(key);
		if (it == end())
			throw "The value does not exist";
		for (int i = 0; i < data.size(); i++) 
		{
			if (data[i].first == key) 
			{
				data.erase(data.begin() + i);
				break;
			}
		}
		return it;
	}
	Iterator insert(const TKey& key, const TValue& value)
	{
		data.push_back(std::make_pair(key, value));
		int i = data.size() - 1;
		if (i == 0)
			return Iterator(data.begin());
		while (i > 0) {
			if (data[i - 1].first > data[i].first)
			{
				std::swap(data[i - 1], data[i]);
				i--;
			}
			else if (data[i - 1].first == data[i].first)
			{
				throw "This value is exist";
			}
			else
				break;
		}
		return Iterator(data.begin() + i);

	}
	Iterator begin()
	{
		return Iterator(data.begin());
	}
	Iterator end()
	{
		return Iterator(data.end());
	}
};