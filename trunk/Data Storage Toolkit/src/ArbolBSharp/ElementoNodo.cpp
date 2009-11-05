/*
 * ElementoNodo.cpp
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#include "ElementoNodo.h"

ElementoNodo::ElementoNodo(Referencia ref,Clave* cla) {
	referencia= ref;
    clave = cla->clonarce();
};
ElementoNodo::ElementoNodo(){};
ElementoNodo::~ElementoNodo() {
       delete clave;
};
Referencia ElementoNodo::getReferencia(){
	return referencia;
};
void ElementoNodo::setReferencia(Referencia ref){
	referencia = ref;
};
Clave* ElementoNodo::getClave(){
	return clave;
};
void ElementoNodo::setClave(Clave* nueva){
     clave = nueva->clonarce();
};
ElementoNodo* ElementoNodo::clonarce(){
	ElementoNodo* clon = new ElementoNodo();
	clon->setReferencia(referencia);
	clon->setClave(clave);
	return clon;
};

