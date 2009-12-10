/*
 * Clave.h
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#ifndef CLAVE_H_
#define CLAVE_H_

#include "../Compuesto/Registro.h"
#include "../Compuesto/AtributoFijo.h"
#include "../Compuesto/AtributoVariable.h"
typedef int Referencia;
class Clave {

public:
	Clave(const Clave & otra);
	Clave(Registro* registro,unsigned int cantidadAtributosEntrantes,...);
	Clave();
	void set(Registro*reg);
	unsigned int getCantidadAtributos();
	void setCantidadAtributos(unsigned int cant);
	Atributo* getAtributo(Ttamanio posicion);
	Atributo* getAtributo(std::string nombre);
	void deserializar(std::streambuf&buf);
    void serializar(std::streambuf&buf);
    int tamanioSerializado();
    Clave* clonarce();
	virtual ~Clave();
private:
	unsigned int cantidadAtributos;
	std::vector<Atributo*> atributosDeClave;
};

#endif /* CLAVE_H_ */
