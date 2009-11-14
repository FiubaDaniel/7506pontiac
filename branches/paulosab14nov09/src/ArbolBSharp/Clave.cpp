/*
 * Clave.cpp
 *
 *  Created on: 09/10/2009
 *      Author: daniel
 */

#include "Clave.h"
Clave::Clave(const Clave & otra){
	for(Ttamanio i=0;i<otra.atributosDeClave.size();i++){
		atributosDeClave.push_back(  otra.atributosDeClave.at(i)->clonar()   );
	}
}
Clave::Clave(Registro* registro,unsigned int cantidadAtributosEntrantes,...){
	va_list ap;
	va_start(ap, cantidadAtributosEntrantes);
	Atributo* atributo;
	char* nombre;
	cantidadAtributos = cantidadAtributosEntrantes;
	while(cantidadAtributosEntrantes>0){
		nombre = va_arg(ap,char*);
		atributo = registro->get(nombre);
		atributosDeClave.push_back(atributo->clonar());
		cantidadAtributosEntrantes--;
	}
	va_end(ap);
}
Clave::Clave(){}
void Clave::set(Registro*reg){
	for(Ttamanio i=0;i<atributosDeClave.size();i++){
		Atributo*att=atributosDeClave.at(i);
		std::string nombre=att->getNombre();
		*att=*reg->get(nombre);
	}
}
unsigned int Clave::getCantidadAtributos(){
	return atributosDeClave.size();
}
void Clave::setCantidadAtributos(unsigned int cant){
	cantidadAtributos = cant;
}
Clave* Clave::clonarce(){
	Clave* clon = new Clave();
	clon->setCantidadAtributos(cantidadAtributos);
	for(Ttamanio i=0;i < atributosDeClave.size();i++){
		clon->atributosDeClave.push_back(atributosDeClave.at(i)->clonar());
	}
	return clon;
}
Atributo* Clave::getAtributo(Ttamanio posicion){
	return atributosDeClave.at(posicion);
}
Atributo* Clave::getAtributo(std::string nombre){
	for(Ttamanio i=0;i<atributosDeClave.size();i++)
		if(atributosDeClave.at(i)->getNombre()==nombre)
			return atributosDeClave.at(i);
	return NULL;
}
void Clave::deserializar(std::streambuf&buf){
	for(Ttamanio i=0;i<atributosDeClave.size();i++){
		atributosDeClave.at(i)->deserializar(buf);
	}
}
void Clave::serializar(std::streambuf&buf){
	for(Ttamanio i=0;i<atributosDeClave.size();i++){
		atributosDeClave.at(i)->serializar(buf);
	}
}
int Clave::tamanioSerializado(){
	Ttamanio tamanio=0;
	for(Ttamanio i=0;i<atributosDeClave.size();i++){
		tamanio+=atributosDeClave.at(i)->tamanioSerializado();
	}
	tamanio= tamanio+sizeof(cantidadAtributos);
	int retorno = (int)tamanio;
	return retorno;
}
Clave::~Clave(){
	for(Ttamanio i=0;i<atributosDeClave.size();i++){
		delete atributosDeClave.at(i);
	}
}
