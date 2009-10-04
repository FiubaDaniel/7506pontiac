/*
 * SerializadorRegistro.h
 *
 *  Created on: 03/10/2009
 *      Author: paulo
 */

#ifndef SERIALIZADORREGISTRO_H_
#define SERIALIZADORREGISTRO_H_
#include "Serializador.h"
#include "RegistroDuro.h"
class SerializadorRegistro :public Serializador{
public:
	SerializadorRegistro();
	void serializar(Componente*componente,std::streambuf salida);
	void deserializar(Componente*componente,std::streambuf entrada);
	virtual ~SerializadorRegistro();
};

#endif /* SERIALIZADORREGISTRO_H_ */
