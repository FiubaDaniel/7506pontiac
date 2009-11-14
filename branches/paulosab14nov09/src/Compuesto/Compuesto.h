/*
 * Compuesto.h
 *
 *  Created on: 05/10/2009
 *      Author: paulo
 */

#ifndef COMPUESTO_H_
#define COMPUESTO_H_
#include "Componente.h"
#include <vector>
class Compuesto{
public:
	virtual Componente* get(Ttamanio nroComponente)=0;
	virtual Ttamanio cantidadComponentes()=0;
	virtual bool insertar(Componente*componente,Ttamanio posicion)=0;
	virtual bool eliminar(Ttamanio posicion)=0;
	virtual bool agregar(Componente*componente)=0;
	virtual Componente* reemplazar(Componente*componente,Ttamanio posicion)=0;
};
#endif /* COMPUESTO_H_ */
