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
#include "ElementoNodoHoja.h"

using namespace std;

class NodoHoja: public Nodo{

private:
		Referencia referenciaIzq,referenciaDer;
		std::list <ElementoNodo*> listaElementos;
		int calcularDosTercios();
		ComparadorClave * comparador;
		ElementoNodo* buscar(Clave* clave);
public:
	/*
	 * Los nodos hojas deben poder crearce de dos formas, una mediante una tira de bytes
	 * que se obtiene del archivo y otra mediante nuevos datos.
	 */
		NodoHoja(std::stringbuf buf,int unsigned cantElem,ComparadorClave* comp);
		NodoHoja(unsigned int numNodo,unsigned int cantElem,Referencia refIzq,Referencia refDer,ComparadorClave* comp);
		virtual ~NodoHoja();
		void setReferenciaIzq(Referencia ref);
		void setReferenciaDer(Referencia ref);
		int setReferenciaDeClaveX(Clave* clave,Referencia refNueva);
		int agregarElemento(ElementoNodo* elemento);
		void serializate(stringbuf* buffer);
		ElementoNodoIntermedio* dividirse(NodoHoja* nodoHermano,NodoHoja* nodoIzq,NodoHoja* nodoMedio,NodoHoja* nodoDer,NodoIntermedio* nodoPadre,Clave clave);
		void balanceo(NodoHoja* nodoIncompleto,NodoIntermedio* nodoPadre,bool izq);
		std::list<ElementoNodo*> getListaElementos();
		int busquedaSecuencial(Clave* clave,ElementoNodo* elemento);
		Referencia getReferenciaIzq();
		Referencia getReferenciaDer();
		Referencia buscarReferenciaDeClaveX(Clave* clave);
};
#endif /* NODOHOJA_H_ */
