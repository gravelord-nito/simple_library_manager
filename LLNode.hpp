#pragma once

template <typename, typename> class LinkedList;
template <typename KeyT, typename ValueT> class LLNode {
private:
  KeyT key;
  ValueT value;
  LLNode *next;

  LLNode() : next(nullptr) {}
  LLNode(const LLNode<KeyT, ValueT> &other)
      : key(other.key), value(other.value), next(nullptr) {}
  LLNode(KeyT _key, ValueT _value) : key(_key), value(_value), next(nullptr) {}

  template <typename, typename> friend class LinkedList;
};
