/*
 * TamanioInsuficienteException.cpp
 *
 *  Created on: 27/10/2009
 *      Author: daniel
 */

#include "TamanioInsuficienteException.h"

TamanioInsuficienteException::TamanioInsuficienteException() {
	// TODO Auto-generated constructor stub

}

TamanioInsuficienteException::~TamanioInsuficienteException() {
	// TODO Auto-generated destructor stub
}

char* what(){
   char cadena[40]="Error: Tamanio Insuficiente";
   char* lanzamiento;
   strcpy(lanzamiento,cadena);
   return lanzamiento;
};


