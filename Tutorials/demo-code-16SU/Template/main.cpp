#include <iostream>
#include "list.h"
using namespace std;

int main(int argc, char *argv[])
{
	List<int> li; // call default constructor
    cout << "li insert 1 at the front" << endl;
	li.insert(1);
    cout << "li insert 5 at the front" << endl;
	li.insert(5);
    cout << "li insert 4 at the front" << endl;
	li.insert(4);
    cout << "remove the front of li" << endl;
    li.remove();
    cout << "print li" << endl;
    // li is (5 1)
    li.print();

    cout << "lp created as a copy of li" << endl;
	List<int> lp(li); // call copy constructor
    cout << "print lp" << endl;
    // lp is (5 1)
    lp.print();

	List<int> ln;
    cout << "Assign ln as a copy of li" << endl;
	ln = li;    // call assignment operator
    cout << "remove the front of ln" << endl;
	ln.remove();
    cout << "print ln" << endl;
    ln.print();

    return 0;
}
