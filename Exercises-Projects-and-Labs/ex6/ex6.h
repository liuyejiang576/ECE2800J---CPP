#ifndef EX6_H
#define EX6_H

#include <ostream>

/**
 * @struct TreeNode
 * @brief Represents a node in a binary tree.
 */
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
};

/**
 * @class BinaryTree
 * @brief Implements a simple binary tree data structure.
 */
class BinaryTree {
public:
  /**
   * @brief Default constructor that initializes an empty binary tree.
   * The root node is set to nullptr.
   */
  BinaryTree();

  /**
   * @brief Parameterized constructor to create a binary tree with a given value.
   * The left and right subtrees are empty.
   * @param val The value for the root node of the binary tree.
   */
  explicit BinaryTree(int val);

  /**
   * @brief Parameterized constructor to create a binary tree with given value and subtrees.
   * @param val The value for the root node of the binary tree.
   * @param left The left subtree.
   * @param right The right subtree.
   */
  BinaryTree(int val, const BinaryTree &left, const BinaryTree &right);

  /**
   * @brief Copy constructor that creates a deep copy of another binary tree.
   * @param other The binary tree to copy from.
   */
  BinaryTree(const BinaryTree &other);

  /**
   * @brief Assignment operator that copies the content of another binary tree.
   * @param other The binary tree to assign from.
   * @return A reference to the modified binary tree.
   */
  BinaryTree &operator=(const BinaryTree &other);

  /**
   * @brief Destructor that cleans up the allocated resources of the binary tree.
   */
  ~BinaryTree();

  /**
   * @brief Checks equality between this binary tree and another binary tree.
   * @param other The binary tree to compare with.
   * @return True if the trees are equal, false otherwise.
   */
  bool operator==(const BinaryTree &other) const;

  /**
   * @brief Checks inequality between this binary tree and another binary tree.
   * @param other The binary tree to compare with.
   * @return True if the trees are not equal, false otherwise.
   */
  bool operator!=(const BinaryTree &other) const;

  /**
   * @brief Checks if the binary tree is empty.
   * @return True if the tree is empty, false otherwise.
   */
  bool isEmpty() const;

  /**
   * @brief Calculates the sum of all values in the binary tree.
   * @return The sum of all values. If the tree is empty, return 0.
   */
  int sum() const;

  /**
   * @brief Calculates the depth of the binary tree.
   * @return The depth of the tree. If the tree is empty, return 0.
   */
  int depth() const;

  /**
   * @brief Overloaded output operator to print the binary tree.
   * Performs an in-order traversal of the binary tree and prints each value.
   * In-order traversal visits the left subtree, then the root, and finally the right subtree.
   *
   * For example, consider the following binary tree:
   *
   *      4
   *     / \
   *    2   5
   *   / \
   *  1   3
   *
   * The expected output of the overloaded operator<< for this tree would be:
   * "1 2 3 4 5"
   *
   * @param os The output stream to print the tree.
   * @param tree The binary tree to print.
   * @return The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const BinaryTree &tree);

private:
  TreeNode *root;

  /**
   * @brief Recursively copies all nodes from another tree.
   * @param root The root node of the tree to copy.
   * @return The root node of the copied tree.
   */
  TreeNode *copy(TreeNode *root);

  /**
   * @brief Recursively removes all nodes from the tree, deallocating memory.
   * @param root The root node of the tree to remove.
   */
  void removeAll(TreeNode *root);

  /**
   * @brief Recursively checks equality between two trees.
   * @param root The root node of the first tree.
   * @param other The root node of the second tree.
   * @return True if the trees are equal, false otherwise.
   */
  bool equal(TreeNode *root, TreeNode *other) const;

  // You can add additional private helper methods here.
};

#endif // EX6_H
