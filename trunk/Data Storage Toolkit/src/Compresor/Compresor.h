/*
 * Compresor.h
 *
 *  Created on: 25/11/2009
 *      Author: paulo
 */

#ifndef COMPRESOR_H_
#define COMPRESOR_H_
#include <iostream>
#include "bitFile.h"
#include <math.h>
#include "TablaDeProbabilidad.h"
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
	bitFile buffer;
	TablaDeProbabilidad tabla;
	void emitir(unsigned fuente,char cantidad){}
public:
	Compresor(unsigned*array,unsigned tamanio);
	void push(char chr);
	void cerrar(char chr);
	void abrirCierre();
	void descomprimir(unsigned * buffer,std::list<unsigned char>& descomprimido,int tamanioComprimido);
	void setExtremos();
	void rearmarExtremos(unsigned &piso,unsigned &techo,unsigned*buffer,int &posBuffer,unsigned &siguiente,unsigned &bitRestantes);
	virtual ~Compresor();
};

#endif /* COMPRESOR_H_ */
