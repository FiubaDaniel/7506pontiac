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
	RegistroMemoria(Registro*registro,Ttamanio nroCompuesto,Ttamanio nroRegistro);
	virtual ~RegistroMemoria();
	bool estaEscrito();
	bool estaSucio();
	void setSucio(bool valor);
	void setEscrito(bool valor);
public:/*MetodoHeredados*/
	virtual Ttamanio deserializar(std::streambuf&entrada);
	virtual Ttamanio serializar(std::streambuf&salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
private:
	Ttamanio nroCompuesto;
	Ttamanio nroRegitro;
	enum flags {sucio=0x01,escrito=0x02} estado;
};

#endif /* REGISTROMEMORIA_H_ */
