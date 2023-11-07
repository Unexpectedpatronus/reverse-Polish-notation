#include <iostream>
#include <stdexcept>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node *next;

        explicit
        Node(const T &val) : data(val), next(nullptr) {}
    };

    Node *frontNode;
    Node *rearNode;
    size_t size;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    Queue(const Queue &other) : frontNode(nullptr), rearNode(nullptr), size(0) {
        Node *temp = other.frontNode;
        while (temp) {
            push(temp->data);
            temp = temp->next;
        }
    }

    size_t get_size() const {
        return size;
    }

    bool is_empty() const {
        return frontNode == nullptr;
    }

    void push(const T &val) {
        Node *newNode = new Node(val);
        if (is_empty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size++;
    }

    void pop() {
        if (is_empty()) {
            throw std::runtime_error("Queue is empty. Can't pop.");
        }
        Node *temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        size--;
    }

    T front() const {
        if (is_empty()) {
            throw std::runtime_error("Queue is empty. No front element.");
        }
        std::cout << "Front element: " << frontNode->data << std::endl;
        return frontNode->data;
    }

    void display_all() const {
        Node *current = frontNode;
        std::cout << "In queue: ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }


    void clear() {
        while (!is_empty()) {
            pop();
        }
    }

    Queue &operator=(const Queue &other) {
        if (this != &other) {
            clear();
            Node *temp = other.frontNode;
            while (temp) {
                push(temp->data);
                temp = temp->next;
            }
        }
        return *this;
    }

    ~Queue() {
        clear();
    }
};
