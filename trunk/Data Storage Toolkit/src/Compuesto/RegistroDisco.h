/*
 * RegistroDisco.h
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#ifndef REGISTRODISCO_H_
#define REGISTRODISCO_H_
#include "Registro.h"
class RegistroDisco : public Registro {
public:
	RegistroDisco(Registro*registro);
	virtual ~RegistroDisco();
	virtual bool     borrado();
	virtual void setBorrado(bool flag);
public:/*MetodosHeredados*/
	virtual Ttamanio deserializar(std::istream&entrada);
	virtual Ttamanio serializar(std::ostream&salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
protected:
	bool estaBorrado;
};

#endif /* REGISTRODISCO_H_ */
