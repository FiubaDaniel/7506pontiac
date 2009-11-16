/*
 * RegistroMemoria.cpp
 *
 *  Created on: 06/10/2009
 *      Author: paulo
 */

#include "RegistroMemoria.h"
RegistroMemoria::RegistroMemoria(Registro*registro):RegistroMemoria::Registro(){
	for(Ttamanio i=0;i<registro->cantidadAtributos();i++){
		Atributo* aux=registro->get(i);
		this->atributos.push_back( aux->clonar() );
	}
}
RegistroMemoria::~RegistroMemoria(){}

Ttamanio RegistroMemoria::deserializar(std::streambuf&entrada){
	Ttamanio offset=sizeof(flags);
	entrada.sgetn((char*)&estado,offset);
	offset+=Registro::deserializar(entrada);
	return offset;
}
Ttamanio RegistroMemoria::serializar(std::streambuf&salida)const{
	Ttamanio offset=sizeof(flags);
	salida.sputn((char*)&estado,offset);
	offset+=Registro::serializar(salida);
	return offset;
}
Ttamanio RegistroMemoria::tamanioSerializado()const{
	return Registro::tamanioSerializado()+sizeof(char)+sizeof(Ttamanio);
}
Componente* RegistroMemoria::clonar()const{
	RegistroMemoria* clon=new RegistroMemoria(const_cast<RegistroMemoria*>(this));
	clon->estado=estado;
	return clon;
}
void RegistroMemoria::imprimir(std::ostream&salida)const{
	if(estaSucio())
		salida<<"RegistroSucio"<<std::endl;
	Registro::imprimir(salida);
}
