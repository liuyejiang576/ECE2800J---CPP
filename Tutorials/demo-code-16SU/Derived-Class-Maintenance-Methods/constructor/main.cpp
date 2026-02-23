#include <iostream>
#include "object.h"
using namespace std;

int main(int argc, char *argv[])
{
    Derived o(3,4);
    cout << "Base i = " << o.get_Base_i() << "; d = " << o.get_d() << endl;
    return 0;
}
