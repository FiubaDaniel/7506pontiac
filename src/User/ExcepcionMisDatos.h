/*
 * ExcepcionMisDatos.h
 *
 *  Created on: 14/10/2009
 *      Author: ale
 */

#ifndef EXCEPCIONMISDATOS_H_
#define EXCEPCIONMISDATOS_H_

#include<iostream>

class ExcepcionMisDatos {

private:
	std::string mensaje;
public:
	ExcepcionMisDatos(std::string mensaje){
		this->mensaje = mensaje;
	}
	virtual ~ExcepcionMisDatos(){}
	std::string getMensaje(){
		return mensaje;
	}
};

#endif /* EXCEPCIONMISDATOS_H_ */
