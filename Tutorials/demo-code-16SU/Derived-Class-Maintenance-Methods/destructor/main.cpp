#include <iostream>
#include "object.h"
using namespace std;

int main(int argc, char *argv[])
{
    Derived x(3,4);
    cout << "x: Base i = " << x.get_Base_i() << "; d = " << x.get_d() << endl;
    return 0;
}
