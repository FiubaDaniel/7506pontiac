/*
 * ElementoNodoHoja.cpp
 *
 *  Created on: 02/10/2009
 *      Author: daniel
 */

#include "ElementoNodoHoja.h"

ElementoNodoHoja::ElementoNodoHoja(Referencia referencia,Clave* cla) {
	referenciaAlArchivo = referencia;
    clave = cla->clonarce();
};
ElementoNodoHoja::ElementoNodoHoja(){};
ElementoNodoHoja::~ElementoNodoHoja() {
       clave->~Clave();
       delete clave;
};
Referencia ElementoNodoHoja::getReferencia(){
	return referenciaAlArchivo;
};
void ElementoNodoHoja::setReferencia(Referencia ref){
	referenciaAlArchivo = ref;
};
Clave* ElementoNodoHoja::getClave(){
	return clave;
};
void ElementoNodoHoja::setClave(Clave* cla){
	clave=cla;
};
ElementoNodoHoja* ElementoNodoHoja::clonarce(){
	ElementoNodoHoja* clon = new ElementoNodoHoja();
	clon->setReferencia(referenciaAlArchivo);
	clon->setClave(clave->clonarce());
	return clon;
};

