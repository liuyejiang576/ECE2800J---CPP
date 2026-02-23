#ifndef EX5_H
#define EX5_H

#include <string>

const int MAX_BOOKS = 20;

class Exception {

  std::string message;

public:
  explicit Exception(const std::string &message) : message(message) {}

  std::string what() const {
    return message;
  }
};

struct Book {
  std::string title;
  std::string author;

  // Default constructor
  Book() {}

  // Constructor with parameters
  Book(const std::string &title, const std::string &author) : title(title), author(author) {}
};

class bookInventory {

protected:
  // Dynamic array of books
  Book *books;
  // Number of books in the inventory
  unsigned int numBooks;
  // Flag to check if the inventory is empty
  bool empty;
  // Size of the books array
  unsigned int size;

public:
  /**
   * TODO: Implement the default constructor.
   * @brief Default constructor. Initializes the number of books to 0
   *        and fills the books with default values.
   *        The size of the books array is MAX_BOOKS.
   */
  bookInventory();

  /**
   * TODO: Implement the constructor with parameter.
   * @brief Constructor with parameter. Initializes the number of books to 0
   *        and fills the books with default values.
   *        The size of the books array is maxBooks.
   *
   * @param maxBooks The size of the books array.
   * @throw Exception if maxBooks is less than 1 or greater than MAX_BOOKS.
   *        The exception message should be "Invalid size."
   */
  explicit bookInventory(int maxBooks);

  /**
   * @brief Returns the number of books in the inventory.
   *
   * @return The number of books.
   */
  int getNumBooks() const {
    return (int)this->numBooks;
  }

  /**
   * @brief Returns if the inventory is empty.
   *
   * @return True if the inventory is empty, false otherwise.
   */
  bool isEmpty() const {
    return this->empty;
  }

  /**
   * @brief Returns the size of the books array.
   *
   * @return The size of the books array.
   */
  unsigned int getSize() const {
    return this->size;
  }

  /**
   * @brief Sets the number of books in the inventory.
   *
   * @param numBooks The number of books.
   */
  void setNumBooks(unsigned int numBooks) {
    this->numBooks = numBooks;
  }

  /**
   * @brief Sets if the inventory is empty.
   *
   * @param empty True if the inventory is empty, false otherwise.
   */
  void setEmpty(bool empty) {
    this->empty = empty;
  }

  /**
   * @brief Sets a book in the inventory at a specific index.
   *
   * @param book The book to be set.
   * @param ID The ID of the book to be set. It is guaranteed to be valid.
   */
  void setBook(const Book &book, int ID) {
    books[ID - 1] = book;
  }

  /**
   * TODO: Implement the invariant checker.
   * @brief Checks if the invariants are true.
   *
   * @return True if the invariants are true, false otherwise.
   */
  bool repOK();

  /**
   * TODO: Implement the destructor.
   * @brief Destructor for the bookInventory class.
   */
  ~bookInventory();

  // TODO: Update the functions to accommodate the new invariants.

  /**
   * @brief Adds a book to the inventory.
   *
   * @param book The book to be added.
   * @throw Exception if the inventory is full.
   *        The exception message should be "The inventory is full.".
   */
  void addBook(const Book &book);

  /**
   * @brief Removes a book from the inventory.
   *
   * @param ID The ID of the book to be removed. The ID is the index of the book in the books array + 1.
   * @throw Exception if the ID is invalid.
   *        The exception message should be "Invalid book ID.".
   */
  void removeBook(int ID);

  /**
   * @brief Prints the inventory of books.
   *
   * @throw Exception if the inventory is empty.
   *        The exception message should be "The inventory is empty.".
   */
  void printInventory() const;
};

#endif
