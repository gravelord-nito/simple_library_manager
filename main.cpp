// WARNING no proper error handling whatsoever
// Library admin is considered perfect
#include "AVLTree.hpp"
#include "LinkedList.hpp"
#include "Queue.hpp"
#include <ctime>
#include <iostream>
#include <string>

// one day in seconds, can be changed for testing purposes
const int DAY = 3 * 60;

using namespace std;

#define cln                                                                    \
  system("cls");                                                               \
  system("clear")

class Library { // TODO move this to header and source files?
public:
  const static int RETURN_DEADLINE = 10 * DAY;
  const static int RESERVE_DEADLINE = 3 * DAY;

  class Book;
  class User;
  class Admin;

  class Book {
  private:
    bool available;
    string title;
    string publishDate;
    string author;
    Queue<User *> reserves;
    time_t lastActionDate;

  public:
    Book() { lastActionDate = time(nullptr); }
    friend class Admin;
    friend ostream &operator<<(ostream &os, const Book &book) {
      os << "Title: " << book.title << "\n";
      os << "Author: " << book.author << "\n";
      os << "Publish Date: " << book.publishDate << "\n";
      os << "Book is " << (book.available ? "available\n" : "unavailable\n");
      if (!book.available) {
        os << "Last Borrow Date: " << ctime(&book.lastActionDate);
      }
      return os;
    }
  };

  class User {
  public:
    string username;
    const LinkedList<string, Book *> &getBooks() const { return mybooks; }

    void borrowBook(const string &title, Book *book) {
      mybooks.insert(title, book);
    }

    void returnBook(const string &title) { mybooks.remove(title); }

    friend Library;

  protected:
    string first_name;
    string last_name;
    string national_id;
    string password; // TODO add hashing
    LinkedList<string, Book *> mybooks;
  };

  class Admin : public User {
  private:
    Admin() {
      username = "admin";
      password = "1234";
      library = new Library();
      library->users.insert("admin", this);
    }
    Library *library;

  public:
    ~Admin() { delete library; }

    static Admin &getInstance() {
      static Admin instance;
      return instance;
    }

    Library *getLibrary() { return library; }

    void addBook() {
      Book *book = new Book();

      cout << "Adding a new Book...\n";

      cout << "Enter Book Title: ";
      cin >> book->title;

      cout << "Enter Publish Date: ";
      cin >> book->publishDate;

      cout << "Enter Author: ";
      cin >> book->author;

      book->available = 1;

      library->books.insert(book->title, book);
    }

    tuple<string, string, User *, Book *> getInfo() {
      string title, username;
      cout << "Enter title of the book: ";
      cin >> title;
      cout << "Enter username: ";
      cin >> username;
      User *user = library->users.find(username);
      Book *book = library->books.find(title);
      return make_tuple(title, username, user, book);
    }

    void lendBook() {
      auto [title, __, borrower, book] = getInfo();
      if (!book->available) {
        cout << "Book is not available right now!\n";
        return;
      }
      while (!book->reserves.empty()) {
        time_t reserveDeadline = book->lastActionDate + RESERVE_DEADLINE;
        time_t currentTime = time(nullptr);
        if (currentTime > reserveDeadline) {
          book->reserves.pop();
          book->lastActionDate = reserveDeadline;
        } else {
          break;
        }
      }
      if (!book->reserves.empty()) {
        if (book->reserves.get()->username == borrower->username) {
          book->reserves.pop();
        } else {
          cout << "Someone else reserved the book\n";
          return;
        }
      }

      borrower->borrowBook(title, book);
      book->lastActionDate = time(nullptr);
      book->available = 0;
      cout << " Successfully Lended\n";
    }

    void extendDue() {
      auto [_, __, ___, book] = getInfo();
      if (!book->reserves.empty()) {
        cout << "Can't extend, book has reservers\n";
        return;
      }
      book->lastActionDate = time(nullptr);
      cout << "Extended return date successfully\n";
    }

    void reserveBook() {
      auto [_, __, reserver, book] = getInfo();
      book->reserves.push(reserver);
      cout << "Successfully Reserved\n";
    }

