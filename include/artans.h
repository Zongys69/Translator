#ifndef __ARTANS_H__
#define __ARTANS_H__

#include "stack.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <cstring>


class ArithmeticTranslator {
private:
    struct Monom {
        double coefficient;
        std::map<char, int> variables;

        Monom(double coeff = 0.0, std::map<char, int> vars = {})
            : coefficient(coeff), variables(vars) {}

        bool sameVars(const Monom& other) const {
            return variables == other.variables;
        }

        Monom operator*(const Monom& other) const {
            Monom res;
            res.coefficient = coefficient * other.coefficient;
            for (const auto& v : variables) res.variables[v.first] += v.second;
            for (const auto& v : other.variables) res.variables[v.first] += v.second;
            return res;
        }
    };

    struct Polynomial {
        std::vector<Monom> terms;

        void simplify() {
            std::vector<Monom> new_terms;
            for (auto& term : terms) {
                bool found = false;
                for (auto& existing : new_terms) {
                    if (existing.sameVars(term)) {
                        existing.coefficient += term.coefficient;
                        found = true;
                        break;
                    }
                }
                if (!found && term.coefficient != 0) {
                    new_terms.push_back(term);
                }
            }
            terms = new_terms;
        }
    };

    std::map<std::string, Polynomial> variables;

    static void trim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
            }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

