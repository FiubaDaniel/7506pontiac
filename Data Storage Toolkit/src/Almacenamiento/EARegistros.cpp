/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "EARegistros.h"

EARegistros::EARegistros(){
	almacen=NULL;
	registroSerializado=NULL;
	tamanioRegistro=0;
	tamanioEncabezado=sizeof(siguienteLibre)+sizeof(tamanioRegistro);
}
void EARegistros::finalizarAlmacenamiento(){
	if(almacen!=NULL){
		almacen->posicionarByte(0);
		almacen->escribir((char*)tamanioEncabezado,sizeof(tamanioEncabezado));
		almacen->escribir((char*)siguienteLibre,sizeof(siguienteLibre));
	}
}
EARegistros::~EARegistros() {
	finalizarAlmacenamiento();
	delete[] registroSerializado;
}
Almacenamiento* EARegistros::abrir(Almacenamiento*almacen){
	finalizarAlmacenamiento();
	Almacenamiento* anterior=almacen;
	this->almacen=almacen;
	almacen->posicionarByte(0);
	Ttamanio tamRegistro=0;
	almacen->leer((char*)tamRegistro,sizeof(tamanioRegistro));
	if(tamRegistro>tamanioRegistro){
		delete[] registroSerializado;
		registroSerializado=new char[tamRegistro];
		tamanioRegistro=tamRegistro;
	}
	almacen->leer((char*)siguienteLibre,sizeof(siguienteLibre));
	posicionarComponente(0);
	return anterior;
};
Almacenamiento* EARegistros::nuevo(Almacenamiento*almacen,Ttamanio tamregistro){
	finalizarAlmacenamiento();
	Almacenamiento* anterior=almacen;
	tamanioRegistro=tamregistro;
	registroSerializado=new char[tamregistro];
	tamanioEncabezado=sizeof(tamanioEncabezado)+sizeof(siguienteLibre);
	siguienteLibre=0;
	posicionarComponente(0);
	return anterior;
}
void EARegistros::escribir( Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->posicionarByte(nroRegistro);
		almacen->escribir(registroSerializado,tamanioRegistro);
	}
}
void EARegistros::leer( Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		almacen->posicionarByte(nroRegistro);
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->deserializar(buf);
	}
}

size_t EARegistros::insertar( Componente *componente){
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

bool EARegistros::modificar( Componente *componente){
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

bool EARegistros::eliminar( Componente *componente){
	size_t posicionEliminado=0;
	//TODO busqueda
	almacen->posicionarByte(posicionEliminado);
	almacen->escribir((char*)siguienteLibre,sizeof(siguienteLibre));
	siguienteLibre=posicionEliminado;
	return true;
}

void EARegistros::posicionarComponente(size_t nroCompuesto){
	nroRegistro=nroCompuesto*tamanioRegistro+tamanioEncabezado;
}
bool EARegistros::buscar(Componente *componente){
	return false;
}




