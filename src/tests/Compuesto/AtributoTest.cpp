/*
 * AtributoTest.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "AtributoTest.h"

/*#include <iostream>
#include <cstdlib>
#include <map>
#include "RecursoAlmacenamiento/EREscrituraDirecta.h"
using namespace std;


void imprimir(AdministradorDeBuffer &admin){
	cout<<"Tope : ";
	cout<<admin.tope->first<<endl;
	for(PNodoSiguiente nodo=admin.tablaArchivoBuffer.begin();nodo!=admin.tablaArchivoBuffer.end();nodo++){
		cout<<nodo->first<<" : ";
		cout<<nodo->second->posicionBuffer<<" : ";
		if(nodo->second->siguiente!=admin.tablaArchivoBuffer.end())
			cout<<nodo->second->siguiente->first;
		cout<<endl;
	};
	cout<<endl;
};
void imprimir2(AdministradorDeBuffer &admin){
	PNodoSiguiente nodo=admin.tope;
	do{
		cout<<nodo->first<<" : ";
		cout<<nodo->second->posicionBuffer<<" : ";
		if(nodo->second->siguiente!=admin.tablaArchivoBuffer.end())
				cout<<nodo->second->siguiente->first;
		cout<<endl;
		nodo=nodo->second->siguiente;
	}while(nodo!=admin.tablaArchivoBuffer.end());
	cout<<endl;
};
int main(int argc,char* argv[]){
	AdministradorDeBuffer admin(5);
	cout<<"inicio"<<endl;
	for(int i=1;i<6;i++){
		admin.insertar(i*10);
	}
	admin.acceder(1);
	imprimir2(admin);
	admin.acceder(0);
	imprimir2(admin);
	admin.acceder(1);
	imprimir2(admin);
	admin.acceder(0);
	imprimir2(admin);
	imprimir2(admin);
	admin.insertar(100);
	imprimir2(admin);
	admin.insertar(0);
	imprimir2(admin);
	admin.acceder(40);
	imprimir2(admin);
	admin.insertar(25);
	imprimir2(admin);
	admin.insertar(35);
	imprimir2(admin);
	return 0;
};
 */
