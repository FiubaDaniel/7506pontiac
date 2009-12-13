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
	Clave*clave2=clave->clonarce();
	clave->set(reg1);
	clave2->set(reg2);
	int resultado=comparador->Comparar(clave,clave2);
	delete clave2;
	return resultado;
}

void EARegistros::escribirRegistro(Registro*registro ){
	std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
	buf.pubsetbuf(registroSerializado,tamanioRegistro);
	registro->serializar(buf);
	almacen->escribir(registroSerializado,tamanioRegistro);
	nroRegistro++;
}
bool EARegistros::leerRegistro(Registro*registro ){
	almacen->leer(registroSerializado,tamanioRegistro);
	if(almacen->fin()){
		almacen->clear();
		return false;
	}
	std::stringbuf buf;
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in );
	buf.pubsetbuf(registroSerializado,tamanioRegistro);
	registro->deserializar(buf);
	nroRegistro++;
	return true;
}
bool EARegistros::escribir(Componente *componente ){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<=siguienteRegLibre){
		escribirRegistro(registro);
		if(nroRegistro<=siguienteRegLibre)
			siguienteRegLibre=nroRegistro;
		return true;
	}
	return false;
}

bool EARegistros::leer(Componente *componente ){
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL&&nroRegistro<siguienteRegLibre){
		return leerRegistro(registro);
	}
	return false;
}

bool EARegistros::insertar(Componente *componente ){
	posicionarComponente(siguienteRegLibre);
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo){
		escribirRegistro(nuevo);
		if(colaDeCambios){
			clave->set(nuevo);
			colaDeCambios->push(Cambio(clave,siguienteRegLibre,Cambio::Alta));
		}
		siguienteRegLibre++;
		return true;
	}
	return false;
}

bool EARegistros::modificar(Componente *componente ){
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo!=NULL && nroRegistro<siguienteRegLibre){
		Referencia eliminado=nroRegistro;
		if(leerRegistro(registro)){
			if(comparar(nuevo,registro)==0){
				if(colaDeCambios){
					clave->set(nuevo);
					colaDeCambios->push(Cambio(clave,eliminado,Cambio::Modificacion));
				}
				posicionarComponente(eliminado);
				escribirRegistro(nuevo);
				return true;
			}
		}
	}
	return false;
}

bool EARegistros::eliminar(Componente *componente ){
	Registro*eliminado=dynamic_cast<Registro*>(componente);
	if(eliminado!=NULL && nroRegistro<siguienteRegLibre){
		Referencia borrado=nroRegistro;
		leerRegistro(registro);
		if(comparar(eliminado,registro)==0){
			siguienteRegLibre--;
			posicionarComponente(siguienteRegLibre);
			leerRegistro(registro);
			if(colaDeCambios){
				clave->set(eliminado);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Baja));
				clave->set(registro);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Reubicacion));
			}
			posicionarComponente(borrado);
			escribirRegistro(registro);
			return true;
		}
	}
	return false;
}
bool EARegistros::eliminar(Clave *unaClave ){
	if(nroRegistro<siguienteRegLibre){
		Referencia borrado=nroRegistro;
		leerRegistro(registro);
		clave->set(registro);
		if(comparador->Comparar(clave,unaClave)==0){
			siguienteRegLibre--;
			posicionarComponente(siguienteRegLibre);
			leerRegistro(registro);
			if(colaDeCambios){
				colaDeCambios->push(Cambio(unaClave,borrado,Cambio::Baja));
				clave->set(registro);
				colaDeCambios->push(Cambio(clave,borrado,Cambio::Reubicacion));
			}
			posicionarComponente(borrado);
			escribirRegistro(registro);
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
	resultado.append((char*)&tamanioRegistro,sizeof(Ttamanio));
	resultado.append((char*)&siguienteRegLibre,sizeof(Ttamanio));
	return resultado;
}
void EARegistros::setMetadata(char* metadata){
	tamanioRegistro=*(Ttamanio*)metadata;
	metadata+=sizeof(Ttamanio);
	siguienteRegLibre=*(Referencia*)metadata;
}
Referencia EARegistros::ultimoComponente(){
	return siguienteRegLibre-1;
}
