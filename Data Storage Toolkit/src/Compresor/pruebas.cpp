/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"
#define MAX_CANT_CHAR 122
#define MIN_CANT_CHAR 35
#define TAM_BUF1 28
#define TAM_BUF2 10

int pruebacompresion1(){
	unsigned buffer[TAM_BUF1];

	//1234567890123456789012345678901234512345678901234567890123456789012345

	unsigned char str[MAX_CANT_CHAR]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(buffer,TAM_BUF1);
	compresor.setSalida(&cout);
	compresor.comprimirPrimeros(str,MAX_CANT_CHAR);
	compresor.cerrar();
	/*********************************************************/
	/*                   DESCOMPRIMIENDO                     */
	/*********************************************************/
	Compresor descompresor(buffer,TAM_BUF1);
	string descomprimido;
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF1);
	/*Mostrando resultados*/
	cout<<str<<endl;
	cout<<descomprimido<<endl;
	return 0;
}
int pruebacompresion2(){
	unsigned primer_buffer_comprimido[TAM_BUF1];
	unsigned segundo_buffer_comprimido[TAM_BUF2];
						             //1234567890123456789012345678901234512345678901234567890123456789012345
	string fuente_de_caracteres="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(primer_buffer_comprimido,TAM_BUF1);
	/*USANDO EL PRIMER BUFFER*/
	compresor.comprimirPrimeros((unsigned char*)fuente_de_caracteres.c_str(),MIN_CANT_CHAR);
	/*intenta agregar mas de los q pueden entrar*/
	compresor.agregar((unsigned char*)fuente_de_caracteres.c_str(),MAX_CANT_CHAR);
	/*intena agregar una cantidad menor*/
    compresor.agregar((unsigned char*)fuente_de_caracteres.c_str()+MIN_CANT_CHAR,MIN_CANT_CHAR);
    /*termina de usar el buffer*/
	compresor.cerrar();
	/*USANDO EL SEGUNDO BUFFER*/
	/*setea el segundo buffer para escribir*/
	compresor.setContinuacionCompresion(segundo_buffer_comprimido,TAM_BUF2);
	/*escribo los primero bits en el nuevo buffer*/
	compresor.comprimirPrimeros((unsigned char*)fuente_de_caracteres.c_str()+2*MIN_CANT_CHAR,MIN_CANT_CHAR);
	/*agrego el resto de los caracteres*/
	compresor.agregar((unsigned char*)fuente_de_caracteres.c_str()+3*MIN_CANT_CHAR,fuente_de_caracteres.size()-3*MIN_CANT_CHAR);
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
	cout<<caracteres_descomprimidos;
	/*Descomprimo los caracteres del siguiente buffer y muestro*/
	caracteres_descomprimidos.clear();//borrar los anteriores
	descompresor.setExtremos();//reiniciar
	descompresor.descomprimir(segundo_buffer_comprimido,caracteres_descomprimidos,TAM_BUF2);
	cout<<caracteres_descomprimidos<<endl;
	return 0;
}
int pruebaEstragiaCompresionAlmacenamiento(){
	stringbuf buffer;
	buffer.str("hola");
	buffer.str("hola");
	cout<<buffer.str()<<endl;
	AtributoFijo<char*> nombre("N",10);
	Registro registro(1,&nombre);
	EARegistros estrategia(&registro);
	Archivo archivo(&estrategia);
	/*archivo original*/
	archivo.crear("resgitroIniciales.dat");
	for(int i=0;i<MIN_CANT_CHAR;i++){
		*(AtributoFijo<char*>*)registro.get(0)=Terminos::obtenerTermino(i).c_str();
		((Almacenamiento&)archivo).escribir(&registro);
	}
	archivo.imprimir(cout);
	/***************COMPRIMIENDO************************/
	EstrategiaCompresion compresion;
	compresion.compresion(&archivo,"prueba.zip",10);
	archivo.cerrar();
	/*creo un archivo para los descomprimidos*/
	/***************DESCOMPRIMIENDO************************/
	archivo.crear("registroDescomprimido.dat");
	compresion.descompresion(&archivo,"prueba.zip");
	archivo.imprimir(cout);
	archivo.cerrar();
	return 0;
}
