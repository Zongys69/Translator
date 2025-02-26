#ifndef __ARTANS_H__
#define __ARTANS_H__

#include "stack.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>

class ArithmeticTranslator {
public:
    std::string ToPostFix(const std::string& infix) {
        std::string output = "";
        Stack<char> operators;
        std::vector<std::string> tokens = StringAnalyze(infix);

        for (size_t i = 0; i < tokens.size(); ++i) {
            const std::string& token = tokens[i];

            if (isNumber(token) || token == "x") {
                output += token + " ";
            }
            else if (token == "(") {
                operators.push('(');
            }
            else if (token == ")") {
                while (!operators.empty() && operators.top() != '(') {
                    output += operators.top();
                    output += " ";
                    operators.pop();
                }
                if (operators.empty()) {
                    throw std::invalid_argument("Mismatched parentheses");
                }
                operators.pop();
            }
            else if (isOperator(token)) {
                if (token == "-" && (i == 0 || isOperator(tokens[i - 1]) || tokens[i - 1] == "(")) {
                    operators.push('~');
                }
                else {
                    while (!operators.empty() && precedence(operators.top()) >= precedence(token[0])) {
                        output += operators.top();
                        output += " ";
                        operators.pop();
                    }
                    operators.push(token[0]);
                }
            }
            else if (token == "^") {
                while (!operators.empty() && precedence(operators.top()) > precedence(token[0])) {
                    output += operators.top();
                    output += " ";
                    operators.pop();
                }
                operators.push('^');
            }
            else {
                throw std::invalid_argument("Invalid token: " + token);
            }
        }

        while (!operators.empty()) {
            if (operators.top() == '(') {
                throw std::invalid_argument("Mismatched parentheses");
            }
            output += operators.top();
            output += " ";
            operators.pop();
        }

        return output;
    }

    double Calculate(const std::string& postfix, double x_value) {
        Stack<double> operands;
        std::vector<std::string> tokens = StringAnalyze(postfix);

        for (const std::string& token : tokens) {
            if (isNumber(token)) {
                operands.push(stringToNumber(token));
            }
            else if (token == "x") {
                operands.push(x_value);
            }
            else if (token == "~") {
                double a = operands.top();
                operands.pop();
                operands.push(-a);
            }
            else if (isOperator(token)) {
                double b = operands.top();
                operands.pop();
                double a = operands.top();
                operands.pop();
                operands.push(applyOperator(a, b, token[0]));
            }
            else if (token == "^") {
                double b = operands.top();
                operands.pop();
                double a = operands.top();
                operands.pop();
                operands.push(std::pow(a, b));
            }
        }

        if (operands.sizes() != 1) {
            throw std::invalid_argument("Invalid expression");
        }

        return operands.top();
    }
    
    double getAnswer(const std::string& infix, double x) {
        std::string postfix = ToPostFix(infix);
        double result = Calculate(postfix, x);
        return Calculate(postfix, x);
        if (result == 0.0 && std::signbit(result)) {
            return 0.0;
        }
        return result;
    }
    double getAnswer(const std::string& infix) {
        return getAnswer(infix, 0.0); 
    }

private:
    bool isNumber(const std::string& token) {
        if (token.empty()) return false;
        size_t start = 0;
        if (token[0] == '-') {
            if (token.size() == 1) return false;
            start = 1;
        }
        int dotCount = 0;
        for (size_t i = start; i < token.size(); ++i) {
            if (token[i] == '.') {
                if (++dotCount > 1) return false;
            }
            else if (token[i] < '0' || token[i] > '9') {
                return false;
            }
        }
        return true;
    }

    bool isOperator(const std::string& token) {
        return token.size() == 1 &&
            (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/');
    }

    int precedence(char op) {
        if (op == '^') return 4;
        if (op == '~') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    double stringToNumber(const std::string& str) {
        size_t offset = 0;
        double result = 0;
        try {
            result = std::stod(str, &offset);
        }
        catch (...) {
            throw std::invalid_argument("Invalid number: " + str);
        }
        if (offset != str.size()) {
            throw std::invalid_argument("Invalid number: " + str);
        }
        return result;
    }

    double applyOperator(double a, double b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::invalid_argument("Division by zero");
            return a / b;
        default: throw std::invalid_argument("Unknown operator");
        }
    }

    std::vector<std::string> StringAnalyze(const std::string& str) {
        std::vector<std::string> tokens;
        std::string current;

        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];

            if (isspace(c)) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                continue;
            }


            if (!current.empty() &&
                ((isNumber(current) && (c == 'x' || c == '(')) ||
                    (current == "x" && (c == '(' || isdigit(c)))
                    ))
            {
                tokens.push_back(current);
                tokens.push_back("*");
                current.clear();
            }


            if (c == '(' || c == ')' || c == '^' || isOperator(std::string(1, c))) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back(std::string(1, c));
            }
            else {
                current += c;
            }
        }

        if (!current.empty()) {
            tokens.push_back(current);
        }

        return tokens;
    }

};

#endif