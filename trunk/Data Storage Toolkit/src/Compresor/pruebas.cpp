/*
 * pruebasCompresion.cpp
 *
 *  Created on: 30/11/2009
 *      Author: paulo
 */
#include "pruebas.h"
#define MAX_CANT_CHAR 121
#define MIN_CANT_CHAR 50

#define TAM_BUF1 50
#define TAM_BUF2 50
#include <cstring>
#include <cstdlib>
#include <stdio.h>


int pruebacompresion1(){
	unsigned buffer[TAM_BUF1];

	//1234567890123456789012345678901234512345678901234567890123456789012345

	unsigned char str[MAX_CANT_CHAR+1]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(buffer,TAM_BUF1);
	//compresor.setSalida(&cout);
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
	string fuente_de_caracteres="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this classa";
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(primer_buffer_comprimido,TAM_BUF1);
	/*USANDO EL PRIMER BUFFER*/
	compresor.comprimirPrimeros((unsigned char*)fuente_de_caracteres.c_str(),MIN_CANT_CHAR);
	/*intenta agregar mas de los q pueden entrar*/
	compresor.agregar((unsigned char*)fuente_de_caracteres.c_str(),MIN_CANT_CHAR*3);
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
	cout<<fuente_de_caracteres<<"!"<<endl;
	cout<<caracteres_descomprimidos;
	/*Descomprimo los caracteres del siguiente buffer y muestro*/
	caracteres_descomprimidos.clear();//borrar los anteriores
	descompresor.setExtremos();//reiniciar
	descompresor.descomprimir(segundo_buffer_comprimido,caracteres_descomprimidos,TAM_BUF2);
	cout<<caracteres_descomprimidos<<"!"<<endl;
	return 0;
}
int pruebaCompresion3(){

	unsigned buffer1[TAM_BUF2];
	unsigned buffer2[TAM_BUF2];
	unsigned *buffer=buffer1;
	Compresor descompresor(buffer,TAM_BUF2);
	string descomprimido;
	/*********************************************************/
	/*                      COMPRIMIENDO                     */
	/*********************************************************/
	Compresor compresor(buffer,TAM_BUF2);
	compresor.comprimirPrimeros((unsigned char*)Terminos::obtenerTermino(0).c_str(),Terminos::obtenerTermino(0).size());
	for(int i=1;i<296;i++){
		if(!compresor.agregar((unsigned char*)Terminos::obtenerTermino(i).c_str(),Terminos::obtenerTermino(i).size())){
			compresor.cerrar();
			/*********************************************************/
			/*                   DESCOMPRIMIENDO                     */
			/*********************************************************/
			descomprimido.clear();
			descompresor.descomprimir(buffer,descomprimido,TAM_BUF2);
			cout<<descomprimido<<endl;
			buffer=buffer2;
			compresor.setContinuacionCompresion(buffer,TAM_BUF2);
			/*********************************************************/
			compresor.comprimirPrimeros((unsigned char*)Terminos::obtenerTermino(i).c_str(),Terminos::obtenerTermino(i).size());
		}
	}
	compresor.cerrar();
	descomprimido.clear();
	descompresor.descomprimir(buffer,descomprimido,TAM_BUF2);
	cout<<descomprimido<<endl;

	return 0;
};
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

int pruebaEstrategiaCompresionArbol(){
	/*************************Creacion Arbol***************************/
	Referencia ref=0;
	AtributoFijo<char*> nombre("miStringID",15);
	char unNombre[15]="abaco";
	nombre.set(unNombre);
	Registro reg(1,&nombre);
	Clave clave(&reg,1,"miStringID");
	BSharpTree* arbol = new BSharpTree(&clave);
	ComparadorClave* comparador = new ComparadorRegistroVariable();
	string nombreArchivo = "Arbol";
	unsigned int tamanioBloque = 128;
	arbol->crear(nombreArchivo,tamanioBloque,&clave,comparador);
	for(int i=0;i<4;i++){
		strncpy(unNombre,Terminos::obtenerTermino(i).data(),15);
		nombre.set(unNombre);
		Registro reg1(1,&nombre);
		Clave clave1(&reg1,1,"miStringID");
		//std::cout<<clave1.getAtributo("miStringID")<<std::endl;
		arbol->insertar(ref,&clave1);
	}
	arbol->imprimir();
	/************************COMPRIMIENDO*****************************/
	EstrategiaCompresion compresion;
	compresion.compresionArbol(arbol,"pruebaArbolzip",100);
	delete arbol;
	delete comparador;
	/************************DESCOMPRIMIENDO**************************/
	BSharpTree* arbol2 = new BSharpTree(&clave);
	ComparadorClave* comparador2 = new ComparadorRegistroVariable();
	string nombreArchivo2 = "Arbol2";
	arbol2->crear(nombreArchivo2,200,&clave,comparador2);
	compresion.descompresionArbol(arbol2,"pruebaArbolzip");
	arbol2->imprimir();
	delete arbol2;
	delete comparador2;
	return 0;
}
/*int PruebaComprimirNodo(){
	ComparadorClave* comparador = new ComparadorRegistroVariable();
	NodoIntermedio* nodo = new NodoIntermedio(2,4,comparador);
	ElementoNodo* elemento = new ElementoNodo();
}*/
