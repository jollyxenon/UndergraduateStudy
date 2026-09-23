#include <iostream>
#include <stdexcept>

template <typename T> class Stack {
private:
  struct Node {
    T data;
    Node *nxt;
    Node(const T &_data, Node *_nxt = nullptr) : data(_data), nxt(_nxt) {}
  };

  // Keep top of the stack
  Node *head;

public:
  Stack() : head(nullptr) {}
  ~Stack() {
    while (!isEmpty()) {
      pop();  
    }
  }

  bool isEmpty() const { return head == nullptr; }

  void push(const T &value) { head = new Node(value, head); }

  T pop() {
    if (isEmpty())
      throw std::runtime_error("Stack is empty");

    Node *temp = head;
    T pop_data = head->data;
    head = head->nxt;
    delete temp;

    return pop_data;
  }

  T top() const {
    if (isEmpty())
      throw std::runtime_error("Stack is empty");

    return head->data;
  }
};