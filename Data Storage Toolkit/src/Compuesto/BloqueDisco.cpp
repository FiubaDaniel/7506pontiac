/*
 * BloqueDisco.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "BloqueDisco.h"
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
Ttamanio BloqueDisco::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(bool);
	entrada.sgetn((char*)&borrado,offset);
	if(!borrado){
		offset=Bloque::deserializar(entrada);
	}
	return offset;
};
Ttamanio BloqueDisco::serializar(std::streambuf&salida){
	Ttamanio offset=sizeof(bool);
	salida.sputn((char*)&borrado,offset);
	if(!borrado){
		offset=Bloque::serializar(salida);
	};
	return offset+sizeof(bool);
};
Ttamanio BloqueDisco::tamanioSerializado(){
	return Bloque::tamanioSerializado()+sizeof(bool);
};
Componente* BloqueDisco::clonar(){
	BloqueDisco* clon=new BloqueDisco(this);
	//TODO poner estado del clon
	return clon;
};
