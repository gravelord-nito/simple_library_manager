#pragma once

#include "AVLNode.hpp"

template <typename KeyT, typename ValueT> class AVLTree {
public:
  AVLTree();
  ~AVLTree();
  void clear();
  void insert(const KeyT &, const ValueT &);
  void remove(const KeyT &);
  const ValueT find(const KeyT &);
  void display() const;

private:
  AVLNode<KeyT, ValueT> *root;

  int getHeight(AVLNode<KeyT, ValueT> *);
  void updateHeight(AVLNode<KeyT, ValueT> *);
  AVLNode<KeyT, ValueT> *findMinNode(AVLNode<KeyT, ValueT> *);
  AVLNode<KeyT, ValueT> *rightRotate(AVLNode<KeyT, ValueT> *);
  AVLNode<KeyT, ValueT> *leftRotate(AVLNode<KeyT, ValueT> *);
  int getBalanceFactor(AVLNode<KeyT, ValueT> *);
  AVLNode<KeyT, ValueT> *insert(AVLNode<KeyT, ValueT> *, const KeyT &,
                                const ValueT &);
  AVLNode<KeyT, ValueT> *deleteNode(AVLNode<KeyT, ValueT> *, const KeyT &);
  void inOrderTraversal(AVLNode<KeyT, ValueT> *) const;
  void clear(AVLNode<KeyT, ValueT> *);
  const AVLNode<KeyT, ValueT> *findNode(const KeyT &, AVLNode<KeyT, ValueT> *);
};

#include "AVLTree.cpp"
