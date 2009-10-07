/*
 * BloqueDisco.h
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#ifndef BLOQUEDISCO_H_
#define BLOQUEDISCO_H_
#include "Bloque.h"
class BloqueDisco :public Bloque {
public:
	BloqueDisco(Bloque* bloque);
	virtual ~BloqueDisco();
	bool estaBorrado();
	void setBorrado(bool valor);
public:/*Heredados de Componente*/
	Ttamanio deserializar(std::istream&entrada);
	Ttamanio serializar(std::ostream&salida);
	Ttamanio tamanioSerializado();
	Componente* clonar();
protected:
	bool borrado;
};

#endif /* BLOQUEDISCO_H_ */
