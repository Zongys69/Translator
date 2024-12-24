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

            if (isNumber(token)) {
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
                    throw std::invalid_argument("Mismatched parentheses: no opening bracket for ')'");
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
                throw std::invalid_argument("Mismatched parentheses: no closing bracket for '('");
            }
            output += operators.top();
            output += " ";
            operators.pop();
        }

        return output;
    }

    double Calculate(const std::string& postfix) {
        Stack<double> operands;
        std::vector<std::string> tokens = StringAnalyze(postfix);

        for (const std::string& token : tokens) {
            if (isNumber(token)) {
                operands.push(stringToNumber(token));
            }
            else if (token == "~") { // ”нарный минус
                double a = operands.top();
                operands.pop();
                operands.push(-a);
            }
            else if (isOperator(token)) {
                double b = operands.top();
                operands.pop();
                double a = operands.top();
                operands.pop();
                double result = applyOperator(a, b, token[0]);
                operands.push(result);
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
            throw std::invalid_argument("Invalid expression: too many values left in stack after calculation");
        }

        return operands.top();
    }

    double getAnswer(const std::string& infix) {
        std::string postfix = ToPostFix(infix);
        return Calculate(postfix);
    }

private:
    bool isNumber(const std::string& token) {
        if (token.empty()) return false;
        int dotCount = 0;

        for (size_t i = 0; i < token.length(); ++i) {
            if (token[i] == '.') {
                dotCount++;
                if (dotCount > 1) return false;
            }
            else if (token[i] < '0' || token[i] > '9') {
                return false;
            }
        }

        return true;
    }

    double stringToNumber(const std::string& str) {
        double result = 0;
        size_t i = 0;
        bool isNegative = false;

        if (str[i] == '-') {
            isNegative = true;
            i++;
        }

        for (; i < str.length() && str[i] != '.'; ++i) {
            result = result * 10 + (str[i] - '0');
        }

        if (i < str.length() && str[i] == '.') {
            i++;
            for (double place = 0.1; i < str.length(); i++, place *= 0.1) {
                result += (str[i] - '0') * place;
            }
        }

        return isNegative ? -result : result;
    }

    bool isOperator(const std::string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

    int precedence(char op) {
        if (op == '^') return 4;
        if (op == '~') return 3; 
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    double applyOperator(double a, double b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b == 0) ? throw std::invalid_argument("Division by zero!") : a / b;
        default: return 0.0;
        }
    }

    std::vector<std::string> StringAnalyze(const std::string& str) {
        std::vector<std::string> tokens;
        std::string current = "";
        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];

            if (isspace(c)) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current = "";
                }
            }
            else if (isOperator(std::string(1, c)) || c == '(' || c == ')' || c == '^') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current = "";
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
