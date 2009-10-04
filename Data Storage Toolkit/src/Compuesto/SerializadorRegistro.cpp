/*
 * SerializadorRegistro.cpp
 *
 *  Created on: 03/10/2009
 *      Author: paulo
 */

#include "SerializadorRegistro.h"

SerializadorRegistro::SerializadorRegistro() {
}
SerializadorRegistro::~SerializadorRegistro() {
}
void SerializadorRegistro::serializar(Componente*componente,std::streambuf salida){
	RegistroDuro* registro=NULL;
	if((registro=dynamic_cast<RegistroDuro*>(componente))){
		for(Ttamanio i=registro->cantidadAtributos()-1;i>=0;i--){
			Atributo* at=registro->get(i);
			//tamanio atributo
			salida.sputc(at->nrobytes);
			//bytes del atributo
			at->getbytes(salida);
		}
	};
}
void SerializadorRegistro::deserializar(Componente*componente,std::streambuf entrada){
	RegistroDuro* registro=NULL;
	if((registro=dynamic_cast<RegistroDuro*>(componente))){
		for(Ttamanio i=registro->cantidadAtributos()-1;i>=0;i--){
			Atributo* at=registro->get(i);
			//tamanio atributo
			at->nrobytes=entrada.snextc();
			at->setbytes(entrada);
		}
	};
}
