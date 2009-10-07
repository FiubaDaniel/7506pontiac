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
	Ttamanio nroRegitro;
	bool estaEscrito();
	bool estaSucio();
	bool setSucio();
	bool setEscrito();
	Ttamanio deserializar(void*entrada);
	Ttamanio serializar(void*salida);
	Ttamanio tamanioSerializado();
	Componente* clonar();
	Componente* get(Ttamanio nroComponente);
	Ttamanio cantidadComponentes();
	bool insertar(Componente*componente,Ttamanio posicion);
	bool eliminar(Ttamanio posicion);
	bool agregar(Componente*componente);
	Componente* reemplazar(Componente*componente,Ttamanio posicion);
private:
	bool sucio;
	bool escrito;
};

#endif /* BLOQUEMEMORIA_H_ */
