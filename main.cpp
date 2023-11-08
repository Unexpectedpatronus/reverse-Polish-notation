#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <map>
#include "stack.cpp"


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
    for (char ch: str)
        if (!(std::isdigit(ch) || ch == '.'))
            return false;
    return true;
}

bool find_x(const std::vector<std::string> &data) {
    for (const auto &token: data)
        if (token == "x")
            return true;
    return false;
}

std::vector<std::string> tokenization(const std::string &input) {
    std::vector<std::string> tokens;
    std::string digits, chars;

    for (char symbol: input) {
        if (symbol == '(' || symbol == ')' || is_oper(std::string(1, symbol)) || symbol == 'x') {
            if (!digits.empty()) {
                tokens.push_back(digits);
                digits.clear();
            }
            if (!chars.empty()) {
                if (is_func(chars)) {
                    tokens.push_back(chars);
                    chars.clear();
                } else {
                    throw std::invalid_argument("Invalid function: " + chars);
                }
            }
            tokens.emplace_back(1, symbol);
        } else if (std::isdigit(symbol) || symbol == '.') {
            if (!chars.empty()) {
                if (is_func(chars)) {
                    tokens.push_back(chars);
                    chars.clear();
                } else {
                    throw std::invalid_argument("Invalid function: " + chars);
                }
            }
            digits += symbol;
        } else if (std::isalpha(symbol)) {
            if (!digits.empty()) {
                tokens.push_back(digits);
                digits.clear();
            }
            chars += symbol;
        } else if (std::isspace(symbol)) {
            continue;
        } else {
            throw std::invalid_argument("Invalid symbol: " + std::string(1, symbol));
        }
    }

    if (!digits.empty()) {
        tokens.push_back(digits);
    }
    if (!chars.empty()) {
        if (is_func(chars)) {
            tokens.push_back(chars);
        } else {
            throw std::runtime_error("Invalid data!");
        }
    }

    return tokens;
}

void print(const std::vector<std::string> &postfixTokens, const Stack<std::string> &operations) {
    std::cout << "In queue: ";
    for (const std::string &token: postfixTokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
    operations.display_all();
    std::cout << "---------------------------------------" << std::endl;
}

std::vector<std::string> infix_to_postfix(const std::vector<std::string> &infixTokens) {
    Stack<std::string> operatorStack;
    std::vector<std::string> postfixTokens;

    for (const std::string &token: infixTokens) {
        if (is_digits(token) || token == "x") {
            postfixTokens.push_back(token);
        } else if (is_func(token)) {
            operatorStack.push(token);
        } else if (is_oper(token)) {
            if (token == "-" && (operatorStack.is_empty() || operatorStack.top() == "(")) {
                postfixTokens.emplace_back("0");
                operatorStack.push(token);
            } else {
                while (!operatorStack.is_empty() && is_oper(operatorStack.top()) &&
                       operationPriority[operatorStack.top()] >= operationPriority[token]) {
                    postfixTokens.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(token);
            }
        } else if (token == "(") {
            operatorStack.push(token);
        } else if (token == ")") {
            while (operatorStack.top() != "(") {
                if (operatorStack.is_empty()) {
                    throw std::runtime_error("There are mismatched parentheses.");
                }
                postfixTokens.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (operatorStack.top() == "(") {
                operatorStack.pop();
            }
            if (is_func(operatorStack.top())) {
                postfixTokens.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }
        print(postfixTokens, operatorStack);
    }

    while (!operatorStack.is_empty()) {
        if (operatorStack.top() == "(") {
            throw std::runtime_error("There are mismatched parentheses.");
        }
        postfixTokens.push_back(operatorStack.top());
        operatorStack.pop();
        print(postfixTokens, operatorStack);
    }

    return postfixTokens;
}

double calculate(const std::vector<std::string> &postfixTokens) {
    Stack<double> numbers;

    for (const std::string &token: postfixTokens) {
        if (is_digits(token)) {
            numbers.push(std::stod(token));
        } else {
            double result = 0;

            try {
                if (is_oper(token)) {
                    if (numbers.get_size() < 2) {
                        throw std::runtime_error("Not enough operands for the operator: " + token);
                    }

                    double b = numbers.top();
                    numbers.pop();
                    double a = numbers.top();
                    numbers.pop();

                    if (token == "+")
                        result = a + b;
                    else if (token == "-")
                        result = a - b;
                    else if (token == "*")
                        result = a * b;
                    else if (token == "/") {
                        result = a / b;
                    } else if (token == "^")
                        result = std::pow(a, b);
                } else {
                    if (numbers.get_size() < 1) {
                        throw std::runtime_error("Not enough operands for the operator: " + token);
                    }

                    double a = numbers.top();
                    numbers.pop();

                    if (token == "sin")
                        result = std::sin(a);
                    else if (token == "cos")
                        result = std::cos(a);
                    else if (token == "tan")
                        result = std::tan(a);
                    else if (token == "ctg")
                        result = 1 / std::tan(a);
                }
            } catch (const std::invalid_argument &ex) {
                std::cerr << ex.what() << std::endl;
                return 1;
            }

            numbers.push(result);
        }
    }

    if (numbers.get_size() != 1) {
        throw std::runtime_error("Invalid number of arguments in Stack.");
    }

    return numbers.top();
}

int main() {
    std::string inputData;
    std::cout << "Enter a mathematical expression:" << std::endl;
    std::cin >> inputData;

//    std::string inputData = "2.5 + 3 / x";
    std::vector<std::string> tokens = tokenization(inputData);
    std::vector<std::string> postfixTokens = infix_to_postfix(tokens);

    if (find_x(postfixTokens)) {
        double xmin, xmax, h;
        std::cout << "Input xmin, xmax and h:" << std::endl;
        std::cin >> xmin >> xmax >> h;

        if (xmin >= xmax || h >= xmax - xmin) {
            std::cerr << "'Xmin' should be less than 'Xmax', and 'h' should be less than their difference."
                      << std::endl;
            return 1;
        }

        double x = xmin;
        while (x < xmax) {
            std::vector<std::string> opz = postfixTokens;
            for (std::string &token: opz) {
                if (token == "x") {
                    token = std::to_string(x);
                }
            }

            std::cout << "For 'x' = " << x << "\tFunction = " << calculate(opz)
                      << std::endl;
            x += h;
        }
    } else {
        std::cout << "Reverse Polish Notation: ";
        for (const std::string &token: postfixTokens) {
            std::cout << token << " ";
        }
        std::cout << "\nResult: " << calculate(postfixTokens) << std::endl;
    }

    return 0;
}
