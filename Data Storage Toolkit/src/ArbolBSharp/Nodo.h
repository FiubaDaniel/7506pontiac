/*
 * Nodo.h
 *
 *  Created on: 30/09/2009
 *      Author: daniel
 */

#ifndef NODO_H_
#define NODO_H_
#include "Clave.h"
#include "ComparadorClave.h"
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
typedef unsigned int Referencia;
class Nodo {

public:
	     //establecer cantidad de elementos libres y cantidad max de elementos
	     Nodo(unsigned int numNodo,unsigned int cantElem,unsigned int nNivel);
	     Referencia getNumeroDeNodo();
	     Referencia getEspacioLibre();
	     Referencia getCatidadMaximaDeElementos();
         unsigned int getNumeroNivel();
         void setNumeroNivel(unsigned int num);
	     void setEspacioLibre(int esp);
	     void setNumeroDeNodo(unsigned int Nnodo);
	     virtual ~Nodo();
private:
	     unsigned int cantidadMaximaDeElementos,numeroDeNodo,cantidadDeElementosLibre,numeroNivel;
};
#endif /* NODO_H_ */
