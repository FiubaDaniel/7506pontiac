/*
 * RegistroMaleable.cpp
 *
 *  Created on: 02/10/2009
 *      Author: paulo
 */

#include "RegistroMaleable.h"

RegistroMaleable::RegistroMaleable(bool desalocar,unsigned int cantidadAtributos, ...):RegistroMaleable::Componente() {
	/*locales*/
	va_list ap;
	va_start(ap, cantidadAtributos);
	Atributo* atributo;
	/*inicializacion y cargar*/
	deboDesalocar=desalocar;
	while(cantidadAtributos>0){
		atributo = va_arg(ap, Atributo*);
		atributos.push_back(atributo);
		cantidadAtributos--;
	}
	if(! cantidadAtributos>0);//TODO else trow error
	va_end(ap);
}

RegistroMaleable::~RegistroMaleable() {
	if(deboDesalocar)
		for(Ttamanio i= atributos.size()-1;i>=0;i--){
			delete atributos.at(i);
		}
}
Atributo* RegistroMaleable::get(Ttamanio posicion){
	return atributos.at(posicion);
};

Atributo* RegistroMaleable::get(std::string nombre){
	/*Busqueda lineal de atributo con el nombre dado*/
	for(Ttamanio i=0;i<atributos.size();i++)
		if(atributos.at(i)->nombre()==nombre) return atributos.at(i);
	return NULL;
};
void RegistroMaleable::setbytes(std::streambuf& pbuffer){
	//TODO serializador
};
void RegistroMaleable::getbytes(std::streambuf& pbuffer){
	//TODO serializador
};
Ttamanio RegistroMaleable::cantidadAtributos(){return atributos.size();};
//Ttamanio RegistroMaleable::cantidadBytes(){return tamanioBuffer;};
