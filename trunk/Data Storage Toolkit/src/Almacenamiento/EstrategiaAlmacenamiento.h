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
	EstrategiaAlmacenamiento(){};
	virtual ~EstrategiaAlmacenamiento(){};
	virtual void posicionarComponente(size_t nroCompuesto)=0;
	virtual bool escribir(Componente*componente)=0;
	virtual bool leer(Componente*componente)=0;
	virtual size_t insertar(Componente*componente)=0;
	virtual bool modificar(Componente*componente)=0;
	virtual bool eliminar(Componente*componente)=0;
	virtual bool buscar(Componente*componente)=0;
};

#endif /* ESTRATEGIAALMACENAMIENTO_H_ */
