//============================================================================
// Name        : BSharpTree.cpp
// Author      : Daniel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include "Clave.h"
#include "ElementoNodo.h"
#include "Nodo.h"
#include "NodoHoja.h"
#include "NodoIntermedio.h"
#include "Clave.h"
#include "ComparadorClave.h"
#include "ComparadorPrueba.h"
#include "BSharpTree.h"
using namespace std;

int main() {
	cout << "BSharpTree" << endl;
	/*creo registros*/
	int numero=1;
	char unNombre[15]="aaa";
	AtributoFijo<int> dni("dni");
	AtributoFijo<char*> nombre("nombre",15);// nombre 15 chars
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg1(2,&nombre,&dni);
	numero=2;
	strncpy(unNombre,"aab",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg2(2,&nombre,&dni);
	numero=3;
	strncpy(unNombre,"abb",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg3(2,&nombre,&dni);
	numero=4;
	strncpy(unNombre,"abc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg4(2,&nombre,&dni);
	numero=5;
	strncpy(unNombre,"acc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg5(2,&nombre,&dni);
	numero=6;
	strncpy(unNombre,"ccc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg6(2,&nombre,&dni);
	/*numero=7;
	strncpy(unNombre,"ccd",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg7(2,&nombre,&dni);
	numero=8;
	strncpy(unNombre,"cde",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg8(2,&nombre,&dni);
	numero=9;
	strncpy(unNombre,"cee",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg9(2,&nombre,&dni);
	numero=10;
	strncpy(unNombre,"edd",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg10(2,&nombre,&dni);
	numero=11;
	strncpy(unNombre,"eaa",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg11(2,&nombre,&dni);
	numero=12;
	strncpy(unNombre,"eab",15);
	nombre.set(unNombre);
	dni.set(&numero);
    Registro reg12(2,&nombre,&dni);
    numero=13;
    strncpy(unNombre,"eac",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg13(2,&nombre,&dni);
    numero=14;
    strncpy(unNombre,"ead",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg14(2,&nombre,&dni);
    numero=15;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg15(2,&nombre,&dni);
    numero=16;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg16(2,&nombre,&dni);
    numero=17;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg17(2,&nombre,&dni);
    numero=18;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg18(2,&nombre,&dni);
    numero=19;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg19(2,&nombre,&dni);
    numero=20;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg20(2,&nombre,&dni);
    numero=21;
    strncpy(unNombre,"eee",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg21(2,&nombre,&dni);*/
    /*Creo comparador de las claves*/
    ComparadorClave* comparador = new ComparadorPrueba();
    /*Creo las claves*/
    Clave* clave1 = new Clave(&reg1,2,"nombre","dni");
    Clave* clave2 = new Clave(&reg2,2,"nombre","dni");
    Clave* clave3 = new Clave(&reg3,2,"nombre","dni");
    Clave* clave4 = new Clave(&reg4,2,"nombre","dni");
    Clave* clave5 = new Clave(&reg5,2,"nombre","dni");
    //Clave* clave6 = new Clave(&reg6,2,"nombre","dni");
    /*Clave* clave7 = new Clave(reg7,2,"nombre","dni");
    Clave* clave8 = new Clave(reg8,2,"nombre","dni");
    Clave* clave9 = new Clave(reg9,2,"nombre","dni");
    Clave* clave10 = new Clave(reg10,2,"nombre","dni");
    Clave* clave11 = new Clave(reg11,2,"nombre","dni");
    Clave* clave12 = new Clave(reg12,2,"nombre","dni");
    Clave* clave13 = new Clave(reg13,2,"nombre","dni");
    Clave* clave14 = new Clave(reg14,2,"nombre","dni");
    Clave* clave15 = new Clave(reg15,2,"nombre","dni");
    Clave* clave16 = new Clave(reg16,2,"nombre","dni");
    Clave* clave17 = new Clave(reg17,2,"nombre","dni");
    Clave* clave18 = new Clave(reg18,2,"nombre","dni");
    Clave* clave19 = new Clave(reg19,2,"nombre","dni");
    Clave* clave20 = new Clave(reg20,2,"nombre","dni");
    Clave* clave21 = new Clave(reg21,2,"nombre","dni");
    Clave* clave22 = new Clave(reg22,2,"nombre","dni");*/
    Referencia ref1 = 1;
    Referencia ref2 = 2;
    Referencia ref3 = 3;
    Referencia ref4 = 4;
    Referencia ref5 = 5;
    /*Referencia ref6 = 6;
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
    Referencia ref19 = 19;
    Referencia ref20 = 20;
    Referencia ref21 = 21;*/
    ElementoNodo* elementoNodoInteremdio1 = new ElementoNodo(ref1,clave1);
    ElementoNodo* elementoNodoInteremdio2 = new ElementoNodo(ref2,clave2);
    ElementoNodo* elementoNodoInteremdio3 = new ElementoNodo(ref3,clave3);
    ElementoNodo* elementoNodoInteremdio4 = new ElementoNodo(ref4,clave4);
    ElementoNodo* elementoNodoInteremdio5 = new ElementoNodo(ref5,clave5);
    //ElementoNodo* elementoNodoInteremdio6 = new ElementoNodo(ref6,clave6);
    /*ElementoNodo* elementoNodoInteremdio7 = new ElementoNodo(ref7,clave7);
    ElementoNodo* elementoNodoInteremdio8 = new ElementoNodo(ref8,clave8);
    ElementoNodo* elementoNodoInteremdio9 = new ElementoNodo(ref9,clave9);
    ElementoNodo* elementoNodoInteremdio10 = new ElementoNodo(ref10,clave10);
    ElementoNodo* elementoNodoInteremdio11 = new ElementoNodo(ref11,clave11);*/

    NodoIntermedio* nodoIntermedio1 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio1->setRefereciaIzq(8);
    nodoIntermedio1->agregarElemento(elementoNodoInteremdio1);
    nodoIntermedio1->agregarElemento(elementoNodoInteremdio2);
    nodoIntermedio1->agregarElemento(elementoNodoInteremdio3);
    nodoIntermedio1->agregarElemento(elementoNodoInteremdio4);
    nodoIntermedio1->agregarElemento(elementoNodoInteremdio5);
    int numeroAux=1;
    char unNombreAux[15]="aaa";
    AtributoFijo<int> dniAux("dni");
    AtributoFijo<char*> nombreAux("nombre",15);// nombre 15 chars
    nombre.set(unNombreAux);
    dni.set(&numeroAux);
    Registro regAux(2,&nombreAux,&dniAux);
    Clave* claveAux = new Clave(&regAux,2,"nombre","dni");
    if(nodoIntermedio1->estaClave(claveAux)){
    	cout << "Bien" << endl;
    }
    std::list<ElementoNodo*>::iterator it = nodoIntermedio1->getListaElementos()->begin();
    while(it!=nodoIntermedio1->getListaElementos()->end()){
    ElementoNodo* elem = *it;
    elem->getClave()->getAtributo("dni") ->imprimir();
    elem->getClave()->getAtributo("nombre") ->imprimir();
    ++it;
    }
    int retorno = nodoIntermedio1->agregarElemento(elementoNodoInteremdio5);
    cout << retorno << endl;
    cout << "aca" << endl;
	return 0;
}
