/*
 * SerializadorBloques.cpp
 *
 *  Created on: 03/10/2009
 *      Author: paulo
 */

#include "SerializadorBloques.h"

SerializadorBloques::SerializadorBloques() {
	// TODO Auto-generated constructor stub

}

SerializadorBloques::~SerializadorBloques() {
	// TODO Auto-generated destructor stub
}
void SerializadorBloques::serializar(Componente*componente,std::streambuf salida){
	Bloque* bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(componente))){
		salida.sputc(bloque->cantidadComponente());//TODO revisar si usar unsigned
		for(Ttamanio i=0;i<bloque->cantidadComponente();i++){
			bloque->apuntar(i);
			Componente* comp=bloque->get();
			salida.sputc(comp->cantidadBytes());
			comp->getbytes(salida);
			delete comp;
			//TODO trow exception
		}
	};
};
void SerializadorBloques::deserializar(Componente*componente,std::streambuf entrada){
	Bloque* bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(componente))){
		unsigned char nroComp=entrada.snextc();
		Componente* comp=bloque->get();
		for(Ttamanio i=0;i<nroComp;i++){
			comp->setbytes(entrada);
			if(i>=bloque->cantidadComponente())
				bloque->insertar(comp);
			else delete bloque->modificar(comp);
		}
		delete comp;
	};
};
