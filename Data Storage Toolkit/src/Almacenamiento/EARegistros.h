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
#include "../Compuesto/Registro.h"

class EARegistros: public EstrategiaAlmacenamiento {
	size_t nroRegistro;
	Ttamanio tamanioRegistro;
	char* registroSerializado;
public:
	EARegistros();
	virtual ~EARegistros();
	virtual void posicionarComponente(size_t nroCompuesto);
	virtual void escribir(Almacenamiento* almacen,Componente*componente);
	virtual void leer(Almacenamiento* almacen,Componente*componente);
	virtual size_t insertar(Almacenamiento* almacen,Componente*componente);
	virtual bool modificar(Almacenamiento*almacen,Componente*componente);
	virtual void eliminar(Almacenamiento*almacen,Componente*componente);
	virtual bool buscar(Almacenamiento*almacen,Componente*componente);
};

#endif /* EAREGISTROS_H_ */
