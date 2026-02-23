#ifndef OBJECT_T_T
#define OBJECT_T_T
#include <iostream>
#include <string>
using namespace std;

class Base
{
    int i;
public:
    Base(int _i = 0): i(_i)
    { cout << "Call base constructor" << endl; }
    int get_i() const { return i; }
};

class Derived: public Base
{
    double d;
public:
    Derived(int _i = 0, double _d = 0): Base(_i), d(_d)
    { cout << "Call derived constructor" << endl; }
    double get_d() const { return d; }
    int get_Base_i() const { return get_i(); }
};

#endif
