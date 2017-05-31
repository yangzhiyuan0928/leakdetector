#include <iostream>
#include "leaktrace.h"
using namespace std;
int main() 
{
	MEM_ON();
	TRACE_ON();
	int *a = new int;
	int *b = new int[12];
	delete a;
	return 0;
}
