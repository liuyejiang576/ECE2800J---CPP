#include "IntSet.h"
#include <iostream>

using namespace std;

const int MAXELTS = 100;

class IntSetImpl: public IntSet
{
    // OVERVIEW: a mutable set of integers, |set| <= MAXELTS
    int       elts[MAXELTS];
    int       numElts;
    int       indexOf(int v);
    // EFFECTS: returns the index of v if it exists in the
    //          array, MAXELTS otherwise.
public:
    IntSetImpl();
    // EFFECTS: default constructor. Creates an empty IntSetImpl.
    void insert(int v);
    // MODIFIES: this
    // EFFECTS: this = this + {v} if room, 
    //          throws IntSetFull otherwise.
    void remove(int v);
    // MODIFIES: this
    // EFFECTS: this = this - {v} if v is in this
    bool query(int v);
    // EFFECTS: returns true if v is in this, false otherwise.
    int  size();
    // EFFECTS: returns |this|.
    void print();
    // MODIFIES: cout
    // EFFECTS: print out the integers contained in the set in
    //          sequence.
};

IntSetImpl::IntSetImpl(): numElts(0)
{
}

int IntSetImpl::indexOf(int v)
{
    for (int i = 0; i < numElts; i++)
    {
        if (elts[i] == v)
            return i;
    }
    return MAXELTS;
}

int IntSetImpl::size()
{
  return numElts;
}

bool IntSetImpl::query(int v)
{
  return (indexOf(v) != MAXELTS);
}

void IntSetImpl::insert(int v)
{
    if (indexOf(v) == MAXELTS)
    {
        if (numElts == MAXELTS) 
        {
            IntSetFull e;
            throw e; 
        }
        elts[numElts++] = v;
    }
}

void IntSetImpl::remove(int v)
{
    int victim = indexOf(v);
    if (victim != MAXELTS)
    {
        elts[victim] = elts[numElts-1];
        numElts--;
    }
}

void IntSetImpl::print()
{
    for (int i = 0; i < numElts; i++)
    {
        cout << elts[i] << " " << flush;
    }
    cout << endl;
}

static IntSetImpl impl;

IntSet *getIntSet()
{
    return &impl;
}
