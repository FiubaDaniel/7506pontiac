/*
 * main.cpp
 *
 *  Created on: 31/10/2009
 *      Author: paulo
 */
#include <iostream>
#include <cstdlib>
#include "RecursoAlmacenamiento/EREscrituraDirecta.h"
using namespace std;
void imprimir(const AdministradorDeBuffer &admin){
	for(unsigned int i=0;i<admin.tablaArchivoBuffer.size();i++){
		cout<<admin.tablaArchivoBuffer.at(i).posicionArchivo<<" : ";
		cout<<admin.tablaArchivoBuffer.at(i).posicionBuffer<<" : ";
		cout<<admin.tablaArchivoBuffer.at(i).siguiente->posicionArchivo<<endl;
	};
};
int main(int argc,char* argv[]){
	AdministradorDeBuffer admin(10);
	cout<<"inicio"<<endl;
	for(int i=0;i<10;i++){
		admin.insertar(i);
	}
	imprimir(admin);
	int pos;
	srand ( time(NULL) );
	pos = rand() % 10;
	for(int i=0;i<10;i++){
			admin.insertar(pos);
	}
	imprimir(admin);
	cout.flush();
	return 0;
};

