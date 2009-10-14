/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "EARegistros.h"

EARegistros::EARegistros() {

}
EARegistros::~EARegistros() {
}
void EARegistros::escribir(Almacenamiento *almacen, Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->posicionarByte(tamanioRegistro*nroRegistro);
		almacen->escribir(registroSerializado,tamanioRegistro);
	}
}
void EARegistros::leer(Almacenamiento *almacen, Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		almacen->posicionarByte(tamanioRegistro*nroRegistro);
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->deserializar(buf);
	}
}
bool EARegistros::buscar(Almacenamiento *almacen, Componente *componente){
	return false;
}

size_t EARegistros::insertar(Almacenamiento *almacen, Componente *componente){
	return 0;
}

bool EARegistros::modificar(Almacenamiento *almacen, Componente *componente){
	return false;
}

void EARegistros::eliminar(Almacenamiento *almacen, Componente *componente){
}

void EARegistros::posicionarComponente(size_t nroCompuesto){
	nroRegistro=nroCompuesto;
}





