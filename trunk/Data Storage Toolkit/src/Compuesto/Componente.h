/*
 * Componente.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef COMPONENTE_H_
#define COMPONENTE_H_
#include <streambuf>
typedef unsigned int Ttamanio;
/*
 * Interfaz para la serializacion.
 */
class Componente {
public:
	Componente(){};
	virtual ~Componente(){};
	virtual Ttamanio deserializar(std::streambuf&entrada)=0;
	virtual Ttamanio serializar(std::streambuf&salida)=0;
	virtual Ttamanio tamanioSerializado()=0;
	virtual Componente* clonar()=0;
	//virtual bool esfijo()=0;
};

#endif /* REGISTRO_H_ */
