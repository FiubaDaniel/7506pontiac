/*
 * ComparadorPrueba.h
 *
 *  Created on: 30/10/2009
 *      Author: daniel
 */

#ifndef COMPARADORPRUEBA_H_
#define COMPARADORPRUEBA_H_
#include "Clave.h"
#include "ComparadorClave.h"

class ComparadorPrueba: public ComparadorClave{
public:
	ComparadorPrueba();
	int Comparar(const Clave* clave1,const Clave* clave2);
	virtual ~ComparadorPrueba();
};

#endif /* COMPARADORPRUEBA_H_ */
