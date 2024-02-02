#pragma once

template <typename KeyT, typename ValueT> struct AVLNode {
  KeyT key;
  ValueT value;
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(const KeyT &k, const ValueT &v)
      : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};
