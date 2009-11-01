
#include <iostream>
#include <cstring>
#include "ArbolBSharp/Clave.h"
#include "ArbolBSharp/ElementoNodo.h"
#include "Compuesto/Atributo.h"
#include "Compuesto/AtributoFijo.h"
#include "Compuesto/AtributoVariable.h"
#include "Compuesto/Registro.h"
#include "Compuesto/Componente.h"
#include "ArbolBSharp/Nodo.h"
#include "ArbolBSharp/NodoHoja.h"
#include "ArbolBSharp/NodoIntermedio.h"
#include "ArbolBSharp/Clave.h"
#include "ArbolBSharp/ComparadorClave.h"
#include "ArbolBSharp/ComparadorPrueba.h"
#include "ArbolBSharp/BSharpTree.h"
using namespace std;

int main() {
	cout << "BSharpTree" << endl;
	ComparadorClave* comparador = new ComparadorPrueba();
	int numero=1;
	char unNombre[15]="jja";
	AtributoFijo<int> dni("dni");
	AtributoFijo<char*> nombre("nombre",15);// nombre 15 chars
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg1(2,&nombre,&dni);
	Clave clave1(&reg1,2,"nombre","dni");
	BSharpTree* arbol = new BSharpTree(&clave1);
	string nombreArchivo = "Daniel";
	unsigned int tamanioBloque = 150;
    int tamanioClave = clave1.tamanioSerializado();
	arbol->crear(nombreArchivo,tamanioBloque,tamanioClave,comparador);
	int cantidadDeElemXNodo = arbol->calcularCantidadElementosPorNodo(clave1.tamanioSerializado());
	cout << "entran: "<<cantidadDeElemXNodo<<endl;
    delete arbol;
    BSharpTree* arbol2 = new BSharpTree(&clave1);
    arbol2->abrir(nombreArchivo,comparador);
    Referencia ref = 1;
    arbol2->insertar(ref,&clave1);
    cout<<"llego"<<endl;
	/*creo registros*/
	/*numero=2;
	strncpy(unNombre,"jjb",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg2(2,&nombre,&dni);
	numero=3;
	strncpy(unNombre,"jjc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg3(2,&nombre,&dni);
	numero=4;
	strncpy(unNombre,"jjd",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg4(2,&nombre,&dni);
	numero=5;
	strncpy(unNombre,"jjf",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg5(2,&nombre,&dni);
	numero=6;
	strncpy(unNombre,"jjg",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg6(2,&nombre,&dni);
	numero=7;
	strncpy(unNombre,"jje",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg7(2,&nombre,&dni);
	numero=8;
	strncpy(unNombre,"aaa",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg8(2,&nombre,&dni);
	numero=9;
	strncpy(unNombre,"ccc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg9(2,&nombre,&dni);
	numero=10;
	strncpy(unNombre,"kkk",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg10(2,&nombre,&dni);
	numero=11;
	strncpy(unNombre,"www",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg11(2,&nombre,&dni);
    numero=12;
	strncpy(unNombre,"lll",15);
	nombre.set(unNombre);
	dni.set(&numero);
    Registro reg12(2,&nombre,&dni);
    numero=13;
    strncpy(unNombre,"llm",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg13(2,&nombre,&dni);
    numero=14;
    strncpy(unNombre,"lln",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg14(2,&nombre,&dni);
    numero=15;
    strncpy(unNombre,"llo",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg15(2,&nombre,&dni);
    numero=16;
    strncpy(unNombre,"aab",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg16(2,&nombre,&dni);
    numero=17;
    strncpy(unNombre,"aac",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg17(2,&nombre,&dni);
    numero=18;
    strncpy(unNombre,"aad",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg18(2,&nombre,&dni);*/

    /*Clave clave2(&reg2,2,"nombre","dni");
    Clave clave3(&reg3,2,"nombre","dni");
    Clave clave4(&reg4,2,"nombre","dni");
    Clave clave5(&reg5,2,"nombre","dni");
    Clave clave6(&reg6,2,"nombre","dni");
    Clave clave7(&reg7,2,"nombre","dni");
    Clave clave8(&reg8,2,"nombre","dni");
    Clave clave9(&reg9,2,"nombre","dni");
    Clave clave10(&reg10,2,"nombre","dni");
    Clave clave11(&reg11,2,"nombre","dni");
    Clave clave12(&reg12,2,"nombre","dni");
    Clave clave13(&reg13,2,"nombre","dni");
    Clave clave14(&reg14,2,"nombre","dni");
    Clave clave15(&reg15,2,"nombre","dni");
    Clave clave16(&reg16,2,"nombre","dni");
    Clave clave17(&reg17,2,"nombre","dni");
    Clave clave18(&reg18,2,"nombre","dni");

    Referencia ref1 = 1;
    Referencia ref2 = 2;
    Referencia ref3 = 3;
    Referencia ref4 = 4;
    Referencia ref5 = 5;
    Referencia ref6 = 6;
    Referencia ref7 = 7;
    Referencia ref8 = 8;
    Referencia ref9 = 9;
    Referencia ref10 = 10;
    Referencia ref11 = 11;
    Referencia ref12 = 12;
    Referencia ref13 = 13;
    Referencia ref14 = 14;
    Referencia ref15 = 15;
    Referencia ref16 = 16;
    Referencia ref17 = 17;
    Referencia ref18 = 18;

    ElementoNodo elementoNodoInteremdio1(ref1,&clave1);
    ElementoNodo elementoNodoInteremdio2(ref2,&clave2);
    ElementoNodo elementoNodoInteremdio3(ref3,&clave3);
    ElementoNodo elementoNodoInteremdio4(ref4,&clave4);
    ElementoNodo elementoNodoInteremdio5(ref5,&clave5);
    ElementoNodo elementoNodoInteremdio6(ref6,&clave6);
    ElementoNodo elementoNodoInteremdio7(ref7,&clave7);
    ElementoNodo elementoNodoInteremdio8(ref8,&clave8);
    ElementoNodo elementoNodoInteremdio9(ref9,&clave9);
    ElementoNodo elementoNodoInteremdio10(ref10,&clave10);
    ElementoNodo elementoNodoInteremdio11(ref11,&clave11);
    ElementoNodo elementoNodoInteremdio12(ref12,&clave12);
    ElementoNodo elementoNodoInteremdio13(ref13,&clave13);
    ElementoNodo elementoNodoInteremdio14(ref14,&clave14);
    ElementoNodo elementoNodoInteremdio15(ref15,&clave15);
    ElementoNodo elementoNodoInteremdio16(ref16,&clave16);
    ElementoNodo elementoNodoInteremdio17(ref17,&clave17);
    ElementoNodo elementoNodoInteremdio18(ref18,&clave18);
    NodoIntermedio* nodoIntermedio1 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio1->setRefereciaIzq(30);
    NodoIntermedio* nodoIntermedio2 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio2->setRefereciaIzq(40);
    NodoIntermedio* nodoIntermedio3 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio3->setRefereciaIzq(50);
    NodoIntermedio* nodoIntermedio4 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio4->setRefereciaIzq(80);*/
    /*------------------------------------------------------------*/
    /*nodoIntermedio1->agregarElemento(&elementoNodoInteremdio1);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio2);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio3);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio4);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio5);*/
    /*-------------------------------------------------------------*/
    /*nodoIntermedio2->agregarElemento(&elementoNodoInteremdio8);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio9);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio10);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio11);*/
    /*--------------------------------------------------------------*/
    /*nodoIntermedio3->agregarElemento(&elementoNodoInteremdio12);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio13);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio14);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio15);*/
    /*---------------------------------------------------------------*/
    /*nodoIntermedio4->agregarElemento(&elementoNodoInteremdio16);
    nodoIntermedio4->agregarElemento(&elementoNodoInteremdio17);
    nodoIntermedio4->agregarElemento(&elementoNodoInteremdio18);*/


    /*---------------------------------------------------------*/


/*
 * main.cpp
 *
 *  Created on: 31/10/2009
 *      Author: paulo
*/

/*
#include <iostream>
#include <cstdlib>
#include "RecursoAlmacenamiento/EREscrituraDirecta.h"
using namespace std;
void imprimir(AdministradorDeBuffer &admin){
	for(unsigned int i=0;i<admin.size();i++){
		NodoArchivoBuffer *nodo=admin.at(i);
		cout<<nodo->posicionArchivo<<" : ";
		cout<<nodo->posicionBuffer<<" : ";
		if(nodo->siguiente!=NULL){
			cout<<nodo->siguiente->posicionArchivo;
		}else cout<<"NULL";
			cout<<endl;

	};
};
void imprimir2(AdministradorDeBuffer &admin){
	NodoArchivoBuffer *nodo=admin.tope;
	while(nodo!=NULL){
		cout<<nodo->posicionArchivo<<" : ";
		cout<<nodo->posicionBuffer<<" : ";
		if(nodo->siguiente!=NULL){
			cout<<nodo->siguiente->posicionArchivo;
		}else cout<<"NULL";
			cout<<endl;
			nodo=nodo->siguiente;
	};
	cout<<endl;
};
int main(int argc,char* argv[]){
	AdministradorDeBuffer admin(5);
	cout<<"inicio"<<endl;
	for(int i=0;i<5;i++){
		admin.insertar(i);
	}
	admin.acceder(1);
	imprimir2(admin);
	admin.acceder(0);
	imprimir2(admin);
	admin.acceder(1);
	imprimir2(admin);
	admin.acceder(0);
	imprimir2(admin);
	admin.insertar(100);
	imprimir2(admin);
	cout<<endl;*/
	return 0;
};


