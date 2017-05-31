#include <iostream>
#include <vector>
#include "leaktrace.h"
using namespace std;

class MyClass
{
private:
	int *p;
	int example;
public:
	MyClass()
	{
		p = new int(0);		
		example = 0;
	}
	~MyClass()
	{
		delete p;
		p = NULL;
	}
};

void Test()
{
	printf("\nThis is Test:\n");
	int *i = new int(0);
	delete i;
}

void Test2()
{
	printf("\nThis is Test2:\n");
	int *i = NULL; // better for read
	i = new int(0);
	int *&y = i; // pointer's reference
	delete i;

	MyClass *pMyClass = new MyClass();
	delete pMyClass;

	std::vector<MyClass*> myClasses;
	myClasses.push_back(new MyClass());
	myClasses.push_back(new MyClass());
	delete (MyClass *)(myClasses.at(0));
	delete (MyClass *)(myClasses.at(1));

	std::vector<void*> myVector;
	myVector.push_back(new MyClass());
	myVector.push_back(new MyClass());
	delete (MyClass *)(myVector.at(0));
	delete (MyClass *)(myVector.at(1));
	//delete myVector.at(1); // memory leak
}

class Foo
{
	char* s;
public:
	Foo(const char*s)
	{
		this->s = new char[strlen(s) + 1];
		strcpy(this->s, s);
	}
	~Foo()
	{
		delete[] s;
	}
};
void Test3()
{
	printf("\nThis is Test3:\n");
	int* p = new int;
	delete p;
	int* q = new int[3];
	delete[] q;
	int* r;
	//delete r;
	vector<int> v;
	v.push_back(1);
	Foo s("goodbye");
}
int main()
{
	TRACE_ON();
	MEM_ON();
	Test();
	Test2();
	Test3();
	return 0;
}
