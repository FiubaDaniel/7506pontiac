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
using namespace std;
class paulo{
public:
	static int a;
	int b;
	paulo(int x){
		b=a;
		++a;
	}
};
int paulo::a=-8;

typedef paulo* pp;
int main() {
	int aa=8;
	char *chr=(char*)&aa;
	/*vector<paulo> vec;
	for(int i=0;i<3;i++){
		paulo unp(3);
		vec.push_back(&unp);
	}
	for(int i=0;i<3;i++){
			cout<<vec.at(i)->b<<" "<<vec.at(i)->a<<endl;
	}*/
	string str;
	stringstream ss(stringstream::in|stringstream::out|stringstream::binary);
	cout<<"ok"<<endl;
	try{
		ss<<"!!!Hello World!!!";
		ss<<aa;
		cout<<"ok"<<endl;
		ss.write((char*)&aa,sizeof(aa));
		cout<<(long)*chr<<endl;
		cout<<"ok"<<endl;
		cout<<ss.rdbuf()->str()<<endl;
		ss>>str;
	}catch(...){
		cout<<"Exception";
	}
	cout << str << endl; // prints !!!Hello World!!!
	return 0;
}
