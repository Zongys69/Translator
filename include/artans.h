#ifndef __ARTANS_H__
#define __ARTANS_H__

#include "stack.h"
#include "polinom.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <sstream>

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

    int addPolinom(const std::string& expr) {
        std::string cleanedExpr;
        for (char c : expr) {
            if (!std::isspace(c)) {
                cleanedExpr += c;
            }
        }

        if (cleanedExpr.empty()) {
            throw std::invalid_argument("Empty polynomial");
        }

        std::vector<std::string> monomStrs = splitIntoMonoms(cleanedExpr);

        Polinom polinom;

        for (const std::string& monomStr : monomStrs) {
            try {
                Monom monom = parseMonom(monomStr);
                polinom.addMonom(monom);
            }
            catch (const std::exception& e) {
                throw std::invalid_argument("Error parsing monom '" + monomStr + "': " + e.what());
            }
        }

        polinoms.push_back(polinom);
        return polinoms.size();
    }

    std::string getPolinom(int key) const {
        if (key < 1 || key > polinoms.size()) {
            throw std::invalid_argument("Invalid key");
        }
        return polinoms[key - 1].toString();
    }

    std::string getAllPolinoms() const {
        std::ostringstream oss;
        for (size_t i = 0; i < polinoms.size(); ++i) {
            oss << "Key: " << (i + 1) << " = " << polinoms[i].toString() << "\n";
        }
        return oss.str();
    }

private:
    std::vector<Polinom> polinoms;

    std::vector<std::string> splitIntoMonoms(const std::string& expr) {
        std::vector<std::string> monoms;
        if (expr.empty()) {
            return monoms;
        }

        size_t start = 0;
        for (size_t i = 0; i < expr.size(); ++i) {
            if (expr[i] == '+' || expr[i] == '-') {
                if (i != 0) {
                    std::string part = expr.substr(start, i - start);
                    if (!part.empty()) {
                        monoms.push_back(part);
                    }
                }
                start = i;
            }
        }

        std::string lastPart = expr.substr(start);
        if (!lastPart.empty()) {
            monoms.push_back(lastPart);
        }

        if (monoms.empty()) {
            throw std::invalid_argument("No valid monoms found");
        }

        return monoms;
    }

    Monom parseMonom(const std::string& monomStr) {
        if (monomStr.empty()) {
            throw std::invalid_argument("Empty monom");
        }

        double sign = 1.0;
        size_t pos = 0;

        if (monomStr[pos] == '+') {
            sign = 1.0;
            pos++;
        }
        else if (monomStr[pos] == '-') {
            sign = -1.0;
            pos++;
        }

        if (pos >= monomStr.size()) {
            throw std::invalid_argument("Monom has only a sign");
        }

        double coeff = 1.0;
        bool hasCoeff = false;
        size_t coeffEnd = pos;

        while (coeffEnd < monomStr.size() &&
            (std::isdigit(monomStr[coeffEnd]) || monomStr[coeffEnd] == '.')) {
            coeffEnd++;
            hasCoeff = true;
        }

        if (hasCoeff) {
            std::string coeffStr = monomStr.substr(pos, coeffEnd - pos);
            try {
                coeff = std::stod(coeffStr);
            }
            catch (...) {
                throw std::invalid_argument("Invalid coefficient: " + coeffStr);
            }
            pos = coeffEnd;
        }

        coeff *= sign;

        if (pos >= monomStr.size()) {
            return Monom(coeff, 0, 0, 0);
        }

        int x = 0, y = 0, z = 0;
        std::string rest = monomStr.substr(pos);
        size_t len = rest.size();

        for (size_t i = 0; i < len; ) {
            if (rest[i] == 'x' || rest[i] == 'y' || rest[i] == 'z') {
                char var = rest[i];
                i++;
                int degree = 1;

                if (i < len && rest[i] == '^') {
                    i++;
                    size_t startDegree = i;
                    while (i < len && std::isdigit(rest[i])) {
                        i++;
                    }
                    if (i == startDegree) {
                        throw std::invalid_argument("Missing degree for " + std::string(1, var));
                    }
                    degree = std::stoi(rest.substr(startDegree, i - startDegree));
                }

                switch (var) {
                case 'x': x = degree; break;
                case 'y': y = degree; break;
                case 'z': z = degree; break;
                }
            }
            else {
                throw std::invalid_argument("Unexpected character: " + std::string(1, rest[i]));
            }
        }

        return Monom(coeff, x, y, z);
    }

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

#endif // __ARTANS_H__
