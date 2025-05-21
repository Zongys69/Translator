#ifndef __TREE_H__
#define __TREE_H__

#include <iostream>
#include <vector>

template <class Tkey, class Tvalue>
struct Nodel {
	Tkey key;
	Tvalue value;
	Nodel<Tkey, Tvalue>* right;
	Nodel<Tkey, Tvalue>* left;
	int height = 1;

	Nodel(const Tkey& k, const Tvalue& v) : key(k), value(v), right(nullptr), left(nullptr), height(1) {}
};

template <class Tkey, class Tvalue>

class Search_Tree {

private:
	Nodel<Tkey, Tvalue>* root;

	void print_help(Nodel<Tkey, Tvalue>* curr) {
		if (!curr) return;
		print_help(curr->left);
		std::cout << curr->key << " " << curr->value << std::endl;
		print_help(curr->right);
	}

	int get_height_help(Nodel<Tkey, Tvalue>* node) {
		if (node != nullptr)
			return node->height;
		else
			return 0;
	}

	int get_balance_factor(Nodel<Tkey, Tvalue>* node) {
		if (node != nullptr)
			return get_height_help(node->right) - get_height_help(node->left);
		else
			return 0;
	}

	void update_height(Nodel<Tkey, Tvalue>* node) {
		if (node) {
			node->height = 1 + std::max(get_height_help(node->left), get_height_help(node->right));
		}
	}

	Nodel<Tkey, Tvalue>* right_rotation(Nodel<Tkey, Tvalue>* root) {
		Nodel<Tkey, Tvalue>* l_curr = root->left;
		Nodel<Tkey, Tvalue>* r_tree = l_curr->right;

		l_curr->right = root;
		root->left = r_tree;

		update_height(root);
		update_height(l_curr);

		return l_curr;
	}

	Nodel<Tkey, Tvalue>* left_rotation(Nodel<Tkey, Tvalue>* root) {
		Nodel<Tkey, Tvalue>* r_curr = root->right;
		Nodel<Tkey, Tvalue>* l_tree = r_curr->left;

		r_curr->left = root;
		root->right = l_tree;

		update_height(root);
		update_height(r_curr);

		return r_curr;
	}

	Nodel<Tkey, Tvalue>* balance(Nodel<Tkey, Tvalue>* curr) {
		update_height(curr);

		int bal = get_balance_factor(curr);


		if (bal < -1) {
			if (get_balance_factor(curr->left) <= 0)
				return right_rotation(curr);

			else if (get_balance_factor(curr->left) > 0) {

				curr->left = left_rotation(curr->left);

				return right_rotation(curr);
			}

		}

		if (bal > 1) {
			if (get_balance_factor(curr->right) >= 0)
				return left_rotation(curr);

			else if (get_balance_factor(curr->right) < 0) {
				curr->right = right_rotation(curr->right);
				return left_rotation(curr);
			}
		}

		return curr;
	}

	Nodel<Tkey, Tvalue>* insert_help(Nodel<Tkey, Tvalue>* curr, const Tkey& key, const Tvalue& value) {
		if (!curr)
			return new Nodel<Tkey, Tvalue>(key, value);
		else if (key < curr->key) {
			curr->left = insert_help(curr->left, key, value);
		}
		else if (key > curr->key) {
			curr->right = insert_help(curr->right, key, value);
		}
		else if (key == curr->key) {
			curr->value = value;
			return curr;
		}


		return balance(curr);


	}

	Nodel<Tkey, Tvalue>* find_help(Nodel<Tkey, Tvalue>* curr, const Tkey& key) {
		if (!curr)
			return nullptr;
		if (key == curr->key)
			return curr;
		else if (key < curr->key)
			return find_help(curr->left, key);
		else
			return find_help(curr->right, key);
	}

	Nodel<Tkey, Tvalue>* find_max(Nodel<Tkey, Tvalue>* curr) {
		if (!curr)
			return nullptr;
		if (curr->right) {
			return find_max(curr->right);
		}
		return curr;
	}

	Nodel<Tkey, Tvalue>* erase_help(Nodel<Tkey, Tvalue>* curr, const Tkey& key) {
		if (!curr)
			return nullptr;
		else if (key < curr->key) {
			curr->left = erase_help(curr->left, key);
		}
		else if (key > curr->key) {
			curr->right = erase_help(curr->right, key);
		}
		else {
			if (!curr->left || !curr->right) {
				Nodel<Tkey, Tvalue>* temp = nullptr;
				if (curr->left != nullptr)
					temp = curr->left;
				else
					temp = curr->right;

				delete curr;
				curr = temp;
			}

			else {
				Nodel<Tkey, Tvalue>* m = find_max(curr->left);
				curr->key = m->key;
				curr->value = m->value;
				curr->left = erase_help(curr->left, m->key);
			}

		}

		if (curr == nullptr)
			return nullptr;

		return balance(curr);

	}

	Nodel<Tkey, Tvalue>* copy(Nodel<Tkey, Tvalue>* other) {
		if (!other)
			return nullptr;
		Nodel<Tkey, Tvalue>* new_node = new Nodel<Tkey, Tvalue>(other->key, other->value);
		new_node->height = other->height;

		new_node->left = copy(other->left);
		new_node->right = copy(other->right);

		return new_node;
	}

	//óäàëåíèå óçëîâ
	void destruct(Nodel<Tkey, Tvalue>* curr) {
		if (curr) {
			destruct(curr->left);
			destruct(curr->right);
			delete curr;
		}
	}
public:

	Search_Tree() {
		root = nullptr;
	}

	//òðîéêà
	~Search_Tree() {
		destruct(root);
		root = nullptr;
	}

	Search_Tree operator=(const Search_Tree& other) {
		if (this == &other)
			return *this;
		destruct(root);
		root = nullptr;

		root = copy(other.root);

		return *this;
	}

	Search_Tree(const Nodel<Tkey, Tvalue>* other) {
		root = nullptr;
		root = copy(other.root);
	}



	void print() {
		if (!root)
			return;
		print_help(root);
	}

	Nodel<Tkey, Tvalue>* find(const Tkey& key) {
		return find_help(root, key);
	}
	void insert(const Tkey& key, const Tvalue& value) {
		root = insert_help(root, key, value);
	}

	void erase(const Tkey& key) {
		root = erase_help(root, key);
	}

	int get_height() {
		return get_height_help(root);
	}


};

#endif