#include <iostream>
#include <stdexcept>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node *previous;

        explicit
        Node(const T &value) : data(value), previous(nullptr) {}
    };

    Node *topNode;

public:
    Stack() : topNode(nullptr) {}

    bool is_empty() const {
        return topNode == nullptr;
    }

    void push(const T &value) {
        Node *newNode = new Node(value);
        newNode->previous = topNode;
        topNode = newNode;
    }

    void pop() {
        if (is_empty()) {
            throw std::runtime_error("Stack is empty. Can't pop.");
        }
        Node *temp = topNode;
        topNode = topNode->previous;
        delete temp;

    }

    T top() const {
        if (is_empty()) {
            throw std::runtime_error("Stack is empty. No top element.");
        }
        return topNode->data;


    }

    void display_all() const {
        Node *current = topNode;
        std::cout << "In stack: ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->previous;
        }
        std::cout << std::endl;
    }

    void clear() {
        while (!is_empty()) {
            pop();
        }
    }

    Stack &operator=(const Stack &other) {
        if (this != &other) {
            clear();
            Node *temp = other.topNode;
            Node *prevCopy = nullptr;
            while (temp) {
                Node *newNode = new Node(temp->data);
                if (!prevCopy) {
                    topNode = newNode;
                } else {
                    prevCopy->previous = newNode;
                }
                prevCopy = newNode;
                temp = temp->previous;
            }
        }
        return *this;
    }

    ~Stack() {
        clear();
    }
};