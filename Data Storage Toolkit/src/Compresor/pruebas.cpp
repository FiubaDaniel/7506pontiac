/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"
#define MAX_CANT_CHAR 122
#define MIN_CANT_CHAR 35
#define TAM_BUF1 30
#define TAM_BUF2 10
int pruebacompresion1(){
	unsigned buffer[TAM_BUF1];

	//1234567890123456789012345678901234512345678901234567890123456789012345

	unsigned char str[122]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	Compresor compresor(buffer,TAM_BUF1);
	compresor.comprimirPrimeros(str,122);// CANT_Char es el numero de caracteres q queres comprimir
	compresor.cerrar();
	Compresor descompresor(buffer,TAM_BUF1);
	string descomprimido;
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF1);
	cout<<str<<endl;
	cout<<descomprimido<<endl;
	return 0;
}
int pruebacompresion2(){
	unsigned primer_buffer_comprimido[TAM_BUF1];
	unsigned segundo_buffer_comprimido[TAM_BUF2];
						             //1234567890123456789012345678901234512345678901234567890123456789012345
	unsigned char fuente_de_caracteres[MAX_CANT_CHAR]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(primer_buffer_comprimido,TAM_BUF1);
	/*USANDO EL PRIMER BUFFER*/
	compresor.comprimirPrimeros(fuente_de_caracteres,MIN_CANT_CHAR);
	/*intenta agregar mas de los q pueden entrar*/
	compresor.agregar(fuente_de_caracteres+MIN_CANT_CHAR,MAX_CANT_CHAR);
	/*intena agregar una cantidad menor*/
    compresor.agregar(fuente_de_caracteres+MIN_CANT_CHAR,MIN_CANT_CHAR);
    /*termina de usar el buffer*/
	compresor.cerrar();
	/*USANDO EL SEGUNDO BUFFER*/
	/*setea el segundo buffer para escribir*/
	compresor.setContinuacionCompresion(segundo_buffer_comprimido,TAM_BUF2);
	/*escribo los primero bits en el nuevo buffer*/
	compresor.comprimirPrimeros(fuente_de_caracteres+2*MIN_CANT_CHAR,MIN_CANT_CHAR);
	/*ternimo de usar el segundo buffer*/
	compresor.cerrar();
	/*********************************************************/
	/*                   DESCOMPRIMIENDO                     */
	/*********************************************************/
	Compresor descompresor(primer_buffer_comprimido,TAM_BUF1);
	string caracteres_descomprimidos;
	/*Descomprimo los caracteres del primer buffer y muestro*/
	descompresor.descomprimir(primer_buffer_comprimido,caracteres_descomprimidos,TAM_BUF1);
	cout<<fuente_de_caracteres<<endl;
	cout<<caracteres_descomprimidos<<endl;
	/*Descomprimo los caracteres del siguiente buffer y muestro*/
	caracteres_descomprimidos.clear();//borrar los anterior
	descompresor.setExtremos();//reiniciar
	descompresor.descomprimir(segundo_buffer_comprimido,caracteres_descomprimidos,TAM_BUF2);
	cout<<caracteres_descomprimidos<<endl;
	return 0;
}

