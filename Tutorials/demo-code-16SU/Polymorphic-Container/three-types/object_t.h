#ifndef OBJECT_T_T
#define OBJECT_T_T

class Object
{
public:
	virtual Object *clone() = 0;
	virtual ~Object() {}

    virtual void print() const = 0;
};

#endif
