#ifndef BOX_H
#define BOX_H

#include <iostream>

class Box {
private:
    int* length;
    int* width;
    int* height;

public:
    Box(int l = 10, int w = 20, int h = 30)
        : length(new int(l)),
          width(new int(w)),
          height(new int(h)) {}
    ~Box() {
        delete length;
        delete width;
        delete height;
    }
    void set(int l, int w, int h) {
        *length = l;
        *width  = w;
        *height = h;
    }

    void show() {
        std::cout << "length address = " << length << std::endl;
        std::cout << "width address  = " << width  << std::endl;
        std::cout << "height address = " << height << std::endl;
        std::cout << "Length = " << *length << std::endl;
        std::cout << "Width  = " << *width  << std::endl;
        std::cout << "Height = " << *height << std::endl;
    }
};
