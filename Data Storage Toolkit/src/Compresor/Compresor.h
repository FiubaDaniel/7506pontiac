/*
 * Compresor.h
 *
 *  Created on: 25/11/2009
 *      Author: paulo
 */

#ifndef COMPRESOR_H_
#define COMPRESOR_H_
#include <iostream>
#include "BitContainer.h"
void imprimir(unsigned num);
#define UINT_MAX 0xFFFFFFFF
class Compresor {
	/*MANEJO DE UNDERFLOW Y OVERFLOW*/
	static const char MAX_BITS=sizeof(unsigned)*8;
	static const unsigned MSB=0x80000000;
	unsigned piso;
	unsigned techo;
	unsigned char ultimoSimbolo;
	int U;
	char overflow();
	char underflow();
	BitContainer contenedor;
	void emitir(unsigned fuente,char cantidad){}
public:
	Compresor(unsigned*array,unsigned tamanio);
	void push(char chr);
	void cerrar(char chr);
	void abrirCierre();
	virtual ~Compresor();
};

#endif /* COMPRESOR_H_ */
