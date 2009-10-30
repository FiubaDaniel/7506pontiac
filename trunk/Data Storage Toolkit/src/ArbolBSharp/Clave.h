/*
 * Clave.h
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#ifndef CLAVE_H_
#define CLAVE_H_

#include "Registro.h"
#include "Atributo.h"
typedef unsigned int Referencia;
class Clave {

public:
	Clave(Registro* registro,unsigned int cantidadAtributosEntrantes,...);
	Clave();
	unsigned int getCantidadAtributos();
	void setCantidadAtributos(unsigned int cant);
	Atributo* getAtributo(Ttamanio posicion);
	Atributo* getAtributo(std::string nombre);
	void deserializar(std::streambuf&buf);
    void serializar(std::streambuf&buf);
    Ttamanio tamanioSerializado();
    Clave* clonarce();
	virtual ~Clave();
private:
	unsigned int cantidadAtributos;
	std::vector<Atributo*> atributosDeClave;
};

#endif /* CLAVE_H_ */
