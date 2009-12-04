/*
 * EARegistros.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EARegistros.h"
#include <iostream>
EARegistros::EARegistros(Registro*registro){
	registroSerializado=NULL;
	tamanioRegistro=registro->tamanioSerializado();
	this->registro=(Registro*)registro->clonar();
}

EARegistros::~EARegistros() {
	delete registro;
}
bool EARegistros::abrir( ){
	almacen->posicionar(0);
	almacen->leer(&tamanioRegistro,sizeof(tamanioRegistro));
	almacen->leer(&siguienteRegLibre,sizeof(siguienteRegLibre));
	registroSerializado=new char[tamanioRegistro];
	posicionarComponente(0);
	return true;
}
void EARegistros::crear( ){
	registroSerializado=new char[tamanioRegistro];
	siguienteRegLibre=0;
	almacen->posicionar(0);
	almacen->escribir(&tamanioRegistro,sizeof(tamanioRegistro));
	almacen->escribir(&siguienteRegLibre,sizeof(siguienteRegLibre));
	posicionarComponente(0);
}
void EARegistros::cerrar( ){
	delete[] registroSerializado;
	registroSerializado=NULL;
	almacen->posicionar(0);
	almacen->escribir(&tamanioRegistro,sizeof(tamanioRegistro));
	almacen->escribir(&siguienteRegLibre,sizeof(siguienteRegLibre));
}
int EARegistros::comparar(Registro*reg1,Registro*reg2){
	clave->set(reg1);
	Clave*clave2=clave->clonarce();
	clave2->set(reg2);
	int resultado=comparador->Comparar(clave,clave2);
	delete clave2;
	return resultado;
}

void EARegistros::escribir(Registro*registro ){
	if(!almacen->fin()){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->serializar(buf);
		almacen->escribir(registroSerializado,tamanioRegistro);
		nroRegistro++;
	}
}
void EARegistros::leer(Registro*registro ){
	if(!almacen->fin()){
		almacen->leer(registroSerializado,tamanioRegistro);
		std::stringbuf buf;
		buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
		buf.pubsetbuf(registroSerializado,tamanioRegistro);
		registro->deserializar(buf);
		nroRegistro++;
	}
}
bool EARegistros::escribir(Componente *componente ){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<=siguienteRegLibre){
		if(nroRegistro==siguienteRegLibre)
			siguienteRegLibre++;
		if(colaDeCambios){
			clave->set(registro);
			colaDeCambios->push(Cambio(clave,nroRegistro,Cambio::Alta));
		}
		escribir(registro);
		return true;
	}
	return false;
}

bool EARegistros::leer(Componente *componente ){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<siguienteRegLibre){
		leer(registro);
		return true;
	}
	return false;
}

bool EARegistros::insertar(Componente *componente ){
	posicionarComponente(siguienteRegLibre);
	return escribir(componente);
}

bool EARegistros::modificar(Componente *componente ){
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo!=NULL && nroRegistro<siguienteRegLibre){
		leer(registro);
		if(comparar(nuevo,registro)==0){
			nroRegistro--;
			if(colaDeCambios){
				clave->set(nuevo);
				colaDeCambios->push(Cambio(clave,nroRegistro,Cambio::Modificacion));
			}
			posicionarComponente(nroRegistro);
			escribir(nuevo);

			return true;
		}
	}
	return false;
}

bool EARegistros::eliminar(Componente *componente ){
	Registro*eliminado=dynamic_cast<Registro*>(componente);
	if(eliminado!=NULL && nroRegistro<siguienteRegLibre){
		Referencia borrado=nroRegistro;
		leer(registro);
		if(comparar(eliminado,registro)==0){
			siguienteRegLibre--;
			posicionarComponente(siguienteRegLibre);
			leer(registro);
			if(colaDeCambios){
				clave->set(eliminado);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Baja));
				clave->set(registro);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Reubicacion));
			}
			posicionarComponente(borrado);
			escribir(registro);
			return true;
		}
	}
	return false;
}
bool EARegistros::eliminar(Clave *unaClave ){
	if(nroRegistro<siguienteRegLibre){
		Referencia borrado=nroRegistro;
		leer(registro);
		clave->set(registro);
		if(comparador->Comparar(clave,unaClave)==0){
			siguienteRegLibre--;
			posicionarComponente(siguienteRegLibre);
			leer(registro);
			if(colaDeCambios){
				colaDeCambios->push(Cambio(unaClave,borrado,Cambio::Baja));
				clave->set(registro);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Reubicacion));
			}
			posicionarComponente(borrado);
			escribir(registro);
			return true;
		}
	}
	return false;
}
bool EARegistros::posicionarComponente(Referencia nroCompuesto ){
	nroRegistro=nroCompuesto;
	almacen->posicionar(nroCompuesto*tamanioRegistro+sizeof(tamanioRegistro)+sizeof(siguienteRegLibre));
	if(almacen->fin()){
		almacen->clear();
		return false;
	}
	return true;
}
bool EARegistros::siguiente(Componente *componente){
	if(!almacen->fin() && nroRegistro < siguienteRegLibre)
		return leer(componente);
	return false;
}

bool EARegistros::obtener(Componente*componente ){
	return leer(componente);
}
Referencia EARegistros::posicionComponente(){
	return nroRegistro;
}
bool EARegistros::buscar(Componente*componente ){
	bool encontrado=false;
	bool fin=false;
	posicionarComponente(0);
	do{
		fin=leer((Componente*)registro);
		if(comparar(registro,(Registro*)componente)==0){
			std::stringbuf buf;
			buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
			buf.pubsetbuf(registroSerializado,tamanioRegistro);
			componente->deserializar(buf);
			encontrado=true;
		}
	}while((not fin )and not encontrado);
	return encontrado;
}
bool EARegistros::buscar(Clave*unaClave){
	bool fin=false;
	posicionarComponente(0);
	do{
		Referencia pos=posicionComponente();
		fin=leer((Componente*)registro);
		clave->set(registro);
		if(comparador->Comparar(unaClave,clave)==0){
			posicionarComponente(pos);
			return true;
		}
	}while(not fin );
	return false;
}
Componente*EARegistros::getComponenteUsado(){
	return registro;
}
unsigned EARegistros::getTamanioComponenteUsado(){
	return tamanioRegistro;
}
void EARegistros::imprimirMetada(std::ostream&out){
	out<<"Estrategia Registros: "<<std::endl;
	out<< " tamanio Registro: "<< tamanioRegistro ;
	out<< " cantidad Registros: "<< siguienteRegLibre;
	out<<std::endl;
}
std::string EARegistros::getMetadata(){
	std::string resultado;
	char*p=(char*)&tamanioRegistro;
	for(unsigned i=0;i<sizeof(tamanioRegistro);i++){
		resultado.push_back(*p);
		p++;
	}
	p=(char*)&siguienteRegLibre;
	for(unsigned i=0;i<sizeof(siguienteRegLibre);i++){
		resultado.push_back(*p);
		p++;
	}
	return resultado;
}
void EARegistros::setMetadata(char* metadata){
	tamanioRegistro=*(Ttamanio*)metadata;
	metadata+=sizeof(Ttamanio);
	siguienteRegLibre=*(Referencia*)metadata;
}
