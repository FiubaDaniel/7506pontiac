/*
 * RegistroDuro.cpp
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#include "RegistroDuro.h"

RegistroDuro::RegistroDuro(unsigned int cantidadAtributos, ...):RegistroDuro::Componente() {
	/*locales*/
	va_list ap;
	va_start(ap, cantidadAtributos);
	Atributo* atributo;
	/*inicializacion y cargar*/
	while(cantidadAtributos>0){
		atributo = va_arg(ap, Atributo*);
		atributos.push_back(atributo->clonar());
		cantidadAtributos--;
	}
	if(! cantidadAtributos>0);//TODO else trow error
	va_end(ap);
}

RegistroDuro::~RegistroDuro() {
	for(Ttamanio i= atributos.size()-1;i>=0;i--){
			delete atributos.at(i);
	}
}
//TODO ver si sirve o no
Componente* RegistroDuro::clonar(){
	RegistroDuro* clon=new RegistroDuro(true,0);
	for(Ttamanio i=0;i<atributos.size();i++){
		clon->atributos.push_back(atributos.at(i)->clonar());
	}
	return clon;
};
Atributo* RegistroDuro::get(Ttamanio posicion){
	return atributos.at(posicion);
};

Atributo* RegistroDuro::get(std::string nombre){
	/*Busqueda lineal de atributo con el nombre*/
	for(Ttamanio i=0;i<atributos.size();i++)
		if(atributos.at(i)->nombre()==nombre) return atributos.at(i);
	return NULL;
};
void RegistroDuro::setbytes(std::streambuf& pbuffer){
	//TODO estrategia serializador
};
void RegistroDuro::getbytes(std::streambuf& pbuffer){
	//TODO estrategia serializador
};
Ttamanio RegistroDuro::cantidadBytes(){
	Ttamanio tamanio=0;
	for(Ttamanio i=atributos.size();i>0;i--){
		tamanio+=atributos.at(i)->nrobytes;
	}
	return tamanio;
};
Ttamanio RegistroDuro::cantidadAtributos(){return atributos.size();};
//Ttamanio RegistroDuro::cantidadBytes(){return tamanioBuffer;};
