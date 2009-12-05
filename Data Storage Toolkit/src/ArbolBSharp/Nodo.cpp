/*
 * Nodo.cpp
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#include "Nodo.h"

Nodo::Nodo(unsigned int cantElem,unsigned int nNivel,ComparadorClave* comp) {

	cantidadMaximaDeElementos = cantElem;
	cantidadDeElementosLibre = cantElem;
	numeroNivel = nNivel;
	comparador = comp;
}
std::list<ElementoNodo*>* Nodo::getListaElementos(){
	return &listaElementos;
}
unsigned int Nodo::getEspacioLibre(){
	return cantidadDeElementosLibre;
}
unsigned int Nodo::getCatidadMaximaDeElementos(){
	return cantidadMaximaDeElementos;
}
void Nodo::setEspacioLibre(int esp){
	cantidadDeElementosLibre = esp;
}
unsigned int Nodo::getNumeroNivel(){
	return numeroNivel;
}
void Nodo::setNumeroNivel(unsigned int num){
	numeroNivel = num;
}
int Nodo::Eliminar(const Clave* clave){
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	bool encontrado = false;
	int retorno = 1;
	bool noExiste = false;
	while(!encontrado && it!=listaElementos.end()){
		ElementoNodo* elemento = *it;
		if(comparador->Comparar(elemento->getClave(),clave)==0){
			listaElementos.erase(it);
			delete elemento;
			cantidadDeElementosLibre = cantidadDeElementosLibre+1;
			if(listaElementos.size()<(((cantidadMaximaDeElementos)*2)/3)){
				retorno = 2;
			}
			encontrado = true;
		}else if(comparador->Comparar(elemento->getClave(),clave)>0){
			retorno = 0;
			encontrado = true;
			noExiste = true;
		}
		++it;
	}
	if(noExiste || !encontrado){retorno = 0;}
	return retorno;
}

Nodo::~Nodo() {
	std::list<ElementoNodo*>::iterator it = listaElementos.begin();
	while(it!=listaElementos.end()){
		ElementoNodo* elemento = *it;
		delete elemento;
		++it;
	}
	listaElementos.clear();
}
