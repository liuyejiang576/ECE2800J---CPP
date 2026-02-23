#include "box.h"
int main() {
    Box box1;
    box1.set(10, 20, 30);

    Box box2 = box1;   // uses the default copy constructor

    std::cout << "show box1:" << std::endl;
    box1.show();
    std::cout << "show box2:" << std::endl;
    box2.show();

    return 0;
}