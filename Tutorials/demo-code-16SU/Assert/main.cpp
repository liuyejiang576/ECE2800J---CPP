#include <iostream>
//#define NDEBUG
#include <cassert>
using namespace std;

int main(int argc, char *argv[])
{
    int a,b;
    cout << "Please input a and b, where a < b: " << flush;
    cin >> a >> b;
    assert(a < b);
    cout << "Correct" << endl;
    return 0;
}
