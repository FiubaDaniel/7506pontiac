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
#include <cstdio>


int pruebacompresion1(){
	unsigned buffer[TAM_BUF1];
	//1234567890123456789012345678901234512345678901234567890123456789012345
	unsigned char str[MAX_CANT_CHAR+1]="Base class for standard exceptions. All objects thrown by components of the standard library are derived from this class.";
	/*unsigned char str[MAX_CANT_CHAR+1]="Base class for standard exceptions.
	 All objects thrown by components of the standard library are derived from
	 this class. Ella esta sentadita en el limbo diciendome  donde no hay que ir.
	 Hoy la luna me invita a seguir. Determinadas cosas con la que nos sentimos
	 identificados y nos distinguen de otros que no.";*/
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
}
int pruebaEstragiaCompresionAlmacenamiento(char nombre_archivo[]){
	AtributoFijo<char*> nombre("N",10);
	Registro registro(1,&nombre);
	EARegistros estrategia(&registro);
	Archivo archivo(&estrategia);
	/*archivo original*/
	archivo.crear(nombre_archivo);
	for(int i=0;i<200;i++){
		*(AtributoFijo<char*>*)registro.get(0)=Terminos::obtenerTermino(i).c_str();
		((Almacenamiento&)archivo).escribir(&registro);
	}
	cout<<"/***************COMPRIMIENDO************************/"<<endl;
	archivo.imprimir(cout);
	archivo.cerrar();
	EstrategiaCompresion compresion;
	archivo.abrir(nombre_archivo);
	compresion.compresion(&archivo,10);
	archivo.cerrar();
	/*creo un archivo para los descomprimidos*/
	cout<<endl<<"/***************DESCOMPRIMIENDO************************/"<<endl;
	remove(nombre_archivo);
	archivo.crear(nombre_archivo);
	compresion.descompresion(&archivo);
	archivo.imprimir(cout);
	archivo.cerrar();
	return 0;
}
int pruebaEstragiaCompresionAlmacenamiento1(char nombre_archivo[]){
	AtributoVariable<string> nombre("N");
	AtributoVariable<int> numeros("#");
	AtributoFijo<int> num("n");
	Registro registro(3,&nombre,&numeros,&num);
	Bloque bloque(&registro);
	Clave clave(&registro,1,"N");
	ComparadorRegistroVariable comparador;
	EstrategiaCompresion compresion;
	EABloques estrategia(&bloque,125,0.8f);
	estrategia.setClave(&clave);
	estrategia.setComparador(&comparador);
	Archivo archivo(&estrategia);
	/*archivo original*/
	archivo.crear(nombre_archivo);
	if(not compresion.descompresion(&archivo)){
		for(int i=0;i<5;i++){
			*(AtributoVariable<string>*)registro.get(0)=Terminos::obtenerTermino(i).c_str();
			((AtributoVariable<int>*)registro.get(1))->getVector().clear();
			for(int j=0;j< i%4+1;j++){
				((AtributoVariable<int>*)registro.get(1))->getVector().push_back(j);
			};
			*((AtributoFijo<int>*)registro.get(2))=i;
			if(not ((Almacenamiento&)archivo).insertar(&registro))
				cout<<"Problema"<<endl;
		}
		//archivo.imprimir(cout);
		compresion.compresion(&archivo,512);
		cout<<"/***************COMPRIMIENDO************************/"<<endl;
	}else{
		archivo.imprimir(cout);
		cout<<"/***************DESCOMPRIMIDO**********************/"<<endl;
	}
	archivo.cerrar();
	remove(nombre_archivo);
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
	string nombreArbol = "Arbol";
	string nombreArbol2 = "Arbol2";
	string nombreComprimido = "ArbolComprimido";
	unsigned int tamanioBloque = 128;
	arbol->crear(nombreArbol,tamanioBloque,&clave,comparador);
	for(int i=0;i<70;i++){
		strncpy(unNombre,Terminos::obtenerTermino(i).data(),15);
		nombre.set(unNombre);
		Registro reg1(1,&nombre);
		Clave clave1(&reg1,1,"miStringID");
		//std::cout<<clave1.getAtributo("miStringID")<<std::endl;
		arbol->insertar(ref,&clave1);
	}
	for(int i=0;i<30;i++){
		strncpy(unNombre,Terminos::obtenerTermino(i).data(),15);
		nombre.set(unNombre);
		Registro reg1(1,&nombre);
		Clave clave1(&reg1,1,"miStringID");
		//std::cout<<clave1.getAtributo("miStringID")<<std::endl;
		arbol->eliminar(&clave1);
	}
	arbol->imprimir();
	//arbol->cerrar();
	string nombreArchivo = arbol->getNombreArchivo();
	/*delete comparador;
	delete arbol;*/
	/************************COMPRIMIENDO*****************************/
	EstrategiaCompresion compresion;
	compresion.compresionArbol(arbol,nombreComprimido,128);
	arbol->cerrar();
	delete arbol;
	//compresion.compresionIndice(nombreArchivo,nombreComprimido,128);
	/************************DESCOMPRIMIENDO**************************/
	BSharpTree* arbol2 = new BSharpTree(&clave);
	ComparadorClave* comparador2 = new ComparadorRegistroVariable();
	//compresion.descompresionInsdice(nombreComprimido,nombreArchivo);
	//arbol2->abrir(nombreArbol2,nombreArbol,comparador2);
	//arbol2->abrir(nombreArbol2,nombreArbol,comparador2);
	compresion.descompresionArbol(arbol2,nombreComprimido);
	arbol2->imprimir();
	delete arbol2;
	delete comparador2;
	return 0;
}
int pruebaCompresion5(){
	//AAAAAABBBBBBPPPPPPDSAZXCVBNN�
	//12345678901234567890123456789012345678901234567890
	//AAAAAABBBBBBPPPPPPDSAZXCVBNMqweeeeeeeeetttttttttty
	//123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
#define CANT_AGREGAR 2
#define CANT_UNSIGNED 30
#define NUMERO_ITERACIONES 70
	unsigned char str[]="AAAAAABBBBBBPPPPPPDSAZXCBNMqweeeeeeeeettttttttttuiopasdfghjklzxcvbnm12345678901234567890123456789012345678901234567890";
	unsigned array[CANT_UNSIGNED];
	Compresor compresor(array,CANT_UNSIGNED);
	Compresor descompresor(array,CANT_UNSIGNED);
	string descomprimido;
	descompresor.setCaracteres(100);
	for(int i=0;i<NUMERO_ITERACIONES;i++){
		compresor.reiniciarBuffer();
		compresor.comprimirPrimeros(str,25);
		compresor.agregar(str+25,25);
		compresor.agregar(str+2*25,25);
		compresor.agregar(str+3*25,25);
		compresor.cerrar();
		descomprimido.clear();
		descompresor.reiniciarBuffer();
		descompresor.descomprimir(array,descomprimido,CANT_UNSIGNED);
		cout<<str<<endl;
		cout<<descomprimido<<endl;
	}
	return 0;
}
/*int PruebaComprimirNodo(){
	ComparadorClave* comparador = new ComparadorRegistroVariable();
	NodoIntermedio* nodo = new NodoIntermedio(2,4,comparador);
	ElementoNodo* elemento = new ElementoNodo();
}*/
