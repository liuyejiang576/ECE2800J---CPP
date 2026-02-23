#ifndef INTSET_H
#define INTSET_H

class IntSetFull { };

class IntSet {
    // OVERVIEW: mutable set of ints with bounded size
public:
    virtual void insert(int v) = 0;
    // MODIFIES: this
    // EFFECTS: set=set+{v}, throws IntSetFull if full
    virtual void remove(int v) = 0;
    // MODIFIES: this
    // EFFECTS: set=set-{v} 
    virtual bool query(int v) = 0;
    // EFFECTS: returns true if v is in set,
    //          false otherwise
    virtual int  size() = 0;
    // EFFECTS: returns |set|
    virtual void print() = 0;
    // EFFECTS: print the element in the set
};

IntSet *getIntSet();
// EFFECTS: returns a pointer to the IntSet

#endif
