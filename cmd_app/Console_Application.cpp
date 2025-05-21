////
////#include "artans.h"
////#include "sorted_table.h"
////
//int main() {
//
//	std::cout << "The translator processes polynomials written in simplified notation:\n";
//	std::cout << "1) Supports writing without explicit multiplication signs (e.g., 3(x^2)y, -5.2z(y^3)).\n";
//	std::cout << "2) Variables x, y, z can be written in any order (e.g., xy, yx, z(x^2)y).\n";
//	std::cout << "3) Coefficients can be integers or decimal numbers (float), like 2.5x, -0.3(y^2)z, or 7.\n";
//	std::cout << "4) Variable exponents must be  integers (correct: x^3, y^2; invalid: x^2.5, z^0.1).\n";
//	
//	ArithmeticTranslator translator;
//	while (true) {
//		try {
//			std::string infix;
//        std::cout << "Enter an polinom expression: ";
//        std::getline(std::cin, infix);
//		int key = translator.addPolinom(infix);
//		std::cout << "Key: " << key << " = " << translator.getPolinom(key) << std::endl;
//			
//		}
//		catch (const char* e) {
//			std::cout << e;
//		}
//		catch (std::exception e) {
//			std::cout << e.what() << std::endl;
//		}
//	}
//	/*int key1 = translator.addPolinom("x^2 + 3y - 5z");
//	int key2 = translator.addPolinom("2xy + 4z^3");
//
//	int key3 = translator.addPolinom("-3y^2zx^3 - 25 - ");
//	std::cout << translator.getAllPolinoms();*/
//
//	SortedTable<int, std::string> table;
//
//	
//	table.insert(3, "x^2 + 3y - 5z");
//	table.insert(1, "2xy + 4z^3");
//	table.insert(2, "5z - 4");
//
//	
//	for (auto it = table.begin(); it != table.end(); ++it) {
//		std::cout << it->first << ": " << it->second << std::endl;
//	}
//	
//
//	auto found = table.find(2);
//	if (found != table.end()) {
//		std::cout << "Found: " << found->second << std::endl; 
//	}
//
//	
//	table.erase(2);
//
//	
//	for (auto it = table.begin(); it != table.end(); ++it) {
//		std::cout << it->first << ": " << it->second << std::endl;
//	}
//	
//}
//#include <binary_tree.h>
//#include <iostream>
//#include <cassert>
//
//
//int main() {
//    BinaryTree<int, std::string> tree;
//    
//    setlocale(LC_ALL, "Ru");
//   
//    tree.insert(10, "a");
//    tree.insert(20, "b");
//    tree.insert(30, "c");
//    tree.insert(15, "d");
//    tree.insert(25, "e");
//    tree.insert(5, "f");
//    tree.insert(1, "a");
//    std::cout << "Дерево после вставок:\n";
//    tree.printTree(tree.getRoot());
//    tree.printInOrder();
//
//   
//    assert(tree.search(10) && *tree.search(10) == "a");
//    assert(tree.search(25) && *tree.search(25) == "e");
//    assert(tree.search(100) == nullptr);
//
//   
//    tree.insert(10, "updated");
//    assert(tree.search(10) && *tree.search(10) == "updated");
//
//  
//    tree.remove(5);
//    
//
//    std::cout << "\nДерево после удаления 15, 25, 10:\n";
//    tree.printTree(tree.getRoot());
//    tree.printInOrder();
//    
//    BinaryTree<int, std::string> tree2 = tree;
//
//    
//    tree2.printTree(tree2.getRoot());
//    return 0;
//}


#include "binary_tree.h"
#include <iostream>
#include <string>
#include <optional>
#include "hash.h"
#include "artans.h"
#include "arth.h"
#include "sorted_table.h"
int main() {
	std::cout << "The translator processes polynomials written in simplified notation:\n";
	std::cout << "1) Supports writing without explicit multiplication signs (e.g., 3(x^2)y, -5.2z(y^3)).\n";
	std::cout << "2) Variables x, y, z can be written in any order (e.g., xy, yx, z(x^2)y).\n";
	std::cout << "3) Coefficients can be integers or decimal numbers (float), like 2.5x, -0.3(y^2)z, or 7.\n";
	std::cout << "4) Variable exponents must be  integers (correct: x^3, y^2; invalid: x^2.5, z^0.1).\n";

	ArithmeticTranslator translator;
	BinaryTree<int, std::string> tree;
	HashTable<std::string, std::string> table;
	SortedTable<int, std::string> stable;
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
				if (structure == "Hash" || structure == "RBTree" || structure == "Sorted table") {
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
									std::cout << "Not find" << std::endl;
							}
							if (action == "Size") {
								std::cout << "Size of hash:" << table.getSize() << std::endl;
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
									std::cout << "Not find" << std::endl;
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
									std::cout << "Not find" << std::endl;
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
									std::cout << "Not find" << std::endl;

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