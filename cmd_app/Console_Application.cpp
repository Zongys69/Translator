#include "artans.h"
#include <iostream>
#include <limits>  

int main(int argc, char** argv) {
    std::cout << "This is a calculator that supports:\n"
        << "- Arithmetic operations with numbers (+, -, *, /, ^)\n"
        << "- Polynomial expressions with variable x\n"
        << "- Parentheses and unary minus\n"
        << "- Floating-point numbers \n\n"
        << "To quit, enter 'q'\n";

    ArithmeticTranslator translator;

    while (true) {
        try {
            std::string infix;
            std::cout << "\nEnter expression: ";
            std::getline(std::cin, infix);

            if (infix == "q") return 0;

            double x = 0.0;
            bool needs_x = (infix.find('x') != std::string::npos);

            
            if (needs_x) {
                std::cout << "Enter x value: ";
                while (!(std::cin >> x)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input! Enter x value: ";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            
            double result = translator.getAnswer(infix, x);
            if (result == 0.0) result = 0.0;
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}