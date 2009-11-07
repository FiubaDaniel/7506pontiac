/*
 * ComparadorRegistroFijo.cpp
 *
 *  Created on: 06/11/2009
 *      Author: daniel
 */

#include "ComparadorRegistroFijo.h"

ComparadorRegistroFijo::ComparadorRegistroFijo() {};
int ComparadorRegistroFijo::Comparar(const Clave* clave1,const Clave* clave2){
	Clave* aux1 = const_cast<Clave*>(clave1);
	Clave* aux2 = const_cast<Clave*>(clave2);
	int c= aux1->getAtributo("miCharID")->comparar(aux2->getAtributo("miCharID")  ) ;
	      if(c>0) return 1;
	else if (c<0) return -1;
	else return aux1->getAtributo("miIntID")->comparar(aux2->getAtributo("miIntID"));
};
ComparadorRegistroFijo::~ComparadorRegistroFijo() {};
