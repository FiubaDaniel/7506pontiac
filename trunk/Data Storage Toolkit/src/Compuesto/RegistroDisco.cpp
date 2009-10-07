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
	this->~Registro();
};
Ttamanio RegistroDisco::deserializar(void*entrada){
	Ttamanio offset=Registro::deserializar(entrada);
	char* p=(char*)entrada;
	estaBorrado=*(bool*)(p+offset);
	return offset+sizeof(bool);
};
Ttamanio RegistroDisco::serializar(void*salida){
	char* p=(char*)salida;
	Ttamanio offset=Registro::serializar(salida);
	*(bool*)(p+offset)=estaBorrado;
	return offset+sizeof(bool);
};
Ttamanio RegistroDisco::tamanioSerializado(){
	return Registro::tamanioSerializado()+sizeof(bool);
};
Componente* RegistroDisco::clonar(){
	RegistroDisco* registro=new RegistroDisco(this);
	registro->estaBorrado=estaBorrado;
	return registro;
};
Atributo* RegistroDisco::get(Ttamanio posicion){
	return Registro::get(posicion);
};
Atributo* RegistroDisco::get(std::string nombre){
	return Registro::get(nombre);
};
Ttamanio RegistroDisco::cantidadAtributos(){
	return Registro::cantidadAtributos();
};
Atributo* RegistroDisco::getId(){
	return Registro::getId();

};
bool RegistroDisco::borrado(){return estaBorrado;};
void RegistroDisco::setBorrado(bool flag){ estaBorrado=flag;};

