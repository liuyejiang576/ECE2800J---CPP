#ifndef LIST_H
#define LIST_H
#include <iostream>
using namespace std;

class listIsEmpty
{
};

template <class T>
class List
{
public:
    bool isEmpty();
    void insert(T v);
    T remove();
    void print();

    List();
    List(const List &l);
    List &operator=(const List &l);
    ~List();

private:
    struct node
    {
        T value;
        node* next;
    };

    node *first;
    void removeAll();
    void copyList(node *v);
};

template <class T>
bool List<T>::isEmpty()
{
	return (!first);
}

template <class T>
void List<T>::insert(T v)
{
	node *np = new node;
	np->value = v;
	np->next = first;
	first = np;
}

template <class T>
T List<T>::remove()
{
	node *victim = first;
	if(isEmpty())
	{
		listIsEmpty e;
		throw e;
	}
	
	T result = victim->value;
	first = victim->next;
	delete victim;
	return result;
}

template <class T>
void List<T>::print()
{
    node *p = first;
    while(p)
    {
        cout << p->value << " ";
        p = p->next;
    }
    cout << endl;
}

template <class T>
List<T>::List(): first(0)
{
}

template <class T>
void List<T>::removeAll()
{
	while(!isEmpty())
		remove();
}

template <class T>
List<T>::~List()
{
	removeAll();
}

template <class T>
void List<T>::copyList(node *np)
{
	if(!np) return;
	copyList(np->next);
	insert(np->value);
}

template <class T>
List<T>::List(const List &l): first(0)
{
	copyList(l.first);
}

template <class T>
List<T> &List<T>::operator=(const List &l)
{
	if(this != &l)
	{
		removeAll();
		copyList(l.first);
	}

	return *this;
}

#endif
