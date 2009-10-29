/*
 * ElementoNodo.cpp
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#include "ElementoNodoHoja.h"

ElementoNodo::ElementoNodo(Referencia referencia,Clave* cla) {
	referenciaAlArchivo = referencia;
    clave = cla->clonarce();
};
ElementoNodo::ElementoNodo(){};
ElementoNodo::~ElementoNodo() {
       clave->~Clave();
       delete clave;
};
Referencia ElementoNodo::getReferencia(){
	return referenciaAlArchivo;
};
void ElementoNodo::setReferencia(Referencia ref){
	referenciaAlArchivo = ref;
};
Clave* ElementoNodo::getClave(){
	return clave;
};
void ElementoNodo::setClave(Clave* cla){
	clave=cla;
};
ElementoNodo* ElementoNodo::clonarce(){
	ElementoNodo* clon = new ElementoNodo();
	clon->setReferencia(referenciaAlArchivo);
	clon->setClave(clave->clonarce());
	return clon;
};

