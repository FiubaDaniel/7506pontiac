/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */

#include "EABloques.h"

EABloques::EABloques(Ttamanio tamanioBloque) {
	tamanio=tamanioBloque;
	buffertransferencia=new char[tamanioBloque];
};

EABloques::~EABloques() {
	delete[] buffertransferencia;
};
void EABloques::posicionarCompuesto(size_t nroCompuesto){
	corriente=nroCompuesto;
};
void EABloques::escribir(Almacenamiento* almacen,Compuesto*compuesto){
//	Bloque* bloque=(Bloque*)compuesto;
/*	std::stringstream stream;
	stream.rdbuf()->pubsetbuf(buffertransferencia,tamanio);
	bloque->serializar(stream);
	almacen->posicionarByte(corriente*bloque->tamanioSerializado());
	almacen->escribir(stream.rdbuf(),bloque->tamanioSerializado());*/
};
void EABloques::leer(Almacenamiento* almacen,Compuesto*compuesto){
	//Bloque* bloque=(Bloque*)compuesto;
/*	std::stringstream stream;
	almacen->posicionarByte(corriente*bloque->tamanioSerializado());
	almacen->leer(buffertransferencia,tamanio);
	stream.rdbuf()->pubsetbuf(buffertransferencia,tamanio);
	bloque->serializar(stream);*/
};
