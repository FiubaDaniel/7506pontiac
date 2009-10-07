/*
 * BloqueMemoria.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef BLOQUEMEMORIA_H_
#define BLOQUEMEMORIA_H_
#include "Bloque.h"
class BloqueMemoria:public Bloque {
public:
	BloqueMemoria(Bloque* bloque);
	virtual ~BloqueMemoria();
	Ttamanio nroCompuesto;
	bool estaEscrito();
	bool estaSucio();
	void setSucio(bool valor);
	void setEscrito(bool valor);
public:/*Heredado de componente*/
	Ttamanio deserializar(void*entrada);
	Ttamanio serializar(void*salida);
	Ttamanio tamanioSerializado();
	Componente* clonar();
private:
	bool sucio;
	bool escrito;
};

#endif /* BLOQUEMEMORIA_H_ */
