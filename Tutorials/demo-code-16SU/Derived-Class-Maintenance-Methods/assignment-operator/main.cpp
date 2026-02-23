#include <iostream>
#include "object.h"
using namespace std;

int main(int argc, char *argv[])
{
    Derived x(3,4);
    cout << "x: Base i = " << x.get_Base_i() << "; d = " << x.get_d() << endl;
    cout << "Call the copy constructor to create y from x" << endl;
    Derived y(x);
    cout << "y: Base i = " << y.get_Base_i() << "; d = " << y.get_d() << endl;
    Derived z;
    cout << "z: Base i = " << z.get_Base_i() << "; d = " << z.get_d() << endl;
    cout << "Assign z as x" << endl;
    z = x;
    cout << "z: Base i = " << z.get_Base_i() << "; d = " << z.get_d() << endl;
    return 0;
}
