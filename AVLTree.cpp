#include "AVLTree.hpp"
#include <iostream>
#include <stdexcept>

template <typename KeyT, typename ValueT>
AVLTree<KeyT, ValueT>::AVLTree() : root(nullptr) {}

template <typename KeyT, typename ValueT> AVLTree<KeyT, ValueT>::~AVLTree() {
  clear(root);
}

template <typename KeyT, typename ValueT> void AVLTree<KeyT, ValueT>::clear() {
  clear(root);
  root = nullptr;
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::insert(const KeyT &key, const ValueT &value) {
  root = insert(root, key, value);
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::remove(const KeyT &key) {
  root = deleteNode(root, key);
}

template <typename KeyT, typename ValueT>
const ValueT AVLTree<KeyT, ValueT>::find(const KeyT &key) {
  auto ret = findNode(key, root);
  return ret == nullptr ? nullptr : ret->value;
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::display() const {
  inOrderTraversal(root);
  std::cout << std::endl;
}

template <typename KeyT, typename ValueT>
int AVLTree<KeyT, ValueT>::getHeight(AVLNode<KeyT, ValueT> *node) {
  return (node != nullptr) ? node->height : 0;
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::updateHeight(AVLNode<KeyT, ValueT> *node) {
  if (node != nullptr) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
  }
}

template <typename KeyT, typename ValueT>
AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::rightRotate(AVLNode<KeyT, ValueT> *y) {
  AVLNode<KeyT, ValueT> *x = y->left;
  AVLNode<KeyT, ValueT> *T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);

  return x;
}

template <typename KeyT, typename ValueT>
AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::leftRotate(AVLNode<KeyT, ValueT> *x) {
  AVLNode<KeyT, ValueT> *y = x->right;
  AVLNode<KeyT, ValueT> *T2 = y->left;

  y->left = x;
  x->right = T2;

  updateHeight(x);
  updateHeight(y);

  return y;
}

template <typename KeyT, typename ValueT>
int AVLTree<KeyT, ValueT>::getBalanceFactor(AVLNode<KeyT, ValueT> *node) {
  return (node != nullptr) ? getHeight(node->left) - getHeight(node->right) : 0;
}

template <typename KeyT, typename ValueT>
AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::insert(AVLNode<KeyT, ValueT> *node, const KeyT &key,
                              const ValueT &value) {
  if (node == nullptr) {
    return new AVLNode<KeyT, ValueT>(key, value);
  }

  if (key < node->key) {
    node->left = insert(node->left, key, value);
  } else if (key > node->key) {
    node->right = insert(node->right, key, value);
  } else {
    throw std::runtime_error("duplicate keys");
  }

  updateHeight(node);

  int balance = getBalanceFactor(node);

  // Left Left Case
  if (balance > 1 && getBalanceFactor(node->left) >= 0) {
    return rightRotate(node);
  }

  // Left Right Case
  if (balance > 1 && getBalanceFactor(node->left) < 0) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // Right Right Case
  if (balance < -1 && getBalanceFactor(node->right) <= 0) {
    return leftRotate(node);
  }

  // Right Left Case
  if (balance < -1 && getBalanceFactor(node->right) > 0) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // No Rotations needed
  return node;
}

template <typename KeyT, typename ValueT>
AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::findMinNode(AVLNode<KeyT, ValueT> *node) {
  AVLNode<KeyT, ValueT> *current = node;
  while (current->left != nullptr) {
    current = current->left;
  }
  return current;
}

template <typename KeyT, typename ValueT>
AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::deleteNode(AVLNode<KeyT, ValueT> *node,
                                  const KeyT &key) {
  if (node == nullptr) {
    return node;
  }

  if (key < node->key) {
    node->left = deleteNode(node->left, key);
  } else if (key > node->key) {
    node->right = deleteNode(node->right, key);
  } else {
    AVLNode<KeyT, ValueT> *tmp;
    if (node->left == nullptr || node->right == nullptr) {
      tmp = node->left ? node->left : node->right;

      if (tmp == nullptr) {
        tmp = node;
        node = nullptr;
      } else {
        *node = *tmp;
      }
      delete tmp;
    } else {
      tmp = findMinNode(node->right);

      node->key = tmp->key;
      node->value = tmp->value;

      node->right = deleteNode(node->right, tmp->key);
    }
  }

  if (node == nullptr) {
    return node;
  }

  updateHeight(node);

  int balance = getBalanceFactor(node);

  // Left Left Case
  if (balance > 1 && getBalanceFactor(node->left) >= 0) {
    return rightRotate(node);
  }

  // Left Right Case
  if (balance > 1 && getBalanceFactor(node->left) < 0) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // Right Right Case
  if (balance < -1 && getBalanceFactor(node->right) <= 0) {
    return leftRotate(node);
  }

  // Right Left Case
  if (balance < -1 && getBalanceFactor(node->right) > 0) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // No Rotations needed
  return node;
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::inOrderTraversal(
    AVLNode<KeyT, ValueT> *node) const {
  if (node != nullptr) {
    inOrderTraversal(node->left);
    std::cout << node->key << "\n";
    inOrderTraversal(node->right);
  }
}

template <typename KeyT, typename ValueT>
void AVLTree<KeyT, ValueT>::clear(AVLNode<KeyT, ValueT> *node) {
  if (node != nullptr) {
    clear(node->left);
    clear(node->right);
    delete node;
  }
}

template <typename KeyT, typename ValueT>
const AVLNode<KeyT, ValueT> *
AVLTree<KeyT, ValueT>::findNode(const KeyT &key, AVLNode<KeyT, ValueT> *node) {
  if (node == nullptr || key == node->key) {
    return node;
  } else if (key < node->key) {
    return findNode(key, node->left);
  } else {
    return findNode(key, node->right);
  }
}
