// #include <fstream>
// #include <cstdarg>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>

using namespace std;

// clear screen
#define cln                                                                    \
  system("cls");                                                               \
  system("clear")

template <typename KeyT, typename ValueT> struct Node {
  KeyT key;
  ValueT value;
  Node *next;

  Node() : next(nullptr) {}

  Node(KeyT _key, ValueT _value) : key(_key), value(_value), next(nullptr) {}
};

template <typename KeyT, typename ValueT> class LinkedList {
private:
  Node<KeyT, ValueT> *head;

public:
  LinkedList() : head(nullptr) {}

  ~LinkedList() { clear(); }

  bool empty() { return head == nullptr; }

  void clear() {
    Node<KeyT, ValueT> *tmp;
    while (!empty()) {
      tmp = head;
      head = head->next;
      delete tmp;
    }
  }

  void insert(KeyT key, ValueT value) {
    Node<KeyT, ValueT> *newNode = new Node<KeyT, ValueT>(key, value);
    newNode->next = head->next;
    head = newNode;
  }

  ValueT find(KeyT key) {
    Node<KeyT, ValueT> *it = head;
    while (it != nullptr && it->key != key) {
      it = it->next;
    }
    return it->value;
  }

  void remove(KeyT key) {
    Node<KeyT, ValueT> *it = head;
    while (it->next != nullptr) {
      if (it->next->key == key) {
        it->next = it->next->next;
        break;
      }
    }
  }

  Node<KeyT, ValueT> pop() {
    if (empty()) {
      throw runtime_error("linked list is empty");
    } else {
      Node<KeyT, ValueT> tmp = *head;
      head = head->next;
      return tmp;
    }
  }
};

