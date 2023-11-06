#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <map>
#include "stack.cpp"
#include "queue.cpp"

std::map<std::string, int> operationPriority = {
        {"+",   1},
        {"-",   1},
        {"*",   2},
        {"/",   2},
        {"^",   3},
        {"sin", 4},
        {"cos", 4},
        {"tan", 4},
        {"ctg", 4},
        {"(",   5},
};

bool is_func(const std::string &func) {
    return func == "sin" || func == "cos" || func == "tan" || func == "ctg";
}

bool is_oper(const std::string &oper) {
    return oper == "+" || oper == "-" || oper == "*" || oper == "/" || oper == "^";
}

bool is_digits(const std::string &str) {
    for (char ch: str) {
        if (!std::isdigit(ch)) return false;
    }
    return true;
}

void check_digits(std::string &digits, std::vector<std::string> &result) {
    if (!digits.empty()) {
        result.push_back(digits);
        digits.clear();
    }
}

void check_chars(std::string &chars, std::vector<std::string> &result) {
    if (!chars.empty()) {
        if (is_func(chars)) {
            result.push_back(chars);
            chars.clear();
        } else {
            throw std::invalid_argument("Invalid function: " + chars);
        }
    }
}


std::vector<std::string> tokenization(const std::string &input) {
    std::vector<std::string> tokens;
    std::string digits, chars;

    for (char symbol: input) {
        if (symbol == '(' || symbol == ')' || is_oper(std::string(1, symbol)) || symbol == 'x') {
            check_digits(digits, tokens);
            check_chars(chars, tokens);
            tokens.emplace_back(1, symbol);
        } else if (std::isdigit(symbol) || symbol == '.') {
            check_chars(chars, tokens);
            digits += symbol;
        } else if (std::isalpha(symbol)) {
            check_digits(digits, tokens);
            chars += symbol;
        } else if (std::isspace(symbol)) {
            continue;
        } else {
            throw std::invalid_argument("Invalid symbol: " + std::string(1, symbol));
        }
    }

    check_chars(chars, tokens);
    check_digits(digits, tokens);

    return tokens;
}

void print(const std::vector<std::string> &postfixTokens, const Stack<std::string> &operations) {
    std::cout << "In queue: ";
    for (const std::string &token: postfixTokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
    operations.display_all();
    std::cout << "-------------------------------------" << std::endl;
}

std::vector<std::string> infix_to_postfix(const std::vector<std::string> &infixTokens) {
    Stack<std::string> operators;
    std::vector<std::string> postfixTokens;
    for (const std::string &token: infixTokens) {
        if (is_digits(token) || token == "x") {
            postfixTokens.push_back(token);
        } else if (is_func(token)) {
            operators.push(token);
        } else if (is_oper(token)) {
            while (!operators.is_empty() && is_oper(operators.top()) &&
                   operationPriority[operators.top()] >= operationPriority[token]) {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (operators.top() != "(") {
                if (operators.is_empty()) {
                    throw std::runtime_error("There are mismatched parentheses.");
                }
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            if (operators.top() == "(") {
                operators.pop();
            }
            if (is_func(operators.top())) {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
        }
        print(postfixTokens, operators);
    }

    while (!operators.is_empty()) {
        if (operators.top() == "(") {
            throw std::runtime_error("There are mismatched parentheses.");
        }
        postfixTokens.push_back(operators.top());
        operators.pop();
        print(postfixTokens, operators);
    }

    return postfixTokens;
}

int main() {
    std::string inputData = "3 + 4 * 2/cos(1 -5 ) ^2 ^ 3";
    std::vector<std::string> tokens = tokenization(inputData);
    std::vector<std::string> postfixTokens = infix_to_postfix(tokens);
    for (const std::string &token: postfixTokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    return 0;
}


