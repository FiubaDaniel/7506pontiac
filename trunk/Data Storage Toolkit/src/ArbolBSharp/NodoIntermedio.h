/*
 * NodoIntermedio.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODOINTERMEDIO_H_
#define NODOINTERMEDIO_H_
#include "Nodo.h"
#include "ElementoNodo.h"

class NodoIntermedio : public Nodo{
public:
	NodoIntermedio(unsigned int nivelNodo,unsigned int cantElem,ComparadorClave* comp);
	NodoIntermedio(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp);
    virtual ~NodoIntermedio();
	void serializate(std::stringbuf* buf);
	void balanceo(Nodo* nodoIncompletoE,Nodo* nodoPadre, bool izq);
	void balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq);
	void balanceoEspecialIzq(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe);
	void balanceoEspecialDer(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe,Referencia& refIzq);
	int agregarElemento(ElementoNodo* elemento);
	ElementoNodo* dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave clave);
	Referencia getReferenciaIzq();
	int unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre);
	Referencia bucarReferenciaAsiguienteNodo(Clave clave);
	void setRefereciaIzq(Referencia ref);
	void setearClave(Clave claveAeliminar,Clave* claveSetear);
	bool estaClave(Clave clave);
	void limpiar();
private:
	std::list<ElementoNodo*> listaElementos;
	/*
	 * si un elemento se agrega y se crea un nodo a la izq del primer elemento entonces
	 * se utilizq la refeecia izq del nodo
	 */
	Referencia referenciaIzq;
};
#endif /* NODOINTERMEDIO_H_ */