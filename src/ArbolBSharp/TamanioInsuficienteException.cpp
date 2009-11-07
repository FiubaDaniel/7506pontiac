/*
 * TamanioInsuficienteException.cpp
 *
 *  Created on: 27/10/2009
 *      Author: daniel
 */

#include "TamanioInsuficienteException.h"

TamanioInsuficienteException::TamanioInsuficienteException() throw(){
	mensaje=" Error : Tamanio Insuficiente ";

}

TamanioInsuficienteException::~TamanioInsuficienteException() throw(){
	// TODO Auto-generated destructor stub
}

const char* TamanioInsuficienteException::what() const throw(){
   return mensaje.c_str();
}


