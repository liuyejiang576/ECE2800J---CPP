#include <iostream>
#include "IntSet.h"

using namespace std;

int main() 
{
    PosIntSet s;
    try {
        cout << "Insert -1 through s itself" << endl;
        s.insert(-1);
        cout << "Insertion successful!" << endl;
    } catch (int i) {
        cout << "Exception thrown" << endl;
        cout << "Insertion failed!" << endl;
    }
    IntSet& r = s;
    try {
        cout << "Insert -1 through the reference to s" << endl;
        r.insert(-1);
        cout << "Insertion successful!" << endl;
    } catch (int i) {
        cout << "Exception thrown" << endl;
        cout << "Insertion failed!" << endl;
    }
/*
    IntSet* p = &s;
    try {
          p->insert(-1);
    } catch (int i) {
          cout << "Exception thrown" << endl;
    }
*/

    // s.print();
    return 0;
}
