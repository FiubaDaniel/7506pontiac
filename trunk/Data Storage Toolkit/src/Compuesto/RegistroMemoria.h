/*
 * RegistroMemoria.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef REGISTROMEMORIA_H_
#define REGISTROMEMORIA_H_
#include "Registro.h"
#include "ComponenteMemoria.h"

class RegistroMemoria : public Registro , public ComponenteMemoria{
public:
	RegistroMemoria(Registro*registro);
	virtual ~RegistroMemoria();
public:/*MetodoHeredados*/
	virtual Ttamanio deserializar(std::streambuf&entrada);
	virtual Ttamanio serializar(std::streambuf&salida)const;
	virtual Ttamanio tamanioSerializado()const;
	virtual Componente* clonar()const;
private:
	Ttamanio nroCompuesto;
	Ttamanio nroRegitro;
};

#endif /* REGISTROMEMORIA_H_ */
