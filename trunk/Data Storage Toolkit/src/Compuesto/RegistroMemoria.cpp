/*
 * RegistroMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "RegistroMemoria.h"
RegistroMemoria::RegistroMemoria(Registro*registro,Ttamanio nroCompuesto,Ttamanio nroRegistro):RegistroMemoria::Registro(){};
RegistroMemoria::~RegistroMemoria(){};
bool RegistroMemoria::estaEscrito(){return flagSucioEscrito and ESCRITO;};
bool RegistroMemoria::estaSucio(){return flagSucioEscrito and SUCIO;};
void RegistroMemoria::setSucio(bool valor){
	if(valor)
		flagSucioEscrito=flagSucioEscrito or SUCIO;
	else
		flagSucioEscrito=flagSucioEscrito and (not SUCIO);
};
void RegistroMemoria::setEscrito(bool valor){
	if(valor)
			flagSucioEscrito=flagSucioEscrito or ESCRITO;
		else
			flagSucioEscrito=flagSucioEscrito and (not ESCRITO);
};
Ttamanio RegistroMemoria::deserializar(void*entrada){
	Ttamanio offset=Registro::deserializar(entrada);
	char*p=(char*)entrada;
	flagSucioEscrito=*(bool*)(p+offset);
	offset+=sizeof(char);
	nroCompuesto=*(Ttamanio*)(p+offset);
	return offset;
};
Ttamanio RegistroMemoria::serializar(void*salida){
	Ttamanio offset=Registro::serializar(salida);
	char*p=(char*)salida;
	*(bool*)(p+offset)=flagSucioEscrito;
	offset+=sizeof(char);
	*(Ttamanio*)(p+offset)=nroCompuesto;
	return offset;
};
Ttamanio RegistroMemoria::tamanioSerializado(){
	return Registro::tamanioSerializado()+sizeof(char)+sizeof(Ttamanio);
};
Componente* RegistroMemoria::clonar(){
	RegistroMemoria* clon=new RegistroMemoria(this,nroCompuesto,nroRegitro);
	return clon;
};

