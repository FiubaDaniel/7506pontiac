/*
 * MiRegistroTexto.h
 *
 *  Created on: 14/10/2009
 *      Author: ale
 */

#ifndef MIREGISTROTEXTO_H_
#define MIREGISTROTEXTO_H_

#include<iostream>

class MiRegistroTexto {

private:
	std::string texto;
public:
	MiRegistroTexto(std::string texto){
		this->texto = texto;
	}
	virtual ~MiRegistroTexto(){}
	std::string getTexto(){
		return texto;
	}
};

#endif /* MIREGISTROTEXTO_H_ */
