#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


template <typename T>
struct Node {
  T data;
  Node* next;

  Node(T value): data(value), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
  Node<T>* head;

public:
  
  LinkedList(): head(nullptr) {}

  ~LinkedList() {
    clear();
  }

  void clear() {
    Node<T>* tmp;
    while (head != nullptr) {
      tmp = head;
      head = head->next;
      delete tmp;
    }
  }

  void insert(T value) {
    Node<T>* newNode = new Node<T>(value);
    newNode->next = head->next;
    head = newNode;
  }

};

template <typename T>
class Queue {
private:
  static const size_t Initial_capacity = 5;

  T* arr;
  size_t capacity;
  size_t size;
  size_t front;
  size_t rear;

  void resize() {
    T* newArr = new T[capacity * 2];

    for (size_t i = 0; i < size; i++) {
      newArr[i] = arr[(front + i) % capacity];
    }

    delete[] arr;
    arr = newArr;
    capacity *= 2;
    front = 0;
    rear = size;

  }

public:
  Queue()
    : capacity(Initial_capacity), size(0), front(0), rear(0) 
  {
      arr = new T[capacity];
  }

  ~Queue() {
    delete[] arr;
  }

  void clear() {
    size = front = rear = 0;
  }

  bool empty() const {
    return size == 0;
  }

  void push(T value) {
    if (size == capacity) {
      resize();
    }

    arr[rear++] = value;
    rear %= capacity;
    size++;
  }

  void pop() {
    if (empty()) {
      throw runtime_error("empty queue");
    }

    (front += 1) %= capacity;
    size--;
  }

  T get() const {
    if (empty()) {
      throw runtime_error("empty queue");
    }

    return arr[front];
  }
};

template<typename KeyT, typename ValueT>
struct AVLNode {
  KeyT key;
  ValueT value;
  AVLNode* left;
  AVLNode* right;
  int height;

  AVLNode(const KeyT& k, const ValueT& v)
    : key(k), value(v), left(nullptr), right(nullptr), height(1) {}

};

template<typename KeyT, typename ValueT>
class AVLTree {
private:
  AVLNode<KeyT, ValueT>* root;


  int getHeight(AVLNode<KeyT, ValueT>* node) {
    return (node != nullptr) ? node->height : 0;
  }

  void updateHeight(AVLNode<KeyT, ValueT>* node) {
    if (node != nullptr) {
      node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
  }

  AVLNode<KeyT, ValueT>* rightRotate(AVLNode<KeyT, ValueT>* y) {
    AVLNode<KeyT, ValueT>* x = y->left;
    AVLNode<KeyT, ValueT>* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
  }

   AVLNode<KeyT, ValueT>* leftRotate(AVLNode<KeyT, ValueT>* x) {
    AVLNode<KeyT, ValueT>* y = x->right;
    AVLNode<KeyT, ValueT>* T2 = y->left;


    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
  }

  int getBalanceFactor(AVLNode<KeyT, ValueT>* node) {
    return (node != nullptr) ? getHeight(node->left) - getHeight(node->right) : 0;
  }

  AVLNode<KeyT, ValueT>* insert(AVLNode<KeyT, ValueT>* node, const KeyT& key, const ValueT& value) {
    if (node == nullptr) {
      return new AVLNode<KeyT, ValueT>(key, value);
    }

    if (key < node->key) {
      node->left = insert(node->left, key, value);
    } else if (key > node->key) {
      node->right = insert(node->right, key, value);
    } else {
      throw runtime_error("duplicate keys");
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

  AVLNode<KeyT, ValueT>* findMinNode(AVLNode<KeyT, ValueT>* node) {
    AVLNode<KeyT, ValueT>* current = node;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current;
  }

  AVLNode<KeyT, ValueT>* deleteNode(AVLNode<KeyT, ValueT>* node, const KeyT& key) {
    if (node == nullptr) {
      return node;
    }

    if (key < node->key) {
      node->left = deleteNode(node->left, key);
    } else if (key > node->key) {
      node->right = deleteNode(node->right, key);
    } else {
      AVLNode<KeyT, ValueT>* tmp;
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

    if (node == nullptr) { // HACK guts told me to do so
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

  void inOrderTraversal(AVLNode<KeyT, ValueT>* node) const {
    if (node != nullptr) {
      inOrderTraversal(node->left);
      cout << "(" << node->key << ", " << node->value << ") ";
      inOrderTraversal(node->right);
    }
  }

  void clear(AVLNode<KeyT, ValueT>* node) {
    if (node != nullptr) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

public:
  AVLTree(): root(nullptr) {}

  ~AVLTree() {
    clear(root);
  }

  void clear() {
    clear(root);
    root = nullptr;
  }

  void insert(const KeyT& key, const ValueT& value) {
    root = insert(root, key, value);
  }

  void remove(const KeyT& key) {
    root = deleteNode(root, key);
  }

  void display() const {
    inOrderTraversal(root);
    cout << endl;
  }

};

class User {
private:
  string first_name;
  string last_name;
  string national_id;
  string username;
  // TODO add password and hashing
  LinkedList books;
};

class Admin: public User {
private:
  // TODO set user and password to hash of 1234? (and save in file)
public:
  Admin() {
    throw "class is singleton";
  }
};

class Book {
private:
  string title;
  string publish_date;
  string author;
  Queue<User> reserves;
};

LinkedList<User> users;
LinkedList<Book> books;

int main() {

  return 0;
}
