

#include "binary_tree.h"
#include <iostream>
#include <string>
#include <optional>
#include "avl_tree.h"
#include "hash_chain.h"
#include "unsorted_table.h"
#include "hash.h"
#include "artans.h"
#include "arth.h"
#include "sorted_table.h"
int main() {
	
	std::cout << "We have an arithmetic translator, polinoms translator and data structures in which we can store polynomials.\n";
	std::cout << "The translator processes polynomials written in simplified notation:\n";
	std::cout << "1) Supports writing without explicit multiplication signs (e.g., 3(x^2)y, -5.2z(y^3)).\n";
	std::cout << "2) Variables x, y, z can be written in any order (e.g., xy, yx, z(x^2)y).\n";
	std::cout << "3) Coefficients can be integers or decimal numbers (float), like 2.5x, -0.3(y^2)z, or 7.\n";
	std::cout << "4) Variable exponents must be  integers (correct: x^3, y^2; invalid: x^2.5, z^0.1).\n";
	std::cout << "5) You can save your polinom in structures with names: Hash, Hash Chain, RBTree, AVLTree, Sorted table, Unsorted table\n";
	std::cout << "6) Structers supports all necessary operations search, insert, delete, output of all polynomials, size\n";

	ArithmeticTranslator translator;
	BinaryTree<int, std::string> tree;
	Search_Tree<int, std::string> tree_avl;
	HashTable<std::string, std::string> table;
	Hash_table<std::string, std::string> hash;
	SortedTable<int, std::string> stable;
	Unsorted_table<int, std::string> utable;

	std::string mode;
	std::string polinom;
	std::string structure;
	std::string action;
	std::string action_2;

	std::cout << "Chose mode of work P/A:";
	std::getline(std::cin, mode);

	if (mode == "P") {
		while (true) {
			try {

				action = " ";
				std::cout << "Enter an polinom expression: ";
				std::getline(std::cin, polinom);

				std::cout << "Enter an structure: ";
				std::getline(std::cin, structure);
				if (structure == "Hash" || structure == "RBTree" || structure == "Sorted table" || structure == "Hash Chain" || structure == "AVLTree" || structure == "Unsorted table") {
					int key = translator.addPolinom(polinom);

					if (structure == "Hash") {

						table.insert(std::to_string(key), translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								if (table.find(action_2) != "")
									std::cout << table.find(action_2) << std::endl;
								else
									std::cout << "Not found" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Size of hash: " << table.getSize() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								table.remove(action_2);
								std::cout << "Size of hash: " << table.getSize() << std::endl;
							}
							if (action == "Select all polinoms") {
								for (int i = 1; i < 10; i++) {
									std::cout << "Key: " << i << " Value: " << table.find(std::to_string(i)) << std::endl;
								}
							}
						}

					}
					else if (structure == "Hash Chain") {

						hash.insert(std::to_string(key), translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								try {
									if (hash.find(action_2) != "")
										std::cout << hash.find(action_2) << std::endl;
								}
								catch (const char* error_msg) { 
									std::cout << "Not found" << std::endl;
								}
							}
							if (action == "Size") {
								std::cout << "Size of hash: " << hash.size() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								try {
									if (hash.find(action_2) != "") {
										hash.erase(action_2);
										std::cout << "Size of hash: " << hash.size() << std::endl;
									}
								}
								catch (const char* error_msg) { 
									std::cout << "Not found" << std::endl;
								}
								
								
							}
							if (action == "Select all polinoms") {
								for (int i = 1; i < 10; i++) {
									try {
										std::string key = std::to_string(i); 
										std::cout << "Key: " << key << " Value: " << hash.find(key) << std::endl;
									}
									catch (const char* error_msg) { 
										std::cout << "Key: " << i << " - Not found" << std::endl;
									}
								}
							}
						}

					}
					else if (structure == "AVLTree") {
						tree_avl.insert(key, translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								Nodel<int, std::string>* node = tree_avl.find(std::stoi(action_2));
								if (node != nullptr)
									std::cout << node->value << std::endl;
								else
									std::cout << "Not found" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Height of tree:" << tree_avl.get_height() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								Nodel<int, std::string>* nodel = tree_avl.find(std::stoi(action_2));
								if (nodel != nullptr)
									tree_avl.erase(std::stoi(action_2));
								else
									std::cout << "Not found" << std::endl;
								
								std::cout << "Height of tree:" << tree_avl.get_height() << std::endl;
							}
							if (action == "Select all polinoms") {

								for (int i = 0; i < 10; i++) {
									Nodel<int, std::string>* nodell = tree_avl.find(i);
									if (nodell != nullptr)
										std::cout << "Key: " << i << " Value: " << nodell->value << std::endl;	
								}
							}
						}
					}

					else if (structure == "RBTree")
					{
						tree.insert(key, translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								if (tree.search(std::stoi(action_2)) != NULL)
									std::cout << *(tree.search(std::stoi(action_2))) << std::endl;
								else
									std::cout << "Not found" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Size of tree:" << tree.size() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								tree.remove(std::stoi(action_2));
								std::cout << "Size of tree: " << tree.size() << std::endl;
							}
							if (action == "Select all polinoms") {
								for (int i = 1; i < 10; i++) {
									if (tree.search(i) != NULL)
										std::cout << "Key: " << i << " Value: " << *(tree.search(i)) << std::endl;
								}
							}
						}

					}
					else if (structure == "Sorted table")
					{
						stable.insert(key, translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								auto it = stable.find(std::stoi(action_2));
								if (it != stable.end())
									std::cout << it->second << std::endl;
								else
									std::cout << "Not found" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Size of table:" << stable.size() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								auto it = stable.find(std::stoi(action_2));
								if (it != stable.end())
									stable.erase(std::stoi(action_2));
								else
									std::cout << "Not found" << std::endl;

								std::cout << "Size of table: " << stable.size() << std::endl;
							}
							if (action == "Select all polinoms") {
								auto it = stable.begin();
								while (it != stable.end()) {
									std::cout << "Key: " << it->first << " " << "Value: " << it->second << std::endl;
									it++;
								}
							}
						}
					}
					else if (structure == "Unsorted table")
					{
						utable.insert(key, translator.getPolinom(key));
						while (action != ":q") {
							std::cout << "What action do you want to do: ";
							std::getline(std::cin, action);
							if (action == "Find") {
								std::cout << "What do you want to find: ";
								std::getline(std::cin, action_2);
								auto it = utable.find(std::stoi(action_2));
								if (it != utable.end())
									std::cout << it->second << std::endl;
								else
									std::cout << "Not found" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Size of table:" << utable.size() << std::endl;
							}

							if (action == "Remove") {
								std::cout << "What do you want to remove: ";
								std::getline(std::cin, action_2);
								auto it = utable.find(std::stoi(action_2));
								if (it != utable.end())
									utable.erase(std::stoi(action_2));
								else
									std::cout << "Not found" << std::endl;

								std::cout << "Size of table: " << utable.size() << std::endl;
							}
							if (action == "Select all polinoms") {
								auto it = utable.begin();
								while (it != utable.end()) {
									std::cout << "Key: " << it->first << " " << "Value: " << it->second << std::endl;
									it++;
								}
							}
						}
						}
				}


			}
			catch (const char* e) {
				std::cout << e;
			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}


		}
	}

	else if (mode == "A") {
		while (true) {
			try {
				std::string infix;
				std::cout << "Enter an arithmetic expression: ";
				std::getline(std::cin, infix);

				if (infix == ":q") {
					return 0;
				}

				double result = translator.getAnswer(infix);
				std::cout << "Result: " << result << std::endl;
			}
			catch (const char* e) {
				std::cout << e;
			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}


		}
	}
}