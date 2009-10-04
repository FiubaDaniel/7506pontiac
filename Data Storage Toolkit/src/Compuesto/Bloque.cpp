/*
 * Bloque.cpp
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#include "Bloque.h"
/*------------------------------------------------------------------------------------*/
Bloque::Bloque(Ttamanio tamanio,Componente*componente) : Bloque::Componente(){
	componentes.push_back(componente->clonar());
	espacioLibre=tamanioBloque=tamanio;
	espacioLibre-=componente->cantidadBytes();
	corriente=0;
};
Bloque::~Bloque() {
	for(Ttamanio i= componentes.size()-1;i>=0;i--){
				delete componentes.at(i);
	}
};
void Bloque::setbytes(std::streambuf& pbuffer){
	//TODO serializador
};
void Bloque::getbytes(std::streambuf& pbuffer){
	//TODO serializador
};

Componente* Bloque::clonar(){
	//TODO ver si sirve o no
	Bloque* clon=new Bloque(tamanioBloque,componentes.at(0)->clonar());
	return clon;
};
Ttamanio Bloque::cantidadBytes(){return tamanioBloque;};
/*------------------------------------------------------------------------------------*/
Componente* Bloque::get(){
	return componentes.at(corriente)->clonar();
};
bool Bloque::apuntar(Ttamanio nroComponente){
	if(nroComponente>=componentes.size())return false;
	corriente=nroComponente;
	return true;
};
void Bloque::eliminar(){
	espacioLibre+=componentes.at(corriente)->cantidadBytes();
	componentes.erase(componentes.begin()+corriente);
};
Componente* Bloque::modificar(Componente* nuevo){
	Componente* anterior=NULL;
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=componentes.at(corriente)->cantidadBytes();
	nuevoTamanio+=nuevo->cantidadBytes();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		anterior=componentes.at(corriente);
		componentes.at(corriente)=nuevo;
	}
	return anterior;
};
bool Bloque::insertar(Componente* nuevo){
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=componentes.at(corriente)->cantidadBytes();
	nuevoTamanio+=nuevo->cantidadBytes();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		componentes.insert(componentes.begin()+corriente-1,nuevo);
		return true;
	}
	return false;
};
//intenta insertar detras del registro actual si puede
bool Bloque::append(Componente* nuevo){
	Ttamanio nuevoTamanio=espacioLibre;
	nuevoTamanio-=componentes.at(corriente)->cantidadBytes();
	nuevoTamanio+=nuevo->cantidadBytes();
	if(nuevoTamanio>0){
		espacioLibre=nuevoTamanio;
		componentes.push_back(nuevo->clonar());
		return true;
	}
	return false;
};//intenta insdertar detras del ultimo registro
Ttamanio Bloque::cantidadComponente(){
	return componentes.size();
};
