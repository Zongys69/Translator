
#include "artans.h"
#include "sorted_table.h"

int main() {

	//std::cout << "The translator processes polynomials written in simplified notation:\n";
	//std::cout << "1) Supports writing without explicit multiplication signs (e.g., 3(x^2)y, -5.2z(y^3)).\n";
	//std::cout << "2) Variables x, y, z can be written in any order (e.g., xy, yx, z(x^2)y).\n";
	//std::cout << "3) Coefficients can be integers or decimal numbers (float), like 2.5x, -0.3(y^2)z, or 7.\n";
	//std::cout << "4) Variable exponents must be  integers (correct: x^3, y^2; invalid: x^2.5, z^0.1).\n";
	//
	//ArithmeticTranslator translator;
	//while (true) {
	//	try {
	//		std::string infix;
 //       std::cout << "Enter an polinom expression: ";
 //       std::getline(std::cin, infix);
	//	int key = translator.addPolinom(infix);
	//	std::cout << "Key: " << key << " = " << translator.getPolinom(key) << std::endl;
	//		
	//	}
	//	catch (const char* e) {
	//		std::cout << e;
	//	}
	//	catch (std::exception e) {
	//		std::cout << e.what() << std::endl;
	//	}
	//}
	///*int key1 = translator.addPolinom("x^2 + 3y - 5z");
	//int key2 = translator.addPolinom("2xy + 4z^3");

	//int key3 = translator.addPolinom("-3y^2zx^3 - 25 - ");
	//std::cout << translator.getAllPolinoms();*/

	SortedTable<int, std::string> table;

	
	table.insert(3, "x^2 + 3y - 5z");
	table.insert(1, "2xy + 4z^3");
	table.insert(2, "5z - 4");

	
	for (auto it = table.begin(); it != table.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
	

	auto found = table.find(2);
	if (found != table.end()) {
		std::cout << "Found: " << found->second << std::endl; 
	}

	
	table.errase(2);

	
	for (auto it = table.begin(); it != table.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
	
}