/*
 * RegistroMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "RegistroMemoria.h"
RegistroMemoria::RegistroMemoria(Registro*registro,Ttamanio nroCompuesto,Ttamanio nroRegistro):RegistroMemoria::Registro(){};
RegistroMemoria::~RegistroMemoria(){};
bool RegistroMemoria::estaEscrito(){return estado&escrito;};
bool RegistroMemoria::estaSucio(){return estado&sucio;};
void RegistroMemoria::setSucio(bool valor){
	if(valor)estado=flags(estado|sucio);
	else estado=flags(estado|!sucio);
};
void RegistroMemoria::setEscrito(bool valor){
	if(valor)estado=flags(estado|escrito);
	else estado=flags(estado|!escrito);
};
Ttamanio RegistroMemoria::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(flags);
	entrada.sgetn((char*)&estado,offset);
	//TODO nro compuesto vs nro registro, q hacer si sucio
	if(!estaSucio())
		offset+=Registro::deserializar(entrada);
	return offset;
};
Ttamanio RegistroMemoria::serializar(std::streambuf&salida){
	Ttamanio offset=sizeof(flags);
	salida.sputn((char*)&estado,offset);
	//salida.sputn((char*)&nroCompuesto,sizeof(Ttamanio));
	//Todo nroCompuesto vs nroRegistro
	offset+=Registro::serializar(salida);
	return offset;
};
Ttamanio RegistroMemoria::tamanioSerializado(){
	return Registro::tamanioSerializado()+sizeof(char)+sizeof(Ttamanio);
};
Componente* RegistroMemoria::clonar(){
	RegistroMemoria* clon=new RegistroMemoria(this,nroCompuesto,nroRegitro);
	return clon;
};

