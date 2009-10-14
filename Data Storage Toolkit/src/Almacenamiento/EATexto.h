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
	void abrir(Almacenamiento*almacen);
	void nuevo(Almacenamiento*almacen);
	void posicionarComponente(size_t nroCompuesto);
	bool escribir(Componente*componente);
	bool leer(Componente*componente);
	size_t insertar(Componente*componente);
	bool modificar(Componente*componente);
	bool eliminar(Componente*componente);
	bool buscar(Componente*componente);
};

#endif /* EATEXTO_H_ */
