/*
 * RegistroMaleable.h
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#ifndef REGISTROMALEABLE_H_
#define REGISTROMALEABLE_H_
#include <cstdarg>
#include <vector>
#include "Componente.h"
#include "Atributo.h"



class RegistroMaleable: public Componente {
public:
	RegistroMaleable(bool desalocar,unsigned int cantidadAtributos, ... );
	virtual ~RegistroMaleable();
	Atributo* get(Ttamanio posicion);
	Atributo* get(std::string nombre);
	void setbytes(std::streambuf& fuente);//lee bytes desde el buffer
	void getbytes(std::streambuf& pbuffer);//escribe bytes hacia el buffer
	Ttamanio cantidadAtributos();
	//Ttamanio cantidadBytes();
private:
	bool deboDesalocar;
	std::vector<Atributo*> atributos;
	Ttamanio tamanioBuffer;
};

#endif /* REGISTROMALEABLE_H_ */
