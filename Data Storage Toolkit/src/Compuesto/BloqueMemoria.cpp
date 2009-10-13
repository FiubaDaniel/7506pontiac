/*
 * BloqueMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "BloqueMemoria.h"
BloqueMemoria::BloqueMemoria(Bloque* bloque):BloqueMemoria::Bloque(){
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
			Componente* aux=bloque->get(i);
			this->componentes.push_back( aux->clonar());
	}
};
BloqueMemoria::~BloqueMemoria(){};
bool BloqueMemoria::estaEscrito(){return estado&escrito;};
bool BloqueMemoria::estaSucio(){return estado&sucio;};
void BloqueMemoria::setSucio(bool valor){
	if(valor) estado =flags(estado|sucio);
	else estado=flags(estado|(!sucio));
};
void BloqueMemoria::setEscrito(bool valor){
	if(valor) estado =flags(estado|escrito);
	else estado=flags(estado|(!escrito));
};
Ttamanio BloqueMemoria::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(flags);
	entrada.sgetn((char*)&estado,offset);
	if(!estaSucio()){
		offset+=Bloque::deserializar(entrada);
	}
	return offset;
};
Ttamanio BloqueMemoria::serializar(std::streambuf&salida){
	Ttamanio offset=sizeof(flags);
	salida.sputn((char*)&estado,offset);
	if(!estaSucio())
		offset+=Bloque::serializar(salida);
	return offset;
};
Ttamanio BloqueMemoria::tamanioSerializado(){
	return Bloque::tamanioSerializado()+sizeof(flags);
};
Componente* BloqueMemoria::clonar(){
	BloqueMemoria* clon=new BloqueMemoria(this);
	//clon->estado=estado; TODO
	return clon;
};
