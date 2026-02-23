#include <iostream>
#include "IntSet.h"

using namespace std;

void foo(IntSet x)
{
}

int main() 
{
    IntSet s;
    s.insert(5);
    foo(s);

    {
        IntSet x;
        x = s;
    }
    s.query(5);

    return 0;
}
