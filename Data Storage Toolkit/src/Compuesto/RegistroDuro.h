/*
 * RegistroDuro.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */
#ifndef REGISTROFIJO_H_
#define REGISTROFIJO_H_
#include <cstdarg>
#include <vector>
#include "Componente.h"
#include "Atributo.h"

class RegistroDuro : public Componente{
public:
	RegistroDuro(unsigned int cantidadAtributos, ... );
	virtual ~RegistroDuro();
	//TODO ver si sirve o no
	Componente* clonar();
	Atributo* get(Ttamanio posicion);
	Atributo* get(std::string nombre);
	void setbytes(std::streambuf& fuente);// tarea del serializador
	void getbytes(std::streambuf& pbuffer);//tarea del serializador
	Ttamanio cantidadAtributos();
	Ttamanio cantidadBytes();
private:
	std::vector<Atributo*> atributos;
	//Ttamanio tamanioBuffer;
};

#endif /* REGISTROFIJO_H_ */
