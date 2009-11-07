/*
 * ComparadorPrueba.cpp
 *
 *  Created on: 30/10/2009
 *      Author: daniel
 */

#include "ComparadorPrueba.h"

ComparadorPrueba::ComparadorPrueba(){}
int ComparadorPrueba::Comparar(const Clave* clave1,const Clave* clave2){
	Clave* aux1 = const_cast<Clave*>(clave1);
	Clave* aux2 = const_cast<Clave*>(clave2);
	int c= aux1->getAtributo("nombre")->comparar(aux2->getAtributo("nombre")  ) ;
	      if(c>0) return 1;
	else if (c<0) return -1;
	else return aux1->getAtributo("dni")->comparar(aux2->getAtributo("dni"));
}
ComparadorPrueba::~ComparadorPrueba(){}
