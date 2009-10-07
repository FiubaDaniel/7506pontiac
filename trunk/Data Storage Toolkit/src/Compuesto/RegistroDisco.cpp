/*
 * RegistroDisco.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "RegistroDisco.h"

RegistroDisco::RegistroDisco(Registro*registro): RegistroDisco::Registro(){
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		Atributo* aux=registro->get(i);
		this->atributos.push_back( aux->clonar() );
	}
};
RegistroDisco::~RegistroDisco(){
	//this->~Registro();
};
Ttamanio RegistroDisco::deserializar(std::istream&entrada){
	Ttamanio offset=sizeof(bool);
	entrada.read((char*)&estaBorrado,offset);
	if(!estaBorrado)
		offset+=Registro::deserializar(entrada);
	return offset;
};
Ttamanio RegistroDisco::serializar(std::ostream&salida){
	Ttamanio offset=sizeof(bool);
	salida.write((char*)&estaBorrado,offset);
	offset+=Registro::serializar(salida);
	return offset;
};
Ttamanio RegistroDisco::tamanioSerializado(){
	return Registro::tamanioSerializado()+sizeof(bool);
};
Componente* RegistroDisco::clonar(){
	RegistroDisco* registro=new RegistroDisco(this);
	registro->estaBorrado=estaBorrado;
	return registro;
};
bool RegistroDisco::borrado(){return estaBorrado;};
void RegistroDisco::setBorrado(bool flag){ estaBorrado=flag;};

