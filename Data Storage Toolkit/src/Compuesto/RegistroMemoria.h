/*
 * RegistroMemoria.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef REGISTROMEMORIA_H_
#define REGISTROMEMORIA_H_
#include "Registro.h"
#define SUCIO 0x01
#define ESCRITO 0x02
class RegistroMemoria :public Registro {
public:
	RegistroMemoria(Registro*registro,Ttamanio nroCompuesto,Ttamanio nroRegistro);
	virtual ~RegistroMemoria();
	bool estaEscrito();
	bool estaSucio();
	void setSucio(bool valor);
	void setEscrito(bool valor);
	virtual Ttamanio deserializar(void*entrada);
	virtual Ttamanio serializar(void*salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
private:
	Ttamanio nroCompuesto;
	Ttamanio nroRegitro;
	char flagSucioEscrito;
};

#endif /* REGISTROMEMORIA_H_ */
