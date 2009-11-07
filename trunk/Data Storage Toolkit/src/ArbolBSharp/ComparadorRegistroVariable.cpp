/*
 * ComparadorRegistroVariable.cpp
 *
 *  Created on: 06/11/2009
 *      Author: daniel
 */

#include "ComparadorRegistroVariable.h"

ComparadorRegistroVariable::ComparadorRegistroVariable() {};
int ComparadorRegistroVariable::Comparar(const Clave* clave1,const Clave* clave2){
	Clave* aux1 = const_cast<Clave*>(clave1);
	Clave* aux2 = const_cast<Clave*>(clave2);
	int c= aux1->getAtributo("miStringID")->comparar(aux2->getAtributo("miStringID"));
	      if(c>0) return 1;
	else if (c<0) return -1;
	return 0;
};
ComparadorRegistroVariable::~ComparadorRegistroVariable() {};