    void takeBookBack() {
      auto [title, __, returner, book] = getInfo();
      time_t returnDeadline = book->lastActionDate + RETURN_DEADLINE;
      time_t currentTime = time(nullptr);
      cout << "User turned book back after "
           << currentTime - book->lastActionDate
           << "seconds\n"; // debug purposes
      if (currentTime > returnDeadline) {
        cout << "Deadline reached, user is fined "
             << (currentTime - returnDeadline) / DAY * 5 << " hezar tomans\n";
      }
      returner->returnBook(title);
      book->available = 1;
      cout << "Book returned successfully\n";
    }
  };

  User *signUp() {
    User *newUser = new User();

    cout << "Enter your National ID: ";
    cin >> newUser->national_id;

    cout << "Enter your First Name: ";
    cin >> newUser->first_name;

    cout << "Enter your Last Name: ";
    cin >> newUser->last_name;

    do {
      cout << "Username already exists!\n";
      cout << "Enter a username: ";
      cin >> newUser->username;
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
    } while (existingUser != nullptr &&
             existingUser->password != user.password);
    cln;
    cout << "Login successful! Welcome, " << existingUser->username << "!\n";

    return existingUser;
  }

  const LinkedList<string, Book *> &getBooks() const { return books; };
  const LinkedList<string, User *> &getUsers() const { return users; };

private:
  LinkedList<string, User *> users;
  LinkedList<string, Book *> books;

  Library() {}
};

bool isAdmin(Library::User *user) { return user->username == "admin"; }

int main() {

  cln;

  int choice;
  bool flag;
  Library::Admin &admin = Library::Admin::getInstance();
  Library *library = admin.getLibrary();
  Library::User *currentUser = nullptr;
  do {
    flag = 1;

    if (currentUser == nullptr) {
      cout << "Menu:\n";
      cout << "1. Log In\n";
      cout << "2. Sign Up\n";
      cout << "3. Exit\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cln;

      switch (choice) {
      case 1:
        currentUser = library->logIn();
        break;
      case 2:
        currentUser = library->signUp();
        break;
      case 3:
        flag = 0;
        cout << "Exiting program. Goodbye!\n";
        break;
      default:
        cout << "Invalid choice. Please enter a valid option.\n";
      }
    } else {
      cout << "Menu:\n";
      cout << "  1. Show all books\n";
      cout << "  2. Show my books\n";
      cout << "  3. Search books\n";
      if (isAdmin(currentUser)) {
        cout << "Admin options: \n";
        cout << "  5. Add a book to the library\n";
        cout << "  6. Lend a book\n";
        cout << "  7. Extend turning back due\n";
        cout << "  8. Reserve a book\n";
        cout << "  9. Take a book back\n";
      }
      cout << "0. Logout\n";
      cout << "Enter your choice: ";
      cin >> choice;
      cln;

      switch (choice) {
      case 0: {
        currentUser = nullptr;
        cout << "Logged out!\n";
        break;
      }
      case 1: {
        auto tree = library->getBooks().toAVL();
        tree->display();
        delete tree;
      } break;
      case 2: {
        auto tree = currentUser->getBooks().toAVL();
        tree->display();
        delete tree;
        break;
      }
      case 3: {
        cout << "Book title: ";
        string title;
        cin >> title;
        auto tree = library->getBooks().toAVL();
        Library::Book *book = tree->find(title);
        if (book == nullptr) {
          cout << "Book not found\n";
        } else {
          cout << *book << '\n';
        }
        delete tree;
        break;
      }
      case 5:
        if (isAdmin(currentUser))
          admin.addBook();
        break;
      case 6:
        if (isAdmin(currentUser))
          admin.lendBook();
        break;
      case 7:
        if (isAdmin(currentUser))
          admin.extendDue();
        break;
      case 8:
        if (isAdmin(currentUser))
          admin.reserveBook();
        break;
      case 9:
        if (isAdmin(currentUser))
          admin.takeBookBack();
        break;
      default:
        cout << "Invalid choice. Please enter a valid option.\n";
      }
    }
  } while (flag);

  return 0;
}
