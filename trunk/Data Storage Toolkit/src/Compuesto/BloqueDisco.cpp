/*
 * BloqueDisco.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "BloqueDisco.h"
//TODO Implementar
BloqueDisco::BloqueDisco(Bloque* bloque):BloqueDisco::Bloque(){
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
			Componente* aux=bloque->get(i);
			this->componentes.push_back( aux->clonar());
	}
};
BloqueDisco::~BloqueDisco(){};
void BloqueDisco::setBorrado(bool valor){borrado=valor;};
bool BloqueDisco::estaBorrado(){
	return borrado;
};
Ttamanio BloqueDisco::deserializar(void*entrada){
	char*p=(char*)entrada;
	Ttamanio offset=Bloque::deserializar(entrada);
	borrado=*(bool*)(p+offset);
	offset+=sizeof(bool);
	return offset;
};
Ttamanio BloqueDisco::serializar(void*salida){
	char*p=(char*)salida;
	Ttamanio offset=Bloque::serializar(salida);
	*(bool*)(p+offset)=borrado;
	offset+=sizeof(bool);
	return offset;
};
Ttamanio BloqueDisco::tamanioSerializado(){
	return Bloque::tamanioSerializado()+sizeof(bool);
};
Componente* BloqueDisco::clonar(){
	BloqueDisco* clon=new BloqueDisco(this);
	//TODO poner estado del clon
	return clon;
};
