/*
 * EATexto.h
 *
 *  Created on: 13/10/2009
 *      Author: paulo
 */

#ifndef EATEXTO_H_
#define EATEXTO_H_

#include "EstrategiaAlmacenamiento.h"

class EATexto: public EstrategiaAlmacenamiento {
public:
	EATexto();
	virtual ~EATexto();
	void posicionarComponente(size_t nroCompuesto)=0;
	void escribir(Almacenamiento* almacen,Componente*componente)=0;
	void leer(Almacenamiento* almacen,Componente*componente)=0;
	size_t insertar(Almacenamiento* almacen,Componente*componente)=0;
	bool modificar(Almacenamiento*almacen,Componente*componente)=0;
	void eliminar(Almacenamiento*almacen,Componente*componente)=0;
	bool buscar(Almacenamiento*almacen,Componente*componente)=0;
};

#endif /* EATEXTO_H_ */
