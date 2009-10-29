/*
 * Nodo.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#include "Nodo.h"

Nodo::Nodo(unsigned int numNodo,unsigned int cantElem,unsigned int nNivel) {

	cantidadMaximaDeElementos = cantElem;
	numeroDeNodo = numNodo;
	cantidadDeElementosLibre = cantElem;
	numeroNivel = nNivel;
};

Referencia Nodo::getNumeroDeNodo(){
	return numeroDeNodo;
};

Referencia Nodo::getEspacioLibre(){
	return cantidadDeElementosLibre;
};
Referencia Nodo::getCatidadMaximaDeElementos(){
	return cantidadMaximaDeElementos;
};
void Nodo::setEspacioLibre(int esp){
	cantidadDeElementosLibre = esp;
};
unsigned int Nodo::getNumeroNivel(){
	return numeroNivel;
};
void Nodo::setNumeroNivel(unsigned int num){
	numeroNivel = num;
};
void Nodo::setNumeroDeNodo(unsigned int Nnodo){
	numeroDeNodo = Nnodo;
};
Nodo::~Nodo() {	};
