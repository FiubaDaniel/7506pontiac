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
#include "Atributo.h"
#include "AtributoFijo.h"
#include "AtributoVariable.h"
#include "Registro.h"
#include "Componente.h"
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
	numero=6;// Para probar sobre flujo
	strncpy(unNombre,"ccc",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg6(2,&nombre,&dni);
	numero=7;
	strncpy(unNombre,"aac",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg7(2,&nombre,&dni);
	/*numero=8;
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
    Clave clave1(&reg1,2,"nombre","dni");
    Clave clave2(&reg2,2,"nombre","dni");
    Clave clave3(&reg3,2,"nombre","dni");
    Clave clave4(&reg4,2,"nombre","dni");
    Clave clave5(&reg5,2,"nombre","dni");
    Clave clave6(&reg6,2,"nombre","dni");
    int comparado = comparador->Comparar(&clave1,&clave1);
    cout << comparado << endl;
    Clave clave7(&reg7,2,"nombre","dni");
    /*Clave clave8(reg8,2,"nombre","dni");
    Clave clave9(reg9,2,"nombre","dni");
    Clave clave10(reg10,2,"nombre","dni");
    Clave clave11(reg11,2,"nombre","dni");
    Clave clave12(reg12,2,"nombre","dni");
    Clave clave13(reg13,2,"nombre","dni");
    Clave clave14(reg14,2,"nombre","dni");
    Clave clave15(reg15,2,"nombre","dni");
    Clave clave16(reg16,2,"nombre","dni");
    Clave clave17(reg17,2,"nombre","dni");
    Clave clave18(reg18,2,"nombre","dni");
    Clave clave19(reg19,2,"nombre","dni");
    Clave clave20(reg20,2,"nombre","dni");
    Clave clave21(reg21,2,"nombre","dni");
    Clave clave22(reg22,2,"nombre","dni");*/
    Referencia ref1 = 1;
    Referencia ref2 = 2;
    Referencia ref3 = 3;
    Referencia ref4 = 4;
    Referencia ref5 = 5;
    Referencia ref6 = 6;
    Referencia ref7 = 7;
    /*Referencia ref8 = 8;
    Referencia ref9 = 9;
    Referencia ref10 = 10;
    Referencia ref11 = 11;
    Referencia ref12 = 12;
    Referencia ref13 = 13;
    Referencia ref14 = 14;
    Referencia ref15 = 15;*/
    /*Referencia ref16 = 16;
    Referencia ref17 = 17;
    Referencia ref18 = 18;
    Referencia ref19 = 19;
    Referencia ref20 = 20;
    Referencia ref21 = 21;*/
    ElementoNodo elementoNodoInteremdio1(ref1,&clave1);
    ElementoNodo elementoNodoInteremdio2(ref2,&clave2);
    ElementoNodo elementoNodoInteremdio3(ref3,&clave3);
    ElementoNodo elementoNodoInteremdio4(ref4,&clave4);
    ElementoNodo elementoNodoInteremdio5(ref5,&clave5);
    ElementoNodo elementoNodoInteremdio6(ref6,&clave6);
    ElementoNodo elementoNodoInteremdio7(ref7,&clave7);
    /*ElementoNodo elementoNodoInteremdio8(ref8,clave8);
    ElementoNodo elementoNodoInteremdio9(ref9,clave9);
    ElementoNodo elementoNodoInteremdio10(ref10,clave10);
    ElementoNodo elementoNodoInteremdio11(ref11,clave11);
    ElementoNodo elementoNodoInteremdio12(ref11,clave11);
    ElementoNodo elementoNodoInteremdio13(ref11,clave11);
    ElementoNodo elementoNodoInteremdio14(ref11,clave11);
    ElementoNodo elementoNodoInteremdio15(ref11,clave11);*/
    NodoIntermedio* nodoIntermedio1 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio1->setRefereciaIzq(8);
    /*NodoIntermedio* nodoIntermedio2 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio1->setRefereciaIzq(8);
    NodoIntermedio* nodoIntermedio3 = new NodoIntermedio(1,5,comparador);
    nodoIntermedio1->setRefereciaIzq(8);*/
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio1);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio2);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio3);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio4);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio5);
    /* el esta de nodo intermedio*/
    if(nodoIntermedio1->estaClave(&clave1)){
    	cout << "Bien Esta" << endl;
    }
    numero = 8;
    nombre.set(unNombre);
    dni.set(&numero);
    Registro regAux(2,&nombre,&dni);
    Clave claveAux(&regAux,2,"nombre","dni");
    if(!nodoIntermedio1->estaClave(&claveAux)){
        	cout << "Bien no esta" << endl;
      }
    /*El agregar de nodoIntermedio*/
    std::list<ElementoNodo*>::iterator it = nodoIntermedio1->getListaElementos()->begin();
    while(it!=nodoIntermedio1->getListaElementos()->end()){
    ElementoNodo* elem = *it;
			elem->getClave()->getAtributo("dni")->imprimir(cout);
			cout << " " << endl;
			elem->getClave()->getAtributo("nombre") ->imprimir(cout);
            cout << " " << endl;
    ++it;
    }
    int retorno = nodoIntermedio1->agregarElemento(&elementoNodoInteremdio5);
    cout << retorno << endl;
    retorno = nodoIntermedio1->agregarElemento(&elementoNodoInteremdio6);
    cout << retorno << endl;
    cout << nodoIntermedio1->getListaElementos()->size() <<endl;//se espera 6
    /*meto uno por el medio*/
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio7);
    std::list<ElementoNodo*>::iterator itInsert = nodoIntermedio1->getListaElementos()->begin();
    cout << "aac tiene q quedar tercero"<< endl;
    while(itInsert!=nodoIntermedio1->getListaElementos()->end()){
        ElementoNodo* elem = *itInsert;
    	elem->getClave()->getAtributo("dni")->imprimir(cout);
    	cout << " " << endl;
    	elem->getClave()->getAtributo("nombre") ->imprimir(cout);
        cout << " " << endl;
        ++itInsert;
        }
    /*seriañizar*/
    cout<< "deserilizar y serializar"<<endl;
    std:: stringbuf buf(ios_base :: in | ios_base :: out | ios_base :: binary);
    nodoIntermedio1->serializate(&buf);
    NodoIntermedio* nodoDeserealizado = new NodoIntermedio(&buf,5,comparador,&clave2);
    std::list<ElementoNodo*>::iterator itdes = nodoDeserealizado->getListaElementos()->begin();
    while(itdes!=nodoDeserealizado->getListaElementos()->end()){
        ElementoNodo* elem = *itdes;
    	elem->getClave()->getAtributo("dni")->imprimir(cout);
    	cout << " " << endl;
    	elem->getClave()->getAtributo("nombre") ->imprimir(cout);
        cout << " " << endl;
        ++itdes;
    }
    /*Eliminar*/
    cout << "Eliminar"<<endl;
    nodoIntermedio1->Eliminar(&clave1);
    if(!nodoIntermedio1->estaClave(&claveAux)){
            	cout << "Bien no esta2" << endl;
          }
    std::list<ElementoNodo*>::iterator itEliminar = nodoIntermedio1->getListaElementos()->begin();
    while(itEliminar!=nodoIntermedio1->getListaElementos()->end()){
       ElementoNodo* elem = *itEliminar;
       elem->getClave()->getAtributo("dni")->imprimir(cout);
       cout << " " << endl;
       elem->getClave()->getAtributo("nombre") ->imprimir(cout);
       cout << " " << endl;
       ++itEliminar;
     }
    cout << "Espacion Libre: "<< nodoIntermedio1->getEspacioLibre() << endl;
    return 0;
};
