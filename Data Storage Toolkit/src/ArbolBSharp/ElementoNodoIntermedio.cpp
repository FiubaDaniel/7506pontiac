/*
 * ElementoNodoIntermedio.cpp
 *
 *  Created on: 08/10/2009
 *      Author: daniel
 */

#include "ElementoNodoIntermedio.h"

ElementoNodoIntermedio::ElementoNodoIntermedio(Referencia refDer,Clave* cla) {
	 referenciaDer = refDer;
	 clave = cla->clonarce();
};
ElementoNodoIntermedio::ElementoNodoIntermedio(){};
Referencia ElementoNodoIntermedio::getReferenciaDer(){
	return referenciaDer;
};
Clave* ElementoNodoIntermedio::getClave(){
	return clave;
};
void ElementoNodoIntermedio::setClave(Clave* cla){
	clave = cla;
}
void ElementoNodoIntermedio::setReferenciaDer(Referencia ref){
	referenciaDer = ref;
};

ElementoNodoIntermedio* ElementoNodoIntermedio::clonarce(){
	ElementoNodoIntermedio* clon = new ElementoNodoIntermedio();
	clon->setReferenciaDer(referenciaDer);
	clon->setClave(clave->clonarce());
	return clon;
};
ElementoNodoIntermedio::~ElementoNodoIntermedio() {
	  clave->~Clave();
	  delete clave;
};
