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
}
BloqueMemoria::~BloqueMemoria(){}

Ttamanio BloqueMemoria::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(flags);
	entrada.sgetn((char*)&estado,offset);
	offset+=Bloque::deserializar(entrada);
	return offset;
}
Ttamanio BloqueMemoria::serializar(std::streambuf&salida)const{
	Ttamanio offset=sizeof(flags);
	salida.sputn((char*)&estado,offset);
	offset+=Bloque::serializar(salida);
	return offset;
}
Ttamanio BloqueMemoria::tamanioSerializado()const{
	return Bloque::tamanioSerializado()+sizeof(flags);
}
Componente* BloqueMemoria::clonar()const{
	BloqueMemoria* clon=new BloqueMemoria(const_cast<BloqueMemoria*>(this));
	clon->estado=estado;
	return clon;
}
