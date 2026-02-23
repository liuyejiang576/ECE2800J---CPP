#ifndef BIGTHING_T_T
#define BIGTHING_T_T

class BigThing
{
private:
	int value;

public:
	BigThing(int v = 0): value(v) {}

	int get_value() { return value; }
    void print() const;
};

#endif
