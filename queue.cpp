#include <iostream>

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

    size_t getSize() const {
        return size;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }


    void push(const T &val) {
        Node *newNode = new Node(val);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size++;
    }

    void pop() {
        if (isEmpty()) {
            std::cout << "Queue is empty. Cannot pop." << std::endl;
            return;
        }
        Node *temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        size--;
    }

    T front() const {
        if (isEmpty()) {
            std::cout << "Queue is empty. No front element." << std::endl;
            return T();
        }
        return frontNode->data;
    }

    void display_all() const {
        Node *current = frontNode;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }


    void clear() {
        while (!isEmpty()) {
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
