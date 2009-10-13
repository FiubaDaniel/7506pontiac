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
	virtual void escribir(Almacenamiento* almacen,Componente*componente)=0;
	virtual void leer(Almacenamiento* almacen,Componente*componente)=0;
	virtual size_t insertar(Almacenamiento* almacen,Componente*componente)=0;
	virtual bool modificar(Almacenamiento*almacen,Componente*componente)=0;
	virtual void eliminar(Almacenamiento*almacen,Componente*componente)=0;
	virtual bool buscar(Almacenamiento*almacen,Componente*componente)=0;
};

#endif /* ESTRATEGIAALMACENAMIENTO_H_ */