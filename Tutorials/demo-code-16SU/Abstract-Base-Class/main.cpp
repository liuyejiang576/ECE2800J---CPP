#include <iostream>
#include "IntSet.h"

using namespace std;

int main() 
{
    //IntSet a;
    ///IntSetImpl a;

    
    IntSet *foo = getIntSet();
    cout << "insert 7" << endl;
    foo->insert(7);
    foo->print();

    cout << "insert 3" << endl;
    foo->insert(3);
    foo->print();

    cout << "insert 5" << endl;
    foo->insert(5);
    foo->print();

    cout << "insert 7" << endl;
    foo->insert(7);
    foo->print();

    cout << "remove 5" << endl;
    foo->remove(5);
    foo->print();
    

    return 0;
}
