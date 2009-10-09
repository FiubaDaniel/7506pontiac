//============================================================================
// Name        : dataStorageToolkit.cpp
// Author      : Paulo
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <vector>
#include "tests/Compuesto/AtributoTest.h"
#include "tests/Compuesto/RegistroTest.h"
#include "tests/Compuesto/compuestolib.h"
using namespace std;
struct A{
	int a;
	char b;
};
int main() {
	//AtributoTest a;
	//a.runTest();
	//foo(cout);
	//RegistroTest b;
	//b.runTest();
	A a;
	A b=a;
	return 0;
}
