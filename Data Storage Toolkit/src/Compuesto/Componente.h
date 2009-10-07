/*
 * Componente.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef COMPONENTE_H_
#define COMPONENTE_H_
#include <iostream>
typedef unsigned int Ttamanio;
/*
 * Interfaz para la serializacion.
 */
class Componente {
public:
	Componente(){};
	virtual ~Componente(){};
	virtual Ttamanio deserializar(std::istream&entrada)=0;
	virtual Ttamanio serializar(std::ostream&salida)=0;
	virtual Ttamanio tamanioSerializado()=0;
	virtual Componente* clonar()=0;
	//virtual bool esfijo()=0;
};

#endif /* REGISTRO_H_ */
