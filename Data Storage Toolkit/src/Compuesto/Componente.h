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
class Componente {
public:
	Componente(){};
	virtual ~Componente(){};
	//TODO ver si sirve o no
	virtual Componente* clonar()=0;
	//virtual Atributo* get(Ttamanio posicion)=0;
	virtual void setbytes(std::streambuf& pbuffer)=0;// tarea Serializador
	virtual void getbytes(std::streambuf& pbuffer)=0;// tarea Serializador
	//virtual write(ostream);
	//virtaul read(ostream):
	//virtual Ttamanio cantidadAtributos()=0;
	virtual Ttamanio cantidadBytes()=0;
};

#endif /* REGISTRO_H_ */
