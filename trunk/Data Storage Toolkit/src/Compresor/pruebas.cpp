/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"

int pruebacompresion(){
	unsigned buffer[TAM_BUF_U_SIG];
	char str[122]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	Compresor compresor(buffer,TAM_BUF_U_SIG);
	compresor.comprimirPrimeros(str,MIN_CANT_CHAR);
	compresor.cerrar();
	Compresor descompresor(buffer,TAM_BUF_U_SIG);
	string descomprimido;
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF_U_SIG);
	cout<<str<<endl;
	cout<<descomprimido<<endl;
	return 0;
}

