/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"
#define MAX_CANT_CHAR 122
#define MIN_CANT_CHAR 35
#define TAM_BUF1 40
#define TAM_BUF2 10
int pruebacompresion(){
	unsigned buffer[TAM_BUF1];
				 //1234567890123456789012345678901234512345678901234567890123456789012345
	char str[122]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	Compresor compresor(buffer,TAM_BUF1);
	compresor.comprimirPrimeros(str,MIN_CANT_CHAR);
	if(not compresor.agregar(str+MIN_CANT_CHAR,MIN_CANT_CHAR))
		cout<<"no se agrego"<<endl;
	compresor.cerrar();
	unsigned buffer2[TAM_BUF2];
	compresor.setContinuacionCompresion(buffer2,TAM_BUF2);
	compresor.comprimirPrimeros(str+2*MIN_CANT_CHAR,MIN_CANT_CHAR);
	compresor.cerrar();
	Compresor descompresor(buffer,TAM_BUF1);
	string descomprimido;
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF1);
	cout<<str<<endl;
	cout<<descomprimido<<endl;
	descomprimido.clear();
	descompresor.setExtremos();
	descompresor.descomprimir(buffer2,descomprimido,TAM_BUF2);
	cout<<descomprimido<<endl;
	return 0;
}

