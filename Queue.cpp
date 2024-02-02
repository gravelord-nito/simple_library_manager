#include "Queue.hpp"
#include <stdexcept>

template <typename T> void Queue<T>::resize() {
  T *newArr = new T[capacity * 2];

  for (size_t i = 0; i < size; i++) {
    newArr[i] = arr[(front + i) % capacity];
  }

  delete[] arr;
  arr = newArr;
  capacity *= 2;
  front = 0;
  rear = size;
}

template <typename T>
Queue<T>::Queue() : capacity(Initial_capacity), size(0), front(0), rear(0) {
  arr = new T[capacity];
}

template <typename T> Queue<T>::~Queue() { delete[] arr; }

template <typename T> void Queue<T>::clear() { size = front = rear = 0; }

template <typename T> bool Queue<T>::empty() const { return size == 0; }

template <typename T> void Queue<T>::push(T value) {
  if (size == capacity) {
    resize();
  }

  arr[rear++] = value;
  rear %= capacity;
  size++;
}

template <typename T> void Queue<T>::pop() {
  if (empty()) {
    throw std::runtime_error("empty queue");
  }

  (front += 1) %= capacity;
  size--;
}

template <typename T> T Queue<T>::get() const {
  if (empty()) {
    throw std::runtime_error("empty queue");
  }

  return arr[front];
}
