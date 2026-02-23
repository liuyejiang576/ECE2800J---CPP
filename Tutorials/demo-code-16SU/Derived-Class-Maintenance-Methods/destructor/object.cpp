#include "object.h"

Base &Base::operator=(const Base &rhs)
{
    if(this != &rhs)
    {
        i = rhs.i;
    }
    return *this;
}

Derived &Derived::operator=(const Derived &rhs)
{
    if(this != &rhs)
    {
        Base::operator=(rhs);
        d = rhs.d;
    }
    return *this;
}
