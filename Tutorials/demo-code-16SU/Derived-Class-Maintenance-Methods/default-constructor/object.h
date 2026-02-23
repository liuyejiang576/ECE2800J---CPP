#ifndef OBJECT_T_T
#define OBJECT_T_T
#include <string>
using namespace std;

class Base
{
    int i;
public:
    Base(int _i = 1): i(_i) {}
    int get_i() const { return i; }
};

class Derived: public Base
{
    double d;
public:
    Derived(): d(2) {}
    double get_d() const { return d; }
    int get_Base_i() const { return get_i(); }
};

#endif
