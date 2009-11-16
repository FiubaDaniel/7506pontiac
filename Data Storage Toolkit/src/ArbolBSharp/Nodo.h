/*
 * Nodo.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODO_H_
#define NODO_H_
#include "Clave.h"
#include "ComparadorClave.h"
#include <list>
#include <sstream>
#include <iostream>
#include <string>
#include "ElementoNodo.h"
typedef unsigned int Referencia;
class Nodo {
public:
	     Nodo(unsigned int cantElem,unsigned int nNivel,ComparadorClave* comparador);

	     unsigned int getEspacioLibre();

	     unsigned int getCatidadMaximaDeElementos();

	     unsigned int getNumeroNivel();

	     void setNumeroNivel(unsigned int num);

	     void setEspacioLibre(int esp);

	     std::list<ElementoNodo*> *getListaElementos();

	     virtual void balanceo(Nodo* nodoIncompletoE,Nodo* nodoPadreE, bool izq)=0;

	     virtual void balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq)=0;

	     virtual ElementoNodo* dividirse(Nodo* nodoHermano,Nodo* nodoIzq,Nodo* nodoMedio,Nodo* nodoDer,Nodo* nodoPadreE,Clave* clave)=0;

	     virtual void serializate(std::stringbuf* buffer)=0;

	     virtual int agregarElemento(ElementoNodo* elemento)=0;
	     /*
	      * Elimina un registro del nodo.
	      */
	     int Eliminar(const Clave* clave);

	     virtual int unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre)=0;

	     virtual ~Nodo();

protected:
	     unsigned int cantidadMaximaDeElementos,cantidadDeElementosLibre,numeroNivel;
	     std::list <ElementoNodo*> listaElementos;
	     ComparadorClave * comparador;
};
#endif /* NODO_H_ */