public:
    std::string ToPostFix(const std::string& infix) {
        std::string output;
        Stack<char> ops;
        auto tokens = StringAnalyze(infix);

        for (size_t i = 0; i < tokens.size(); ++i) {
            const std::string& token = tokens[i];

            if (isNumber(token) || isalpha(token[0])) {
                output += token + " ";
            }
            else if (token == "(") {
                ops.push('(');
            }
            else if (token == ")") {
                while (!ops.empty() && ops.top() != '(') {
                    output += ops.top();
                    output += " ";
                    ops.pop();
                }
                if (ops.empty()) throw std::invalid_argument("Mismatched parentheses");
                ops.pop();
            }
            else if (token == "~") {  // Обработка унарного минуса
                ops.push('~');
            }
            else if (isOperator(token)) {
                while (!ops.empty() && precedence(ops.top()) >= precedence(token[0])) {
                    output += ops.top();
                    output += " ";
                    ops.pop();
                }
                ops.push(token[0]);
            }
            else if (token == "^") {
                ops.push('^');
            }
            else {
                throw std::invalid_argument("Invalid token: " + token);
            }
        }

        while (!ops.empty()) {
            if (ops.top() == '(') throw std::invalid_argument("Mismatched parentheses");
            output += ops.top();
            output += " ";
            ops.pop();
        }

        return output;
    }

    void processExpression(const std::string& expr) {
        size_t eq_pos = expr.find('=');
        if (eq_pos != std::string::npos) {
            std::string var = expr.substr(0, eq_pos);
            std::string value = expr.substr(eq_pos + 1);
            trim(var);
            trim(value);

            try {
                variables[var] = parseExpression(value);
                std::cout << "Stored '" << var << "' = " << polyToString(variables[var]) << "\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
        else {
            try {
                Polynomial result = parseExpression(expr);
                std::cout << "Result: " << polyToString(result) << "\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
    }

private:
    Polynomial parseExpression(const std::string& expr) {
        std::string postfix = ToPostFix(expr);
        return evaluatePostfix(postfix);
    }

    Polynomial evaluatePostfix(const std::string& postfix) {
        Stack<Polynomial> stack;
        auto tokens = StringAnalyze(postfix);

        for (const auto& token : tokens) {
            if (token.empty()) continue;

          
            if (isNumber(token)) {
                stack.push(Polynomial{ { Monom{ stringToDouble(token), {} } } });
            }
            else if (isalpha(token[0])) {
                if (variables.count(token)) {
                    stack.push(variables[token]);
                }
                else {
                    std::map<char, int> vars{ {token[0], 1} };
                    stack.push(Polynomial{ { Monom{1.0, vars} } });
                }
            }
            else if (token == "~") {
                if (stack.empty()) {
                    throw std::invalid_argument("Invalid unary minus position");
                }
                Polynomial a = stack.top();
                stack.pop();
                for (auto& term : a.terms) {
                    term.coefficient *= -1;
                }
                stack.push(a);
            }
            else if (isOperator(token)) {
                Polynomial b = stack.top(); stack.pop();
                Polynomial a = stack.top(); stack.pop();
                stack.push(applyOperation(a, b, token[0]));
            }
            else if (token == "^") {
                Polynomial exp = stack.top(); stack.pop();
                Polynomial base = stack.top(); stack.pop();
                stack.push(power(base, exp));
            }
        }

        if (stack.sizes() != 1) {
            throw std::invalid_argument("Invalid expression");
        }

        Polynomial res = stack.top();
        res.simplify();
        return res;
    }

    Polynomial applyOperation(const Polynomial& a, const Polynomial& b, char op) {
        switch (op) {
        case '+': return add(a, b);
        case '-': return subtract(a, b);
        case '*': return multiply(a, b);
        case '/': return divide(a, b);
        default: throw std::invalid_argument("Unknown operator");
        }
    }

    Polynomial add(const Polynomial& a, const Polynomial& b) {
        Polynomial res = a;
        for (const auto& term : b.terms) res.terms.push_back(term);
        res.simplify();
        return res;
    }

    Polynomial subtract(const Polynomial& a, const Polynomial& b) {
        Polynomial res = a;
        for (const auto& term : b.terms) {
            Monom neg = term;
            neg.coefficient *= -1;
            res.terms.push_back(neg);
        }
        res.simplify();
        return res;
    }

    Polynomial multiply(const Polynomial& a, const Polynomial& b) {
        Polynomial res;
        for (const auto& termA : a.terms) {
            for (const auto& termB : b.terms) {
                Monom new_term;
                // Правильное умножение коэффициентов
                new_term.coefficient = termA.coefficient * termB.coefficient;

                // Правильное объединение переменных
                new_term.variables = termA.variables;
                for (const auto& var : termB.variables) {
                    new_term.variables[var.first] += var.second;
                }
                res.terms.push_back(new_term);
            }
        }
        res.simplify();
        return res;
    }

    Polynomial divide(const Polynomial& a, const Polynomial& b) {
        // Проверяем что делитель - константа
        if (b.terms.size() != 1 || !b.terms[0].variables.empty()) {
            throw std::invalid_argument("Can only divide by a constant");
        }

        double divisor = b.terms[0].coefficient;
        if (divisor == 0) {
            throw std::invalid_argument("Division by zero");
        }

        Polynomial result;
        for (const auto& term : a.terms) {
            Monom newTerm = term;
            newTerm.coefficient /= divisor;
            result.terms.push_back(newTerm);
        }

        result.simplify();
        return result;
    }

    Polynomial power(const Polynomial& base, const Polynomial& exp) {
        if (exp.terms.size() != 1 || !exp.terms[0].variables.empty()) {
            throw std::invalid_argument("Exponent must be a constant");
        }
        int n = static_cast<int>(exp.terms[0].coefficient);

        Polynomial result;
        result.terms.emplace_back(1.0, std::map<char, int>{});

        for (int i = 0; i < n; ++i) {
            result = multiply(result, base);
        }
        return result;
    }

    std::string polyToString(const Polynomial& poly) {
        std::stringstream ss;
        bool first = true;
        for (const auto& term : poly.terms) {
            if (term.coefficient == 0) continue;

            if (!first) {
                ss << (term.coefficient > 0 ? " + " : " - ");
            }
            else if (term.coefficient < 0) {
                ss << "-";
            }
            first = false;

            double abs_coeff = std::abs(term.coefficient);
            if (abs_coeff != 1.0 || term.variables.empty()) {
                ss << abs_coeff;
            }

            for (const auto& var : term.variables) {
                ss << var.first;
                if (var.second > 1) ss << "^" << var.second;
            }
        }
        return ss.str().empty() ? "0" : ss.str();
    }

    double stringToDouble(const std::string& s) {
        std::istringstream iss(s);
        double d;
        if (!(iss >> d)) throw std::invalid_argument("Invalid number: " + s);
        return d;
    }

    bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        size_t start = 0;
        if (s[0] == '-') {
            if (s.size() == 1) return false;
            start = 1;
        }
        bool has_dot = false;
        for (size_t i = start; i < s.size(); ++i) {
            if (s[i] == '.') {
                if (has_dot) return false;
                has_dot = true;
            }
            else if (!isdigit(s[i])) {
                return false;
            }
        }
        return true;
    }

    bool isOperator(const std::string& s) {
        return s.size() == 1 && strchr("+-*/", s[0]);
    }

    int precedence(char op) {
        switch (op) {
        case '^': return 4;
        case '*': case '/': return 3;
        case '+': case '-': return 2;
        default: return 0;
        }
    }

    std::vector<std::string> StringAnalyze(const std::string& str) {
        std::vector<std::string> tokens;
        std::string current;
        auto flush = [&]() {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            };

        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];

            if (isspace(c)) {
                flush();
                continue;
            }
            if (c == '-' && (i == 0 || tokens.empty() ||
                tokens.back() == "(" || isOperator(tokens.back())))
            {
                flush();
                tokens.push_back("~");
            }
            else if (isalnum(c) || c == '.') {
                if (!current.empty()) {
                    if ((isdigit(current.back()) && isalpha(c)) ||
                        (isalpha(current.back()) && (isdigit(c) || c == '.')))
                    {
                        flush();
                        tokens.push_back("*");
                    }
                }
                current += c;
            }
            else {
                flush();
                if (c == '-' && (i == 0 || str[i - 1] == '(' || isOperator(std::string(1, str[i - 1])))) {
                    current = "~"; // Унарный минус
                }
                else {
                    current = c;
                }
                flush();
            }
        }
        flush();

        return tokens;
    }
};

#endif // __ARTANS_H__