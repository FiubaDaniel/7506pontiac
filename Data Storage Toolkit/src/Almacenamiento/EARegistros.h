/*
 * EARegistros.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef EAREGISTROS_H_
#define EAREGISTROS_H_
#include <sstream>
#include "EstrategiaAlmacenamiento.h"

class EARegistros: public EstrategiaAlmacenamiento {

public:
	EARegistros();
	virtual ~EARegistros();
	virtual void posicionarComponente(size_t nroCompuesto);
	virtual void escribir(Almacenamiento* almacen,Compuesto*compuesto);
	virtual void leer(Almacenamiento* almacen,Compuesto*compuesto);
};

#endif /* EAREGISTROS_H_ */
