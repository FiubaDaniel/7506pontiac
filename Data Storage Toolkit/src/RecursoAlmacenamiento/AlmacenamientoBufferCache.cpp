/*
 * AlmacenamientoBufferCache.cpp
 *
 *  Created on: 10/12/2009
 *      Author: paulo
 */

#include "AlmacenamientoBufferCache.h"

AlmacenamientoBufferCache::AlmacenamientoBufferCache(Almacenamiento*archivo ){
	this->almacen=archivo;
	this->estrategia=archivo->getEstrategia();
	if(this->estrategia){
		this->estrategia->setAlmacenamiento(this);
		this->almacen->setEstrategia(NULL);
	}
	this->buffer=NULL;
	this->nro_bloque=-1;
}
AlmacenamientoBufferCache::~AlmacenamientoBufferCache() {
	delete buffer;
	delete almacen;
}
bool AlmacenamientoBufferCache::abrir(const char* nombre){
	if(not almacen->abrir(nombre))
		return false;
	this->nombre=nombre;
	if(estrategia){
		this->tamanio_metadata=estrategia->getMetadata().size();
		this->tamanio_bloque=estrategia->getTamanioComponenteUsado();
		buffer=new BufferCache(almacen,this->tamanio_bloque);
		return estrategia->abrir();
	}
	return true;

}
void AlmacenamientoBufferCache::crear(const char *nombre){
	almacen->crear(nombre);
	if(estrategia){
		estrategia->setAlmacenamiento(this);
		this->tamanio_metadata=estrategia->getMetadata().size();
		this->tamanio_bloque=estrategia->getTamanioComponenteUsado();
		buffer=new BufferCache(almacen,this->tamanio_bloque);
		estrategia->crear();
	}

}
void AlmacenamientoBufferCache::escribir(const void* bytes,Referencia cantidad){
	if(posicion>=tamanio_metadata){
		nro_bloque=(posicion-tamanio_metadata)/tamanio_bloque;
		char*datos=(char*)(const_cast<void*>(bytes));
		switchEstrategia();
		buffer->escribir(nro_bloque,datos);
		switchEstrategia();
		nro_bloque++;
	}else{
		almacen->escribir(bytes,cantidad);

	}
	posicion+=cantidad;
}

void AlmacenamientoBufferCache::leer(void* bytes,Referencia cantidad){
	if(posicion>=tamanio_metadata){
		nro_bloque=(posicion-tamanio_metadata)/tamanio_bloque;
		switchEstrategia();
		buffer->leer(nro_bloque,(char*)bytes);
		switchEstrategia();
		nro_bloque++;
	}else{
		almacen->leer(bytes,cantidad);

	}
	posicion+=cantidad;
}
void AlmacenamientoBufferCache::posicionar(Referencia posicion){
	this->posicion=posicion;
	if(posicion<tamanio_metadata)
		almacen->posicionar(posicion);

}
Referencia AlmacenamientoBufferCache::posicionActual(){
	return posicion;
}
void AlmacenamientoBufferCache::escribir(const void* unByte){}
void AlmacenamientoBufferCache::posicionarAlfinal(){}
void AlmacenamientoBufferCache::leer(void* unBytes){}
bool AlmacenamientoBufferCache::bien(){
	return almacen->bien();
}
bool AlmacenamientoBufferCache::fin(){
	return almacen->fin();
}
/*Limpia los flags de fin de archivo y error*/
void AlmacenamientoBufferCache::clear(){
	almacen->clear();
}
void AlmacenamientoBufferCache::cerrar(){
	if(estrategia){
		estrategia->cerrar();
		delete buffer;
		buffer=NULL;
	}
	almacen->cerrar();
}
Almacenamiento*AlmacenamientoBufferCache::clonar(){
	return almacen->clonar();
}
void AlmacenamientoBufferCache::switchEstrategia(){
	if(estrategia!=NULL){
		almacen->setEstrategia(estrategia);
		estrategia->setAlmacenamiento(almacen);
		estrategia=NULL;
	}else{
		setEstrategia(almacen->getEstrategia());
		almacen->setEstrategia(NULL);
		estrategia->setAlmacenamiento(this);
	}
};
