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

	NodoIntermedio(std::stringbuf* buf,unsigned int cantElem,ComparadorClave* comp,Clave* claveEstructural);

	virtual ~NodoIntermedio();

	void serializate(std::stringbuf* buf);

	/*
	 * En Insercion: balancea un nodo completo con un nodo que al menos tiene un lugar libre, seteando los elementos que correspondan del padre
	 * En eliminacion: balancea un nodo que tiene un elemento menos del minimo tomando un elemento de un nodo hermano que posee al menos un elemento
	 * mas del minimo. seta los elementos que corresponda del padre.
	 */
	void balanceo(Nodo* nodoIncompletoE,Nodo* nodoPadre, bool izq);

	/*
	 * El balanceo especial se utiliza en la eliminacion cuando el nodo en subflujo es
	 * algun nodo extremo haciendo un balanceo consecutivo con nodo hermanos.
	 */
	void balanceoEspecial(Nodo* nodoPegado,Nodo* nodoAlejado,Nodo* padre,bool Izq);
	void balanceoEspecialHermanosDer(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe);
	void balanceoEspecialHermanosIzq(NodoIntermedio* nodoQcede,ElementoNodo* elemPadre,std::list<ElementoNodo*> *listaQrecibe,Referencia& refIzq);

	/*
	 * Agrega un elemento en forma ordenada segun la clave establecida en el nodo seteando los valores correspondientes del mismo.
	 */
	int agregarElemento(ElementoNodo* elemento);

	/*
	 *  Forma tres nodo a partir de dos nodos y setea al padre agregando el nuevo elemento separador producido por el nuevo nodo.
	 */
	ElementoNodo* dividirse(Nodo* nodoHermanoE,Nodo* nodoIzqE,Nodo* nodoMedioE,Nodo* nodoDerE,Nodo* nodoPadreE,Clave* clave);

	Referencia getReferenciaIzq();

	/*
	 * Forma dos nodos a partir de tres nodo de los cuales uno est en subflujo.
	 */
	int unirse(Nodo* nodoHermanoIzq,Nodo* nodoHermanoDer,Nodo* Padre);

	Referencia bucarReferenciaAsiguienteNodo(Clave* clave);

	void setRefereciaIzq(Referencia ref);

	void setearClave(const Clave* claveAeliminar,Clave* claveSetear);

	bool estaClave(const Clave* clave);

	void limpiar();

private:

	Referencia referenciaIzq;
};
#endif /* NODOINTERMEDIO_H_ */
