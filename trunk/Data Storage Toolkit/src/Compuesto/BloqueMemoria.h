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
	Ttamanio deserializar(std::streambuf&entrada);
	Ttamanio serializar(std::streambuf&salida);
	Ttamanio tamanioSerializado();
	Componente* clonar();
private:
	enum flags {sucio=0x01,escrito=0x02} estado;
};

#endif /* BLOQUEMEMORIA_H_ */
