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
#define UINT_MAX (~0x0)

#ifndef UNOS
	#define UNOS (~0x0)
#endif

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
	//void emitir();
	/************************MANEJO DE LA EMISIONES*****************************************************/
	bitFile buffer;
	/* Para el <techo> y <piso> actuales emite el codigo correspondiente al <contener>,
	 * actualiza el nuevo <techo> y <piso>.y actualiza el contador <U> de  underflow
	 */
	void emitir_codigo();
	/* para el <techo> , <piso> y el contador <U> de underflow actuales,emite piso cerrando el codigo.
	 * es decir underflow+piso+padding
	 */
	void cerrar_codigo();
	/* dado el codido contenido en <contendor> y el contador <U> de undeflow,
	 * deja el codigo en un estado tal que, la proxima emision sobreescribe a la emision
	 * que cerro el codigo previamente.
	 */
	void abrir_codigo();

	TablaDeProbabilidad tabla;
public:
	Compresor(unsigned*array,unsigned tamanio);
	/*void push(char chr);
	void cerrar(char chr);
	void abrirCierre();*/
	void descomprimir(unsigned * buffer,std::list<unsigned char>& descomprimido,int tamanioComprimido);
	void setExtremos();
	void rearmarExtremos(unsigned &piso,unsigned &techo,unsigned*buffer,int &posBuffer,unsigned &siguiente,unsigned &bitRestantes);
	virtual ~Compresor();
};

#endif /* COMPRESOR_H_ */
