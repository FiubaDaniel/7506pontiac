/*
 * RegistroMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "RegistroMemoria.h"
RegistroMemoria::RegistroMemoria(Registro*registro):RegistroMemoria::Registro(){}
RegistroMemoria::~RegistroMemoria(){}

Ttamanio RegistroMemoria::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(flags);
	entrada.sgetn((char*)&estado,offset);
	offset+=Registro::deserializar(entrada);
	return offset;
}
Ttamanio RegistroMemoria::serializar(std::streambuf&salida){
	Ttamanio offset=sizeof(flags);
	salida.sputn((char*)&estado,offset);
	offset+=Registro::serializar(salida);
	return offset;
}
Ttamanio RegistroMemoria::tamanioSerializado(){
	return Registro::tamanioSerializado()+sizeof(char)+sizeof(Ttamanio);
}
Componente* RegistroMemoria::clonar(){
	RegistroMemoria* clon=new RegistroMemoria(this);
	clon->estado=estado;
	return clon;
}

