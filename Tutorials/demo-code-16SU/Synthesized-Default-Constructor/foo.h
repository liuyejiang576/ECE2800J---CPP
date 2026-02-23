#ifndef FOO_T
#define FOO_T
#include <string>
using namespace std;

class foo
{
    int i;
    string s;

public:
    int get_int() const { return i; }
    string get_str() const { return s; }
};

#endif
