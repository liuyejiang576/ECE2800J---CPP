## Mid Simulation

### (15%) 1. Linux Command

Write a single command that can:

1. (2%) Copy a file named main.cpp to the directory project. Both file and directory are assumed to exist and to be in the current working directory.

__

2. (2%) Change the current working directory to its parent directory. (For example, if your current working directory is /home/john/, then its parent directory is /home/.)

__

3. (3%) List the content including the hidden content of the home directory, with a general command that works for any user. Suppose you are not in your home directory.

__

4. (3%) Delete a directory along with all of the files and subdirectories inside the directory. Suppose that the name of the directory is dir.

__

5. (3%) Compare two files code.cpp and code.old to see whether they are different. Instead of showing the result on the screen, save it in a file called result.txt.

__

## (12%) 2. Building a C++ program

Given three files: p7.cpp, circuit.cpp, and circuit.h. They are built into an executable (p7) using the following (incomplete) Makefile. Line numbers are labeled on the left. For blanks (3a), (4a) and (4b), see below.

```makefile
1|all: p7
2|
3|p7: p7.o circuit.o
4|	g++ -o p7.o circuit.o p7
5|
6|p7.o: p7.cpp
7|	g++ (3a)
8|
9|circuit.o: circuit.cpp
10|	g++ (3b)
11|
12|clean:
13|	rm (4a)
14|
```

1. (2%) There is a syntax error in this file. Where is it? How will you fix it?

__

2. (1%) How could we build p7 with this Makefile?

__

3. (2%) Fill in the blanks (3a) and (3b) so that we are able to build p7 using the command you provide in (2).

__

__

4. (2%) The target clean removes the previously built executable and object files. Fill in the blank (4a) using no more than 10 characters, spaces included.

__

5. (1%) How could we remove the previously built executable and object files with this Makefile?

__

6. (4%) Suppose circuit.h contains the following code:

```c++
// beginning of circuit.h
class Circuit {
    // class content omitted
};
// end of circuit.h
```

What is a potential issue of the above code? How to fix it?

__

__

__

__

## (22%) 3. C++ Programming Language

1. (2%) Given the following code:

```c++
typedef enum {APPLE, ORANGE, PEAR, BANANA, STRAWBERRY} Fruit_t;
const int NUM = 12;
int value[NUM];
for (int i = 0; i < NUM; i++) {
    value[i] = 3 * i + 1;
}
Fruit_t fruit = BANANA;
int z = value[fruit];
```

What is the value of z?

__

2. (3%) Define a function pointer that could point to the following function:

```c++
double volume(double length, double width, double height);
```

__

3. (4%) Suppose that variable v is an int and variable p is a pointer to int. For each of the following expressions, please tell if it is an lvalue or an rvalue.

(a) 2*v: __

(b) *p: __

(c) &v: __

(d) &p: __

4. (5%) Consider the following C++ statements. What is the output?

```c++
void func(int x) {
    if (x < 4) func(++x);
    cout << x << ",";
}
int main() {
    func(1);
    return 0;
}
```

__

5. (7%) What is the output of the following code?

```c++
void f(double x) {
    cout << "f begins" << endl;
    throw x;
    cout << "f ends" << endl;
}
void g(double x) {
    try {
        cout << "g begins" << endl;
        f();
    }
    cout << "g ends" << endl;
}
int main() {
    try {
        cout << "entering try block" << endl;
        g(10);
        cout << "leaving try block" << endl;
    } catch (int e) {
        cout << "Error int: val = " << e << endl;
    } catch (double e) {
        cout << "Error in main double: val = " << e << endl;
    }
    cout << "After catch block" << endl;
    return 0;
}
```

__

__

__

__

__

__

__

__

### (8%) 4. Recursion and Trees

Recall that a binary tree is well-formed if:

- It is an empty tree, or
- It consists of an integer element, called the root element, plus two children, called the left subtree and the right subtree, each of which is a well-formed binary tree.

Based on this definition, we further define a product tree. A binary tree is called a product tree if and only if:

1. It is an empty tree, or
2. It is a leaf (i.e., a tree with a single element), or
3. Its left and right subtrees are both non-empty product trees and its root element equals the product of the root elements of both subtrees.

