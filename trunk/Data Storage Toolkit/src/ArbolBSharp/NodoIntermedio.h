/*
 * NodoIntermedio.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODOINTERMEDIO_H_
#define NODOINTERMEDIO_H_
#include "Nodo.h"
#include "ElementoNodoIntermedio.h"

class NodoIntermedio : public Nodo{
public:
	NodoIntermedio(unsigned int nivelNodo,unsigned int numNodo,unsigned int cantElem,ComparadorClave* comp);
	NodoIntermedio(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp);
    virtual ~NodoIntermedio();
	void serializate(std::stringbuf* buf);
	std::list<ElementoNodoIntermedio*>getListaElementos();
	void balanceo(NodoIntermedio* nodoHermano,NodoIntermedio* nodoPadre, bool izq);
	int agregarElemento(ElementoNodoIntermedio* elemento);
	int dividirse(NodoIntermedio* nodoHermano,NodoIntermedio* nodoIzq,NodoIntermedio* nodoMedio,NodoIntermedio* nodoDer,NodoIntermedio* nodoPadre,Clave clave,Clave* claveRefNull);
	Referencia getReferenciaIzq();
	Referencia bucarReferenciaAsiguienteNodo(Clave clave);
	void setRefereciaIzq(Referencia ref);
private:
	std::list<ElementoNodoIntermedio*> listaElementos;
	ComparadorClave * comparador;
	/*
	 * si un elemento se agrega y se crea un nodo a la izq del primer elemento entonces
	 * se utilizq la refeecia izq del nodo
	 */
	Referencia referenciaIzq;
};
#endif /* NODOINTERMEDIO_H_ */
