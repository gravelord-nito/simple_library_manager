#pragma once

#include <cstddef>

template <typename T> class Queue {
public:
  static const std::size_t Initial_capacity = 5;

  T *arr;
  size_t capacity;
  size_t size;
  size_t front;
  size_t rear;

  void resize();

public:
  Queue();
  ~Queue();

  void clear();
  bool empty() const;
  void push(T);
  void pop();
  T get() const;
};

#include "Queue.cpp"
