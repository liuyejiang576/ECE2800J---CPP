#ifndef OBJECT_T_T
#define OBJECT_T_T
#include <string>
#include <iostream>
using namespace std;

class Base
{
    int i;
public:
    Base(int _i = 0): i(_i) {}
    Base(const Base &b): i(b.i) {}
    Base &operator=(const Base &b);
    ~Base()
    { cout << "Call base destructor" << endl; }
    int get_i() const { return i; }
};

class Derived: public Base
{
    double d;
public:
    Derived(int _i = 0, double _d = 0): Base(_i), d(_d) {}
    Derived(const Derived &dr): Base(dr), d(dr.d) {}
    Derived &operator=(const Derived &b);
    ~Derived()
    { cout << "Call derived destructor" << endl; }
    double get_d() const { return d; }
    int get_Base_i() const { return get_i(); }
};

#endif