To help you write the required function, suppose that we have defined a type tree_t for representing a binary tree. Also, the following methods for tree_t are provided:

```c++
bool tree_isEmpty(tree_t tree);
// EFFECTS: returns true if "tree" is empty, false otherwise
int tree_elt(tree_t tree);
// REQUIRES: "tree" is not empty
// EFFECTS: returns the root element of "tree"
tree_t tree_left(tree_t tree);
// REQUIRES: "tree" is not empty
// EFFECTS: returns the left subtree of "tree"
tree_t tree_right(tree_t tree);
// REQUIRES: "tree" is not empty
// EFFECTS: returns the right subtree of "tree"
```

Given the definition of a product tree, write the following function:

```c++
bool is_product_tree(tree_t tree);
// EFFECTS: returns true if "tree" is a product tree, false otherwise
```

Notes: You are allowed to write helper functions. If they are needed, please also write the implementations of these helper functions, including **specification comments**. For simplicity, you can omit the specification comments for the function `is_product_tree`. Your solution must be **recursive**. You may not use loops, global / static variables, and goto-s. You may use branches.

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

## (43%) 5. Splitting a Number

Given integers $n ≥1$, $b ≥0$, $s ≥0$, print all different vectors $\overrightarrow{(x_1, x_2, ..., x_n)}$ of length n such that each entry $x_i$ is an integer in the range $[0, b]$ and the sum of all entries is s. For example, if $n=3$, $b=2$, $s=4$, the output vectors are (0,2,2), (1,1,2), (1,2,1), (2,0,2), (2,1,1), (2,2,0). Note that we do not enforce a particular order on these vectors. As long as your program prints all of them, it is good.

Besides, you are also given the following function:

```c++
int atoi(char* str);
// REQUIRES: "str" is a null-terminated C string
// EFFECTS: if "str" is a string representing an integer, returns that integer
```

Suppose the function that achieves the above requirement is:

```c++
void print_split(int n, int b, int s);
// REQUIRES: n>=1, b>=0, s>=0
// EFFECTS: print all different vectors of length n such that each entry is an integer in the range [0,b] and the sum of all the entries in the vector is s.
```

1. (5%) What are the output vectors by calling function print_split(3, 3, 6)?

__

__

2. (8%) The program you are asked to write takes the values for n, b, and s from command line. To be more concrete, suppose that your program is named isplit, if you run it as ./isplit 3 2 4, then $n=3$, $b=2$, $s=4$. You are asked to write the main function. The main function needs to check the following errors:

- Missing arguments.
- An input value outside its required range.

If any of the above errors happens, the main function issues a corresponding error message and returns. Please feel free to decide the error message. If none of the above errors happens, you can assume that the inputs are valid. Then the main function calls the function print_split to print all the vectors.

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

3. (12%) Testing your function is important. Write 4 boundary test cases for the function print_split. Each case should test different boundary conditions. For each test case, you must provide a description of the test case and the expected behavior for a correct implementation.

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

4. (18%) Implement the function print_split. You are asked to use the list_t type to represent a vector and use the operations provided below. You can write helper functions. If so, also give their implementations together with the specification comments. For simplicity, you can omit the specification comments for the function print_split. Hint: you may want to define a recursive helper function.

The list_t type is used to represent a list. Also, the following methods for list_t are provided:

```c++
bool list_isEmpty(list_t list);
// EFFECTS: returns true if "list" is empty, false otherwise
list_t list_make();
// EFFECTS: returns an empty list
list_t list_prepend(int elt, list_t list);
// EFFECTS: given "list", make a new list consisting of the new element "elt" followed by the elements of the original "list"
list_t list_append(list_t list, int elt);
// EFFECTS: given "list", make a new list consisting of the elements of the original "list" followed by the new element "elt"
int list_first(list_t list);
// REQUIRES: "list" is not empty
// EFFECTS: returns the first element of "list"
list_t list_rest(list_t list);
// REQUIRES: "list" is not empty
// EFFECTS: returns the list containing all but the first element of "list"
void list_print(list_t list);
// MODIFIES: cout
// EFFECTS: prints "list" followed by a new line
```

Note that if there is no such a vector, your program prints nothing. Recall that a list is well-formed if:

- It is an empty list, or
- It is an integer followed by a well-formed list.

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

