/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"
#define MAX_CANT_CHAR 122
#define MIN_CANT_CHAR 35
#define TAM_BUF_U_SIG 30
int pruebacompresion(){
	unsigned buffer[TAM_BUF_U_SIG];
				 //1234567890123456789012345678901234512345678901234567890123456789012345
	char str[122]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	Compresor compresor(buffer,TAM_BUF_U_SIG);
	compresor.comprimirPrimeros(str,MIN_CANT_CHAR);
	if(not compresor.agregar(str+MIN_CANT_CHAR,MIN_CANT_CHAR))
		cout<<"no se agrego"<<endl;
	/*compresor.agregar(str+2*MIN_CANT_CHAR,MIN_CANT_CHAR);*/
	compresor.cerrar();
	Compresor descompresor(buffer,TAM_BUF_U_SIG);
	string descomprimido;
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF_U_SIG);
	cout<<str<<endl;
	cout<<descomprimido<<endl;
	return 0;
}

