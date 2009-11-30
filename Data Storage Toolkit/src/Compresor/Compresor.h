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
#include <cmath>
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
	/*para saber el contexto de las nuevas emisiones*/
	unsigned char ultimoSimbolo;
	/*usado para cerrar*/
	unsigned char cerrador;
	unsigned bitRestantes;
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
	/* dado el array de <simbolos> trata de comprimir los <cantidad> simbolos del array,
	 * devuelve el numero de simbolos efectivamente comprimidos
	 */
	unsigned comprimirPrimeros(char*simbolos,unsigned cantidad);
	/*
	 * intenta agregar <cantidad> de <simbolos> al codigo actual, en caso de que
	 * desborde la capacidad, el codigo agregado es desechado.
	 * pre: se usado comprimir previamente, almenos una vez.
	 */
	bool agregar(char*simbolos,unsigned cantidad);
	/*
	 * Para el buffer utilizado cierra el codigo.
	 */
	void cerrar();
	/*
	 *
	 */
	void setContinuacionCompresion(unsigned*buffer,unsigned tamanio);
	unsigned* getbufferComprimido(){
		return buffer.getBuffer();
	};
	unsigned tamanioBufferComprimido(){
		return buffer.size();
	};
	void descomprimir(unsigned * buffer,std::list<unsigned char>& descomprimido,int tamanioComprimido);
	void setExtremos();
	virtual ~Compresor();

private:
	/*-----------------Metodos privados de Descompresion------------------------------------------------------------*/
	/*
	 * Luego de una emision se re-estructura el piso, el techo al igual que el codigo usando el siguiente para obtener
	 * bits necesarios debido a posibles overflow o underflow.
	 */
	void rearmarExtremos(unsigned*buffer,int &posBuffer,unsigned& codigo,unsigned &siguiente,unsigned &contadorDeBits);
	/*
	 * Permite llevar a cabo la re-estructuracion del codigo debido a un overflow.
	 */
	void restructuracionOverflow(unsigned cantidadIteraciones,unsigned& codigo,unsigned& siguiente);
	/*
	 * Permite llevar a cabo la re-estructuracion del codigo debido a un underflow.
	 */
	void restructuracionUnderflow(unsigned cantidadIteraciones,unsigned& codigo,unsigned& siguiente);
};

#endif /* COMPRESOR_H_ */
