/*
 * EABloques.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef EABLOQUES_H_
#define EABLOQUES_H_
#include "EstrategiaAlmacenamiento.h"
#include "../Compuesto/Bloque.h"
#include <sstream>
class EABloques : public EstrategiaAlmacenamiento{
	size_t corriente;
	char* buffertransferencia;
	Ttamanio tamanio;
public:
	EABloques(Ttamanio tamanioBloque);
	virtual ~EABloques();
	void posicionarCompuesto(size_t nroCompuesto);
	void escribir(Almacenamiento* almacen,Compuesto*compuesto);
	void leer(Almacenamiento* almacen,Compuesto*compuesto);
};

#endif /* EABLOQUES_H_ */
