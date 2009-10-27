/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EARegistros.h"
#include <iostream>
EARegistros::EARegistros(){
	almacen=NULL;
	registroSerializado=NULL;
	tamanioRegistro=0;
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
	Almacenamiento* anterior=this->almacen;
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
Almacenamiento* EARegistros::nuevo(Almacenamiento*almacenamiento,Ttamanio tamregistro){
	finalizarAlmacenamiento();
	Almacenamiento* anterior=almacen;
	almacen=almacenamiento;
	tamanioRegistro=tamregistro;
	registroSerializado=new char[tamregistro];
	tamanioEncabezado=sizeof(siguienteLibre);
	siguienteLibre=0;
	this->almacen->posicionarByte(0);
	this->almacen->escribir(&siguienteLibre,sizeof(siguienteLibre));
	return anterior;
}
bool EARegistros::escribir( Componente *componente){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->escribir(registroSerializado,tamanioRegistro);
		return true;
	}
	return false;
}
bool EARegistros::leer( Componente *componente){
	Registro*registro=NULL;
	if((registro=dynamic_cast<Registro*>(componente))){
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf;
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		buf.pubseekpos(0,std::ios_base::binary | std::ios_base::out );
		registro->deserializar(buf);
		return true;
	}
	return false;
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

bool EARegistros::modificar(Componente *componente){
	if(!buscar(componente))return false;
	size_t posicionDelModificado=almacen->posicionActual()-tamanioRegistro;
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->posicionarByte(posicionDelModificado);
		almacen->escribir(registroSerializado,tamanioRegistro);
		return true;
	}
	return false;
}

bool EARegistros::eliminar(Componente *componente){
	if(!buscar(componente))return false;
	size_t posicionEliminado=almacen->posicionActual()-tamanioRegistro;
	/*siguienteLibre=final-tamanioRegistro;
	almacen->posicionarByte(siguienteLibre);
	almacen->leer(registroSerializado,tamanioRegistro);
	almacen->posicionarByte(posicionEliminado);
	almacen->escribir(registroSerializado,tamanioRegistro);
	almacen->truncar();*/
	almacen->posicionarByte(posicionEliminado);
	almacen->escribir((char*)siguienteLibre,sizeof(siguienteLibre));
	siguienteLibre=posicionEliminado;
	return true;
}

void EARegistros::posicionarComponente(size_t nroCompuesto){
	nroRegistro=nroCompuesto*tamanioRegistro+tamanioEncabezado;
	almacen->posicionarByte(nroRegistro);
}
bool EARegistros::buscar(Componente *componente){
	bool encontrado=false;
	Registro*registro=dynamic_cast<Registro*>(componente);
	Registro*reg=(Registro*)componente->clonar();
	posicionarComponente(0);
	do{
		leer(reg);
		if(0==comparar(registro,reg)){
		 	encontrado=true;
		 	std::stringbuf buf;
			buf.pubsetbuf(registroSerializado,tamanioRegistro);
			buf.pubseekpos(0,std::ios_base::binary|std::ios_base::in| std::ios_base::out);
		 	componente->deserializar(buf);
		}
	}while(!almacen->fin() && !encontrado);
	delete reg;
	return encontrado;
}


