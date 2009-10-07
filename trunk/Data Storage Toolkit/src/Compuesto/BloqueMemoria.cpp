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
Ttamanio BloqueMemoria::deserializar(void*entrada){
	char*p=(char*)entrada;
	Ttamanio offset=Bloque::deserializar(entrada);
	sucio=*(bool*)(p+offset);
	offset+=sizeof(bool);
	escrito=*(bool*)(p+offset);
	offset+=sizeof(bool);
	return offset;
};
Ttamanio BloqueMemoria::serializar(void*salida){
	char*p=(char*)salida;
	Ttamanio offset=Bloque::serializar(salida);
	*(bool*)(p+offset)=sucio;
	offset+=sizeof(bool);
	*(bool*)(p+offset)=escrito;
	offset+=sizeof(bool);
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
Componente* BloqueMemoria::get(Ttamanio nroComponente){
	return Bloque::get(nroComponente);
};
Ttamanio BloqueMemoria::cantidadComponentes(){
	return Bloque::cantidadComponentes();
};
bool BloqueMemoria::insertar(Componente*componente,Ttamanio posicion){
	return Bloque::insertar(componente,posicion);
};
bool BloqueMemoria::eliminar(Ttamanio posicion){
	return Bloque::eliminar(posicion);
};
bool BloqueMemoria::agregar(Componente*componente){
	return Bloque::agregar(componente);
};
Componente* BloqueMemoria::reemplazar(Componente*componente,Ttamanio posicion){
	return Bloque::reemplazar(componente,posicion);
};
