/*
 * RegistroMemoria.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef REGISTROMEMORIA_H_
#define REGISTROMEMORIA_H_
#include "Registro.h"
class RegistroMemoria :public Registro {
public:
	RegistroMemoria(Registro*registro,Ttamanio nroCompuesto,Ttamanio nroRegistro,bool escrito);
	virtual ~RegistroMemoria();
	Ttamanio nroCompuesto;
	Ttamanio nroRegitro;
	bool escrito;
	bool sucio;
	virtual Ttamanio deserializar(void*entrada);
	virtual Ttamanio serializar(void*salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
	virtual Atributo* get(Ttamanio posicion);
	virtual Atributo* get(std::string nombre);
	virtual Ttamanio cantidadAtributos();
	virtual Atributo* getId();
	virtual bool esfijo();
};

#endif /* REGISTROMEMORIA_H_ */
