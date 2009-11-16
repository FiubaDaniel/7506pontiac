/*
 * BloqueMemoria.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef BLOQUEMEMORIA_H_
#define BLOQUEMEMORIA_H_
#include "Bloque.h"
#include "Registro.h"
#include "ComponenteMemoria.h"

class BloqueMemoria:public Bloque ,public ComponenteMemoria {
public:
	BloqueMemoria(Bloque* bloque);
	BloqueMemoria(Registro* registro);
	virtual ~BloqueMemoria();
public:/*Heredado de componente*/
	Ttamanio deserializar(std::streambuf&entrada);
	Ttamanio serializar(std::streambuf&salida)const;
	Ttamanio tamanioSerializado()const;
	Componente* clonar()const;
};

#endif /* BLOQUEMEMORIA_H_ */