template <typename T> class Queue {
private:
  static const size_t Initial_capacity = 5;

  T *arr;
  size_t capacity;
  size_t size;
  size_t front;
  size_t rear;

  void resize() {
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

public:
  Queue() : capacity(Initial_capacity), size(0), front(0), rear(0) {
    arr = new T[capacity];
  }

  ~Queue() { delete[] arr; }

  void clear() { size = front = rear = 0; }

  bool empty() const { return size == 0; }

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

template <typename KeyT, typename ValueT> struct AVLNode {
  KeyT key;
  ValueT value;
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(const KeyT &k, const ValueT &v)
      : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template <typename KeyT, typename ValueT> class AVLTree {
private:
  AVLNode<KeyT, ValueT> *root;

  int getHeight(AVLNode<KeyT, ValueT> *node) {
    return (node != nullptr) ? node->height : 0;
  }

  void updateHeight(AVLNode<KeyT, ValueT> *node) {
    if (node != nullptr) {
      node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
  }

  AVLNode<KeyT, ValueT> *rightRotate(AVLNode<KeyT, ValueT> *y) {
    AVLNode<KeyT, ValueT> *x = y->left;
    AVLNode<KeyT, ValueT> *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
  }

  AVLNode<KeyT, ValueT> *leftRotate(AVLNode<KeyT, ValueT> *x) {
    AVLNode<KeyT, ValueT> *y = x->right;
    AVLNode<KeyT, ValueT> *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
  }

  int getBalanceFactor(AVLNode<KeyT, ValueT> *node) {
    return (node != nullptr) ? getHeight(node->left) - getHeight(node->right)
                             : 0;
  }

  AVLNode<KeyT, ValueT> *insert(AVLNode<KeyT, ValueT> *node, const KeyT &key,
                                const ValueT &value) {
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

  AVLNode<KeyT, ValueT> *findMinNode(AVLNode<KeyT, ValueT> *node) {
    AVLNode<KeyT, ValueT> *current = node;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current;
  }

  AVLNode<KeyT, ValueT> *deleteNode(AVLNode<KeyT, ValueT> *node,
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

  void inOrderTraversal(AVLNode<KeyT, ValueT> *node) const {
    if (node != nullptr) {
      inOrderTraversal(node->left);
      cout << node->key << "\n";
      inOrderTraversal(node->right);
    }
  }

  void clear(AVLNode<KeyT, ValueT> *node) {
    if (node != nullptr) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

public:
  AVLTree() : root(nullptr) {}

  ~AVLTree() { clear(root); }

  void clear() {
    clear(root);
    root = nullptr;
  }

  void insert(const KeyT &key, const ValueT &value) {
    root = insert(root, key, value);
  }

  void remove(const KeyT &key) { root = deleteNode(root, key); }

  void display() const {
    inOrderTraversal(root);
    cout << endl;
  }
};

class Book;
class User;
User *signUp();
User *logIn();

struct Date {
  int year;
  int month;
  int day;
  // TODO operator overloading for ditracting dates
};

struct Book {
  string title;
  string publish_date;
  string author;
  Queue<User *> reserves;
  Date last_borrow_date; // todo jaarime kon yaroo ro
};

class User {
protected:
  string password; // TODO add hashing
public:
  string first_name;
  string last_name;
  string national_id;
  string username;
  LinkedList<string, Book *> books;

  friend User *signUp();
  friend User *logIn();
};

class Admin : protected User {
private:
  static bool exists;

public:
  Admin() {
    if (exists) {
      throw runtime_error("class is singleton");
    } else {
      exists = 1;
      username = "admin";
      password = "1234";
    }
  }
} admin;
bool Admin::exists = 0;

LinkedList<string, User *> users;
LinkedList<string, Book *> books;

User *signUp() {
  User *newUser;

  cout << "Enter your National ID: ";
  cin >> newUser->national_id;

  cout << "Enter your First Name: ";
  cin >> newUser->first_name;

  cout << "Enter your Last Name: ";
  cin >> newUser->last_name;

  do {
    cout << "Enter a username: ";
    cin >> newUser->username;
    cout << "Username already exists!\n";
  } while (users.find(newUser->username) != nullptr);

  cout << "Enter a password: ";
  cin >> newUser->password;

  cout << "User successfully registered!\n";

  users.insert(newUser->username, newUser);
  return newUser;
}

User *logIn() {
  User user, *existingUser;
  do {
    cout << "Enter your username: ";
    cin >> user.username;

    cout << "Enter your password: ";
    cin >> user.password;

    existingUser = users.find(user.username);
    cout << "Wrong username or password\n";
  } while (existingUser != nullptr && existingUser->password == user.password);

  cout << "Login successful! Welcome, " << existingUser->username << "!\n";

  return existingUser;
}

void addBook() {
  Book *book;

  std::cout << "Enter Book Title: ";
  std::getline(std::cin, book->title);

  std::cout << "Enter Publish Date: ";
  std::getline(std::cin, book->publish_date);

  std::cout << "Enter Author: ";
  std::getline(std::cin, book->author);

  books.insert(book->title, book);
}

void showBooks(LinkedList<string, Book *> list) {
  AVLTree<string, Book *> tree;
  Node<string, Book *> node;
  while (!list.empty()) {
    node = list.pop();
    tree.insert(node.key, node.value);
  }
  tree.display(); // CODED TREE DELETE FUNCTION FOR NothiNG
}

void searchBook() {
  string title;
  cout << "Enter title: ";
  cin >> title;
  Book *book = books.find(title);
  if (book == nullptr) {
    cout << "Book not found!\n";
  } else {
    cout << book->title << endl;
    cout << book->author << endl;
  }
}

void reserveBook() {
  string title, username;
  cout << "Enter title of the book: ";
  cin >> title;
  cout << "Enter username of borrower: ";
  cin >> username;
  User *borrower = users.find(username);
  Book *book = books.find(title);
  // No error hanlding bellow
  book->reserves.push(borrower);
}

void takeBookBack() {
  string title;
  cout << "Enter title of the book: ";
  cin >> title;
  Book *book = books.find(title);
  User *borrower = book->reserves.get(); // TODO apply jarime here
  // No error hanlding bellow
  book->reserves.pop();
}

int main() {

  cln;

  int choice;
  bool flag;
  User *currentUser = nullptr;
  do {
    flag = 1;

    if (currentUser == nullptr) {
      cout << "Menu:\n";
      cout << "1. Sign Up\n";
      cout << "2. Log In\n";
      cout << "3. Exit\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cln;

      switch (choice) {
      case 1:
        currentUser = signUp();
        break;
      case 2:
        currentUser = logIn();
        break;
      case 3:
        flag = 0;
        cout << "Exiting program. Goodbye!\n";
        break;
      default:
        cout << "Invalid choice. Please enter a valid option.\n";
      }
    } else if (currentUser->username == "admin") {
      cout << "Menu:\n";
      cout << "1. Add a book\n";
      cout << "2. Reserve a book\n";
      cout << "2. Take a book back\n";
      cout << "4. Logout\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cln;

      switch (choice) {
      case 1:
        addBook();
        break;
      case 2:
        reserveBook();
        break;
      case 3:
        takeBookBack();
      case 4:
        currentUser = nullptr;
        cout << "Logged out!\n";
        break;
      default:
        cout << "Invalid choice. Please enter a valid option.\n";
      }
    } else {
      cout << "Menu:\n";
      cout << "1. Show all books\n";
      cout << "2. Show my books\n";
      cout << "3. Search books\n";
      cout << "4. Logout\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cln;

      switch (choice) {
      case 1:
        showBooks(books);
        break;
      case 2:
        showBooks(currentUser->books);
        break;
      case 3:
        searchBook();
      case 4:
        currentUser = nullptr;
        cout << "Logged out!\n";
        break;
      default:
        cout << "Invalid choice. Please enter a valid option.\n";
      }
    }
  } while (flag);

  /*
    string choice, file_line;
    // SignUp/Login
    int LoggedIn = 0, user_not_found = 0;
    cout << "Hey\nWelcome to \"Library\"\n";
    while (!LoggedIn) {
      if (user_not_found) {
        cln;
        cout << "sorry, i couldn't find you account, please try again!\n";
      }
      fstream file;
      file.open("accs.txt");
      cout << "What can i do for you (L for Login, S for signup) ? : ";
      cin >> choice;
      if (choice == "S" || choice == "s") {
        cln;
        int exist = 0;
        string username, password;
        while (1) {
          cout << "Enter you username : ";
          cin >> username;
          while (getline(file, file_line)) {
            if (file_line.rfind(username, 0) == 0) {
              exist = 1;
              break;
            }
          }
          if (exist) {
            cout << "this username is already have been taken, please try "
                    "another one\n";
            continue;
          }
          file.close();
          file.open("accs.txt", ios_base::app);
          string line;
          cout << "Enter Your password : ";
          cin >> password;
          line = username + "&password:" + password + "\n";
          file << line;
          cln;
          cout << "your account successfully created, now you can login\n";
          break;
        }
      } else if (choice == "L" || choice == "l") {
        cln;
        string username, password;
        cout << "Enter you username : ";
        cin >> username;
        while (getline(file, file_line)) {
          if (file_line.rfind(username, 0) == 0) {
            LoggedIn = 1;
            break;
          }
        }
        if (LoggedIn) {
          cout << "Enter Your password : ";
          cin >> password;
          string PassChecker = "&password=";
          PassChecker.append(password);
          while (file_line.find(PassChecker) == string::npos) {
            cout << "password is incorrect !\nplease try again : ";
            cin >> password;
            string PassChecker = "&password=";
            PassChecker.append(password);
          }
          cln;
          cout << "welcome!";
        }
      } else {
        cout << "i can't undrstand, what do you want ? (L for login, S for "
                "signup) :";
      }
      file.close();

      if (!LoggedIn) {
        break;
      }
    }
  */

  return 0;
}
