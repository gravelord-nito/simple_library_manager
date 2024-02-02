#pragma once

#include "AVLTree.hpp"
#include "LLNode.hpp"

template <typename KeyT, typename ValueT> class LinkedList {
private:
  LLNode<KeyT, ValueT> *head;

public:
  LinkedList();
  ~LinkedList();

  const bool empty() const;
  void clear();
  void insert(KeyT, ValueT);
  ValueT find(KeyT);
  void remove(KeyT);
  AVLTree<KeyT, ValueT> *toAVL() const;
};

#include "LinkedList.cpp"
