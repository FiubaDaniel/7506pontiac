/*
 * SerializadorBloques.h
 *
 *  Created on: 03/10/2009
 *      Author: paulo
 */

#ifndef SERIALIZADORBLOQUES_H_
#define SERIALIZADORBLOQUES_H_
#include "Serializador.h"
#include "Bloque.h"

class SerializadorBloques: public Serializador {
public:
	SerializadorBloques();
	void serializar(Componente*componente,std::streambuf salida);
	void deserializar(Componente*componente,std::streambuf entrada);
	virtual ~SerializadorBloques();
};

#endif /* SERIALIZADORBLOQUES_H_ */
