#include <iostream>
#include <cassert>
#include "foo.h"
using namespace std;

int main(int argc, char *argv[])
{
    foo f;
    cout << f.get_int() << endl;
    cout << f.get_str() << "$" << endl;
    
    return 0;
}
