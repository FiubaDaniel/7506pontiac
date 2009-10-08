/*
 * EstrategiaAlmacenamiento.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef ESTRATEGIAALMACENAMIENTO_H_
#define ESTRATEGIAALMACENAMIENTO_H_
#include "Almacenamiento.h"
class Almacenamiento;
class EstrategiaAlmacenamiento {
public:
	EstrategiaAlmacenamiento();
	virtual ~EstrategiaAlmacenamiento();
	virtual void posicionarCompuesto(size_t nroCompuesto)=0;
	virtual void escribir(Almacenamiento* almacen,Compuesto*compuesto)=0;
	virtual void leer(Almacenamiento* almacen,Compuesto*compuesto)=0;
};

#endif /* ESTRATEGIAALMACENAMIENTO_H_ */
