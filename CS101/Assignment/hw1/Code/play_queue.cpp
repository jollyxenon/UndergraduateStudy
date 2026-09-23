template <typename T> class Queue {
private:
  Stack<T> in, out;

public:
  bool isEmpty() const { return in.isEmpty() && out.isEmpty(); }

  void enqueue(const T &value) { in.push(value); }

  T dequeue() {
    if (out.isEmpty()) {
      while (!in.isEmpty()) {
        out.push(in.pop());
      }
    }
    if (out.isEmpty())
      throw std::runtime_error("Queue is empty");
    return out.pop();
  }
};