/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "EARegistros.h"

EARegistros::EARegistros() {}
EARegistros::~EARegistros() {}
void EARegistros::escribir(Almacenamiento *almacen, Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->posicionarByte(nroRegistro);
		almacen->escribir(registroSerializado,tamanioRegistro);
	}
}
void EARegistros::leer(Almacenamiento *almacen, Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		almacen->posicionarByte(nroRegistro);
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->deserializar(buf);
	}
}

size_t EARegistros::insertar(Almacenamiento *almacen, Componente *componente){
	size_t posicionDeInsercion;
	if(siguienteLibre<tamanioEncabezado){
		almacen->posicionarAlfinal();
		posicionDeInsercion=almacen->posicionActual();
	}else{
		posicionDeInsercion=siguienteLibre;
		almacen->posicionarByte(siguienteLibre);
		almacen->leer((char*)siguienteLibre,sizeof(siguienteLibre));
	}
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->escribir(registroSerializado,tamanioRegistro);
	}
	return (posicionDeInsercion-tamanioEncabezado)/tamanioRegistro;
}

bool EARegistros::modificar(Almacenamiento *almacen, Componente *componente){
	size_t posicionDelModificado=0;
	//TODO busqueda
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL && posicionDelModificado!=0){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->posicionarByte(posicionDelModificado);
		almacen->escribir(registroSerializado,tamanioRegistro);
		return true;
	}
	return false;
}

void EARegistros::eliminar(Almacenamiento *almacen, Componente *componente){
	size_t posicionEliminado=0;
	//TODO busqueda
	almacen->posicionarByte(posicionEliminado);
	almacen->escribir((char*)siguienteLibre,sizeof(siguienteLibre));
	siguienteLibre=posicionEliminado;
}

void EARegistros::posicionarComponente(size_t nroCompuesto){
	nroRegistro=nroCompuesto*tamanioRegistro+tamanioEncabezado;
}
bool EARegistros::buscar(Almacenamiento *almacen, Componente *componente){
	return false;
}




