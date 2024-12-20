#include "artans.h"


int main(int argc, char** argv) {
    std::cout << "This is a simple calculator for performing arithmetic operations. It supports integers and floating-point numbers, operations +, -, *, /, ^, as well as parentheses. Unary minus is not supported. For entering floating-point numbers, use formats like 2.1, 2.0, and so on.";
    std::cout << std::endl;
    std::cout << "To quit, press the q key on your keyboard" << std::endl;
    ArithmeticTranslator translator;

    while (true) {
        try {
        std::string infix;
        std::cout << "Enter an arithmetic expression: ";
        std::getline(std::cin, infix);

        if (infix == "q") {
            return 0;
        }
        
            // Получаем ответ как число
            double result = translator.getAnswer(infix);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const char* e) {
            std::cout << e;
        }
        catch (std::exception e) {
            std::cout << e.what();
        }
       
       
    }
    return 0;
}
