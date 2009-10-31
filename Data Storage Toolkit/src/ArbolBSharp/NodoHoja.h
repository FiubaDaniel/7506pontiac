/*
 * NodoHoja.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODOHOJA_H_
#define NODOHOJA_H_
#include "Nodo.h"
#include "NodoIntermedio.h"
#include "ElementoNodo.h"

using namespace std;

class NodoHoja: public Nodo{

private:
		Referencia referenciaSiguiente;
		bool buscar(const Clave* clave,ElementoNodo* elemento);
		int pos;
public:
	/*
	 * Los nodos hojas deben poder crearce de dos formas, una mediante una tira de bytes
	 * que se obtiene del archivo y otra mediante nuevos datos.
	 */
		NodoHoja(std::stringbuf* buf,int unsigned cantElem,ComparadorClave* comp);
		NodoHoja(unsigned int cantElem,Referencia refSiguiente,ComparadorClave* comp);
		virtual ~NodoHoja();
		void setReferenciaSiguiente(Referencia ref);
		bool setReferenciaDeClaveX(const Clave* clave,Referencia refNueva);
		int agregarElemento(ElementoNodo* elemento);
		void serializate(stringbuf* buffer);
		int unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre);
		ElementoNodo* dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave clave);
		void balanceo(Nodo* nodoIncompletoE,Nodo* nodoPadreE,bool izq);
		void balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq);
		int busquedaSecuencial(Clave clave,ElementoNodo* elemento);
		Referencia getReferenciaSiguiente();
		bool buscarReferenciaDeClaveX(const Clave* clave,Referencia* ref);
		void setPos();
		void avanzarPos();
		ElementoNodo* getPos();
};
#endif /* NODOHOJA_H_ */
