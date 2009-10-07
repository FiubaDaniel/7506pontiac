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
BloqueMemoria::~BloqueMemoria(){
	//this->~Bloque();
};
bool BloqueMemoria::estaEscrito(){return escrito;};
bool BloqueMemoria::estaSucio(){return sucio;};
void BloqueMemoria::setSucio(bool valor){sucio=valor;};
void BloqueMemoria::setEscrito(bool valor){escrito=valor;};
Ttamanio BloqueMemoria::deserializar(std::istream&entrada){
	Ttamanio offset=sizeof(bool);
	entrada.read((char*)&sucio,offset);
	if(!sucio){
		entrada.read((char*)&escrito,offset);
		offset*=2;
		offset+=Bloque::deserializar(entrada);
	}
	return offset;
};
Ttamanio BloqueMemoria::serializar(std::ostream&salida){
	Ttamanio offset=sizeof(bool);
	salida.write((char*)&sucio,offset);
	salida.write((char*)&escrito,offset);
	offset*=2;
	offset+=Bloque::serializar(salida);
	return offset;
};
Ttamanio BloqueMemoria::tamanioSerializado(){
	return Bloque::tamanioSerializado()+2*sizeof(bool);
};
Componente* BloqueMemoria::clonar(){
	BloqueMemoria* clon=new BloqueMemoria(this);
	clon->sucio=sucio;
	clon->escrito=escrito;
	return clon;
};
