/*
 * Serializador.h
 *
 *  Created on: 03/10/2009
 *      Author: paulo
 */

#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_
#include "Componente.h"
#include "RegistroDuro.h"
#include <streambuf>
class Serializador {
public:
	Serializador(){};
	virtual void serializar(Componente*componente,std::streambuf salida)=0;
	virtual void deserializar(Componente*componente,std::streambuf entrada)=0;
	virtual ~Serializador(){};
};

#endif /* SERIALIZADOR_H_ */
