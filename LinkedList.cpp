#include "LinkedList.hpp"
#include <stdexcept>

template <typename KeyT, typename ValueT>
LinkedList<KeyT, ValueT>::LinkedList() : head(nullptr) {}

template <typename KeyT, typename ValueT>
LinkedList<KeyT, ValueT>::~LinkedList() {
  clear();
}

template <typename KeyT, typename ValueT>
const bool LinkedList<KeyT, ValueT>::empty() const {
  return head == nullptr;
}

template <typename KeyT, typename ValueT>
void LinkedList<KeyT, ValueT>::clear() {
  LLNode<KeyT, ValueT> *tmp;
  while (!empty()) {
    tmp = head;
    head = head->next;
    delete tmp;
  }
}

template <typename KeyT, typename ValueT>
void LinkedList<KeyT, ValueT>::insert(KeyT key, ValueT value) {
  LLNode<KeyT, ValueT> *newNode = new LLNode<KeyT, ValueT>(key, value);
  newNode->next = head;
  head = newNode;
}

template <typename KeyT, typename ValueT>
void LinkedList<KeyT, ValueT>::remove(KeyT key) {
  LLNode<KeyT, ValueT> *it = head;
  if (it->key == key) {
    head = it->next;
    return;
  }
  while (it->next != nullptr) {
    if (it->next->key == key) {
      it->next = it->next->next;
      break;
    }
  }
}

template <typename KeyT, typename ValueT>
ValueT LinkedList<KeyT, ValueT>::find(KeyT key) {
  LLNode<KeyT, ValueT> *it = head;
  while (it != nullptr && it->key != key) {
    it = it->next;
  }
  return it == nullptr ? nullptr : it->value;
}

template <typename KeyT, typename ValueT>
AVLTree<KeyT, ValueT> *LinkedList<KeyT, ValueT>::toAVL() const {
  AVLTree<KeyT, ValueT> *tree = new AVLTree<KeyT, ValueT>();
  LLNode<KeyT, ValueT> *it = head;
  while (it != nullptr) {
    tree->insert(it->key, it->value);
    it = it->next;
  }
  return tree;
}
