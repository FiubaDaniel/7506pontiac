
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
	/*creo registros*/
	int numero=1;
	char unNombre[15]="jja";
	AtributoFijo<int> dni("dni");
	AtributoFijo<char*> nombre("nombre",15);// nombre 15 chars
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg1(2,&nombre,&dni);
	numero=2;
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
	numero=6;// Para probar sobre flujo
	strncpy(unNombre,"jjg",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg6(2,&nombre,&dni);
	numero=7;
	strncpy(unNombre,"jje",15);
	nombre.set(unNombre);
	dni.set(&numero);
	Registro reg7(2,&nombre,&dni);
	/*-------nodo padre (nodoIntermedio2)-----------*/
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
	/*----------------nodo hermano (nodoInterne3)--------------*/
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
    /*------------------NodoIntermedio 4-------------------*/
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
    Registro reg18(2,&nombre,&dni);
    /*numero=19;
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
    /*Clave clave19(&reg19,2,"nombre","dni");
    Clave clave20(&reg20,2,"nombre","dni");
    Clave clave21(&reg21,2,"nombre","dni");
    Clave clave22(&reg22,2,"nombre","dni");*/
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
    /*Referencia ref19 = 19;
    Referencia ref20 = 20;
    Referencia ref21 = 21;*/
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
    nodoIntermedio4->setRefereciaIzq(80);
    /*------------------------------------------------------------*/
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio1);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio2);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio3);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio4);
    nodoIntermedio1->agregarElemento(&elementoNodoInteremdio5);
    /*-------------------------------------------------------------*/
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio8);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio9);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio10);
    nodoIntermedio2->agregarElemento(&elementoNodoInteremdio11);
    /*--------------------------------------------------------------*/
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio12);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio13);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio14);
    nodoIntermedio3->agregarElemento(&elementoNodoInteremdio15);
    /*---------------------------------------------------------------*/
    nodoIntermedio4->agregarElemento(&elementoNodoInteremdio16);
    nodoIntermedio4->agregarElemento(&elementoNodoInteremdio17);
    nodoIntermedio4->agregarElemento(&elementoNodoInteremdio18);
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
    delete nodoDeserealizado;
    /*Eliminar*/
    cout << "Eliminar"<<endl;
    nodoIntermedio1->Eliminar(&clave7);
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
    /*balanceo*/
    cout << "Balanceo  "<<endl;
    nodoIntermedio1->balanceo(nodoIntermedio3,nodoIntermedio2,false);
    nodoIntermedio1->balanceo(nodoIntermedio3,nodoIntermedio2,false);
    nodoIntermedio3->balanceo(nodoIntermedio1,nodoIntermedio2,true);
    nodoIntermedio2->Eliminar(&clave11);
    nodoIntermedio3->balanceoEspecial(nodoIntermedio1,nodoIntermedio4,nodoIntermedio2,false);
    nodoIntermedio2->Eliminar(&clave8);
    nodoIntermedio4->balanceoEspecial(nodoIntermedio1,nodoIntermedio3,nodoIntermedio2,true);
    cout << "NodoIntermedio1 "<<endl;
    std::list<ElementoNodo*>::iterator itDe1= nodoIntermedio1->getListaElementos()->begin();
    while(itDe1!=nodoIntermedio1->getListaElementos()->end()){
           ElementoNodo* elem = *itDe1;
           elem->getClave()->getAtributo("dni")->imprimir(cout);
           cout << " " << endl;
           elem->getClave()->getAtributo("nombre") ->imprimir(cout);
           cout << " " << endl;
           cout <<"Referencia: " << elem->getReferencia() << endl;
           ++itDe1;
         }
    cout << "NodoIntermedio2 "<<endl;
    std::list<ElementoNodo*>::iterator itDe2= nodoIntermedio2->getListaElementos()->begin();
    while(itDe2!=nodoIntermedio2->getListaElementos()->end()){
               ElementoNodo* elem = *itDe2;
               elem->getClave()->getAtributo("dni")->imprimir(cout);
               cout << " " << endl;
               elem->getClave()->getAtributo("nombre") ->imprimir(cout);
               cout << " " << endl;
               cout << "Referencia: " << elem->getReferencia() << endl;
               ++itDe2;
             }
    cout << "NodoIntermedio3 "<<endl;
    std::list<ElementoNodo*>::iterator itDe3= nodoIntermedio3->getListaElementos()->begin();
    while(itDe3!=nodoIntermedio3->getListaElementos()->end()){
                   ElementoNodo* elem = *itDe3;
                   elem->getClave()->getAtributo("dni")->imprimir(cout);
                   cout << " " << endl;
                   elem->getClave()->getAtributo("nombre") ->imprimir(cout);
                   cout << " " << endl;
                   cout << "Referencia: " << elem->getReferencia() << endl;
                   ++itDe3;
     }
    cout << "NodoIntermedio4 "<<endl;
    std::list<ElementoNodo*>::iterator itDe4= nodoIntermedio4->getListaElementos()->begin();
    while(itDe4!=nodoIntermedio4->getListaElementos()->end()){
                       ElementoNodo* elem = *itDe4;
                       elem->getClave()->getAtributo("dni")->imprimir(cout);
                       cout << " " << endl;
                       elem->getClave()->getAtributo("nombre") ->imprimir(cout);
                       cout << " " << endl;
                       cout << "Referencia: " << elem->getReferencia() << endl;
                       ++itDe4;
                     }
    nodoIntermedio2->Eliminar(&clave11);
    /*Unirce*/
    cout<<"Unirce"<<endl;
    nodoIntermedio1->Eliminar(&clave3);
    nodoIntermedio1->Eliminar(&clave4);
    nodoIntermedio1->Eliminar(&clave5);
    nodoIntermedio3->Eliminar(&clave14);
    nodoIntermedio3->Eliminar(&clave15);
    cout << nodoIntermedio3->getListaElementos()->size()<<endl;
    nodoIntermedio1->unirse(nodoIntermedio4,nodoIntermedio3,nodoIntermedio2);
    cout << nodoIntermedio1->getReferenciaIzq()<<endl;
    cout << nodoIntermedio2->getReferenciaIzq()<<endl;
    cout << nodoIntermedio3->getReferenciaIzq()<<endl;
    cout << nodoIntermedio4->getReferenciaIzq()<<endl;
    delete nodoIntermedio1;
    delete nodoIntermedio2;
    delete nodoIntermedio3;
    cout<< "Diviedirce" <<endl;
    /*Uso nodo intermedio 1 q sera el izq, y dos nuevos*/
    Nodo* nodoPadre = new NodoIntermedio(1,5,comparador);
    Nodo* nodoHermano = new NodoIntermedio(1,5,comparador);

    /*cargo nodo padre y nodo hermano*/
    nodoPadre->agregarElemento(&elementoNodoInteremdio2);
    nodoHermano->agregarElemento(&elementoNodoInteremdio6);
    nodoHermano->agregarElemento(&elementoNodoInteremdio10);
    nodoHermano->agregarElemento(&elementoNodoInteremdio12);
    nodoHermano->agregarElemento(&elementoNodoInteremdio13);
    nodoHermano->agregarElemento(&elementoNodoInteremdio14);
    nodoHermano->agregarElemento(&elementoNodoInteremdio11);

    NodoIntermedio* nodoPadreE = dynamic_cast<NodoIntermedio*>(nodoPadre);
    NodoIntermedio* nodoHermanoE = dynamic_cast<NodoIntermedio*>(nodoHermano);
    nodoPadreE->setRefereciaIzq(40);
    nodoHermanoE->setRefereciaIzq(50);

    Nodo* nodoIntermedioIzq = new NodoIntermedio(1,5,comparador);
    Nodo* nodoIntermedioMedio = new NodoIntermedio(1,5,comparador);
    Nodo* nodoIntermedioDer = new NodoIntermedio(1,5,comparador);

    ElementoNodo* elemAagregar = nodoIntermedio4->dividirse(nodoHermano,nodoIntermedioIzq,nodoIntermedioMedio,nodoIntermedioDer,nodoPadre,clave2);
    elemAagregar->getClave()->getAtributo("dni")->imprimir(cout);
    cout << " " << endl;
    elemAagregar->getClave()->getAtributo("nombre")->imprimir(cout);
    cout << " " << endl;

    cout << "NodoIntermedio1  "<<endl;
       itDe1= nodoIntermedio1->getListaElementos()->begin();
       while(itDe1!=nodoIntermedio1->getListaElementos()->end()){
                          ElementoNodo* elem = *itDe1;
                          elem->getClave()->getAtributo("dni")->imprimir(cout);
                          cout << " " << endl;
                          elem->getClave()->getAtributo("nombre") ->imprimir(cout);
                          cout << " " << endl;
                          cout << "Referencia: " << elem->getReferencia() << endl;
                          ++itDe1;
       }
       cout << "NodoIntermedioMedio  "<<endl;
       itDe2= nodoIntermedioMedio->getListaElementos()->begin();
       while(itDe2!=nodoIntermedioMedio->getListaElementos()->end()){
                      ElementoNodo* elem = *itDe2;
                      elem->getClave()->getAtributo("dni")->imprimir(cout);
                      cout << " " << endl;
                      elem->getClave()->getAtributo("nombre") ->imprimir(cout);
                      cout << " " << endl;
                      cout << "Referencia: " << elem->getReferencia() << endl;
                      ++itDe2;
                    }
        cout << "NodoIntermedio3 "<<endl;
        itDe3= nodoIntermedioDer->getListaElementos()->begin();
        while(itDe3!=nodoIntermedioDer->getListaElementos()->end()){
                          ElementoNodo* elem = *itDe3;
                          elem->getClave()->getAtributo("dni")->imprimir(cout);
                          cout << " " << endl;
                          elem->getClave()->getAtributo("nombre") ->imprimir(cout);
                          cout << " " << endl;
                          cout << "Referencia: " << elem->getReferencia() << endl;
                          ++itDe3;
            }



    /*destruccion*/
    delete nodoPadre;
    delete nodoHermano;
    delete nodoIntermedioIzq;
    delete nodoIntermedioMedio;
    delete nodoIntermedioDer;
    return 0;
};

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
	cout<<endl;
	return 0;
};*/


