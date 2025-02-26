
#include "artans.h"

int main() {
    
    ArithmeticTranslator calc;
    std::cout << "Polynomial Calculator (enter 'q' to quit)\n";
    std::cout << "You can save a polynomial by name (egc a = 2x^2 - 1)\n";
    std::cout << "You can multiply, divide, exponentiate, can use unary minus polynom\n";
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q") break;
        if (input.empty()) continue;

        try {
            calc.processExpression(input);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}