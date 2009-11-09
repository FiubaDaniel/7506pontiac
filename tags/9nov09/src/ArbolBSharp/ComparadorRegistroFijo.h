/*
 * ComparadorRegistroFijo.h
 *
 *  Created on: 06/11/2009
 *      Author: daniel
 */

#ifndef COMPARADORREGISTROFIJO_H_
#define COMPARADORREGISTROFIJO_H_
#include "Clave.h"
#include "ComparadorClave.h"
class ComparadorRegistroFijo: public ComparadorClave{
public:
	ComparadorRegistroFijo();
	int Comparar(const Clave* clave1,const Clave* clave2);
	virtual ~ComparadorRegistroFijo();
};
#endif /* COMPARADORREGISTROFIJO_H_ */
