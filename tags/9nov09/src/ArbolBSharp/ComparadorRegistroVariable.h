/*
 * ComparadorRegistroVariable.h
 *
 *  Created on: 06/11/2009
 *      Author: daniel
 */

#ifndef COMPARADORREGISTROVARIABLE_H_
#define COMPARADORREGISTROVARIABLE_H_
#include "Clave.h"
#include "ComparadorClave.h"

class ComparadorRegistroVariable:public ComparadorClave {
public:
	ComparadorRegistroVariable();
	int Comparar(const Clave* clave1,const Clave* clave2);
	virtual ~ComparadorRegistroVariable();
};

#endif /* COMPARADORREGISTROVARIABLE_H_ */
