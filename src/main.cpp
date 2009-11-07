
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
	/*cout << "BSharpTree" << endl;
	ComparadorClave* comparador = new ComparadorPrueba();
	int numero=1;
	char unNombre[15]="a";
	AtributoFijo<int> dni("dni");
	AtributoFijo<char*> nombre("nombre",15);// nombre 15 chars
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg1(2,&nombre,&dni);
	Clave clave1(&reg1,2,"nombre","dni");
	BSharpTree* arbol = new BSharpTree(&clave1);
	string nombreArchivo = "Daniel4";
	unsigned int tamanioBloque = 150;
    int tamanioClave = clave1.tamanioSerializado();
	arbol->crear(nombreArchivo,tamanioBloque,tamanioClave,comparador);
	int cantidadDeElemXNodo = arbol->calcularCantidadElementosPorNodo(clave1.tamanioSerializado());
	cout << "entran: "<<cantidadDeElemXNodo<<endl;
    delete arbol;
    BSharpTree* arbol2 = new BSharpTree(&clave1);
    arbol2->abrir(nombreArchivo,comparador);
    Referencia ref1 = 1;*/
    /*-------------claves y reg------------------*/
    /*numero=2;
    strncpy(unNombre,"b",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg2(2,&nombre,&dni);
    numero=3;
    strncpy(unNombre,"c",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg3(2,&nombre,&dni);
    numero=4;
    strncpy(unNombre,"d",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg4(2,&nombre,&dni);
    numero=5;
    strncpy(unNombre,"e",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg5(2,&nombre,&dni);
    numero=6;
    strncpy(unNombre,"f",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg6(2,&nombre,&dni);
    numero=7;
    strncpy(unNombre,"g",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg7(2,&nombre,&dni);
    numero=8;
    strncpy(unNombre,"h",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg8(2,&nombre,&dni);
    numero=9;
    strncpy(unNombre,"i",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg9(2,&nombre,&dni);
    numero=10;
    strncpy(unNombre,"j",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg10(2,&nombre,&dni);
    numero=11;
    strncpy(unNombre,"k",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg11(2,&nombre,&dni);
    numero=12;
    strncpy(unNombre,"l",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg12(2,&nombre,&dni);
    numero=13;
    strncpy(unNombre,"m",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg13(2,&nombre,&dni);
    numero=14;
    strncpy(unNombre,"n",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg14(2,&nombre,&dni);
    numero=15;
    strncpy(unNombre,"o",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg15(2,&nombre,&dni);
    numero=16;
    strncpy(unNombre,"p",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg16(2,&nombre,&dni);
    numero=17;
    strncpy(unNombre,"q",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg17(2,&nombre,&dni);
    numero=18;
    strncpy(unNombre,"r",15);
    nombre.set(unNombre);
    dni.set(&numero);
    Registro reg18(2,&nombre,&dni);
    numero=19;
        strncpy(unNombre,"s",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg19(2,&nombre,&dni);
        numero=20;
        strncpy(unNombre,"t",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg20(2,&nombre,&dni);
        numero=21;
        strncpy(unNombre,"p",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg21(2,&nombre,&dni);
        numero=22;
        strncpy(unNombre,"q",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg22(2,&nombre,&dni);
        numero=23;
        strncpy(unNombre,"r",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg23(2,&nombre,&dni);
        numero=24;
        strncpy(unNombre,"s",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg24(2,&nombre,&dni);
        numero=25;
        strncpy(unNombre,"t",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg25(2,&nombre,&dni);
        numero=26;
        strncpy(unNombre,"y",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg26(2,&nombre,&dni);
        numero=27;
        strncpy(unNombre,"z",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg27(2,&nombre,&dni);
        numero=28;
        strncpy(unNombre,"w",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg28(2,&nombre,&dni);
        numero=29;
        strncpy(unNombre,"x",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg29(2,&nombre,&dni);
        numero=30;
        strncpy(unNombre,"aa",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg30(2,&nombre,&dni);
        numero=31;
        strncpy(unNombre,"ab",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg31(2,&nombre,&dni);
        numero=32;
        strncpy(unNombre,"op",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg32(2,&nombre,&dni);
        numero=33;
        strncpy(unNombre,"pr",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg33(2,&nombre,&dni);
        numero=34;
        strncpy(unNombre,"qe",15);
        nombre.set(unNombre);
        dni.set(&numero);
        Registro reg34(2,&nombre,&dni);
        numero=35;
        strncpy(unNombre,"rq",15);
        nombre.set(unNombre);
        dni.set(&numero);
    Registro reg35(2,&nombre,&dni);
    Clave clave2(&reg2,2,"nombre","dni");
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
    Clave clave19(&reg19,2,"nombre","dni");
    Clave clave18(&reg18,2,"nombre","dni");
        Clave clave20(&reg20,2,"nombre","dni");
        Clave clave21(&reg21,2,"nombre","dni");
        Clave clave22(&reg22,2,"nombre","dni");
        Clave clave23(&reg23,2,"nombre","dni");
        Clave clave24(&reg24,2,"nombre","dni");
        Clave clave25(&reg25,2,"nombre","dni");
        Clave clave26(&reg26,2,"nombre","dni");
        Clave clave27(&reg27,2,"nombre","dni");
        Clave clave28(&reg28,2,"nombre","dni");
        Clave clave29(&reg29,2,"nombre","dni");
        Clave clave30(&reg30,2,"nombre","dni");
        Clave clave31(&reg31,2,"nombre","dni");
        Clave clave32(&reg32,2,"nombre","dni");
        Clave clave33(&reg33,2,"nombre","dni");
        Clave clave34(&reg34,2,"nombre","dni");
        Clave clave35(&reg35,2,"nombre","dni");
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
    Referencia ref19 = 19;
       Referencia ref20 = 20;
       Referencia ref21 = 21;
       Referencia ref22= 22;
       Referencia ref23= 23;
       Referencia ref24= 24;
       Referencia ref25= 25;
       Referencia ref26= 26;
       Referencia ref27= 27;
       Referencia ref28= 28;
       Referencia ref29= 29;
       Referencia ref30= 30;
       Referencia ref31= 31;
       Referencia ref32= 32;
       Referencia ref33= 33;
       Referencia ref34= 34;
       Referencia ref35= 35;*/
   /* arbol2->insertar(ref6,&clave6);
    arbol2->insertar(ref1,&clave1);
    arbol2->insertar(ref3,&clave3);
    arbol2->insertar(ref5,&clave5);
    arbol2->insertar(ref4,&clave4);
    arbol2->insertar(ref2,&clave2);
    arbol2->insertar(ref7,&clave7);
    arbol2->insertar(ref9,&clave9);
    arbol2->insertar(ref15,&clave15);
    arbol2->insertar(ref10,&clave10);
    arbol2->insertar(ref8,&clave8);//
    arbol2->insertar(ref12,&clave12);
    arbol2->insertar(ref13,&clave13);
    arbol2->insertar(ref14,&clave14);
    arbol2->insertar(ref16,&clave16);
    arbol2->insertar(ref18,&clave18);
    arbol2->insertar(ref17,&clave17);
    arbol2->insertar(ref19,&clave19);
    arbol2->insertar(ref11,&clave11);
    arbol2->insertar(ref20,&clave20);
    arbol2->insertar(ref21,&clave21);
    Referencia referencia;
    arbol2->Buscar(&clave17,&referencia);
    cout<<referencia<<endl;
    Referencia referencia2;
    arbol2->eliminar(&clave17);
    arbol2->Buscar(&clave17,&referencia2);
        cout<<referencia<<endl;
        arbol2->insertar(ref22,&clave22);
        arbol2->insertar(ref23,&clave23);
        arbol2->insertar(ref24,&clave24);
        arbol2->insertar(ref25,&clave25);
        arbol2->insertar(ref26,&clave26);
        arbol2->insertar(ref27,&clave27);
        arbol2->insertar(ref28,&clave28);
        arbol2->insertar(ref29,&clave29);
        arbol2->insertar(ref30,&clave30);
        arbol2->insertar(ref31,&clave31);
        arbol2->insertar(ref32,&clave32);
        arbol2->insertar(ref33,&clave33);
        arbol2->insertar(ref34,&clave34);
        arbol2->insertar(ref35,&clave35);



    delete arbol2;
    cout<<"llego"<<endl;*/


   /*
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


