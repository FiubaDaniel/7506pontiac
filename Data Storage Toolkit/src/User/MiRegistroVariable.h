/*
 * MiRegistroVariable.h
 *
 *  Created on: 11/10/2009
 *      Author: ale
 */

#ifndef MIREGISTROVARIABLE_H_
#define MIREGISTROVARIABLE_H_


#include <iostream>

class MiRegistroVariable {

private:
	std::string miString;
	int miInt;
	int *miLista;
	int cantidadElementosLista;

public:
	MiRegistroVariable(std::string miStringID, int miInt, int *miLista, int cantidadElementosLista){
		this->miString = miStringID;
		this->miInt = miInt;
		this->miLista = miLista;
		this->cantidadElementosLista = cantidadElementosLista;
	}
	virtual ~MiRegistroVariable(){

	};
    int getCantidadElementosLista()
    {
        return cantidadElementosLista;
    }

    int getMiInt()
    {
        return miInt;
    }

    int *getMiLista()
    {
        return miLista;
    }

    std::string getMiStringID()
    {
        return miString;
    }


};

#endif /* MIREGISTROVARIABLE_H_ */
