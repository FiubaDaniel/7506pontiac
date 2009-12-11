/*
 * Compresor.h
 *
 *  Created on: 25/11/2009
 *      Author: paulo
 */

#ifndef COMPRESOR_H_
#define COMPRESOR_H_
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include "bitFile.h"
#include "TablaOrden1.h"
#include "TablaPPMC.h"
#include "TablaOrden0.h"

void imprimir(unsigned num);

class Compresor {
	std::map<unsigned,unsigned> estadistica;
	std::ostream* salida;
	/*MANEJO DE UNDERFLOW Y OVERFLOW*/
	static const char MAX_BITS=sizeof(unsigned)*8;
	static const unsigned MSB=0x80000000;
	unsigned piso;
	unsigned techo;
	/*para saber el contexto de las nuevas emisiones*/
	unsigned char ultimoSimbolo;
	/*usado para cerrar*/
	unsigned char cerrador;
	int bitRestantes;
	int U;
	unsigned caracteres;
	char overflow();
	char underflow();
	void actualizarEstadistica(std::list<unsigned> lista){
		for(std::list<unsigned>::iterator it_list=lista.begin();it_list!=lista.end();it_list++){
			std::map<unsigned,unsigned>::iterator it=estadistica.find(*it_list);
			if(it!=estadistica.end()){
				it->second++;
			}else{
				estadistica[*it_list]=1;
			}
		}
	}

	/************************MANEJO DE LA EMISIONES*****************************************************/
	bitFile buffer;
	/* Para el <techo> y <piso> actuales emite el codigo correspondiente al <contener>,
	 * actualiza el nuevo <techo> y <piso>.y actualiza el contador <U> de  underflow
	 */
	void emitir_codigo(std::list<unsigned> *lista);
	/* para el <techo> , <piso> y el contador <U> de underflow actuales,emite piso cerrando el codigo.
	 * es decir underflow+piso+padding
	 */
	void cerrar_codigo(std::list<unsigned> *lista);
	/* dado el codido contenido en <contendor> y el contador <U> de undeflow,
	 * deja el codigo en un estado tal que, la proxima emision sobreescribe a la emision
	 * que cerro el codigo previamente.
	 */
	void abrir_codigo();
	//TablaOrden0 tabla;
	TablaOrden1 tabla;
	//PPMC::TablaPPMC tabla;

public:
	Compresor();
	Compresor(unsigned*array,unsigned tamanio);
	/* dado el array de <simbolos> trata de comprimir los <cantidad> simbolos del array,
	 * devuelve el numero de simbolos efectivamente comprimidos
	 */
	unsigned comprimirPrimeros(unsigned char*simbolos,unsigned cantidad);
	/*
	 * intenta agregar <cantidad> de <simbolos> al codigo actual, en caso de que
	 * desborde la capacidad, el codigo agregado es desechado.
	 * pre: se usado comprimir previamente, almenos una vez.
	 */
	bool agregar(unsigned char*simbolos,unsigned cantidad);
	/*
	 * Para el buffer utilizado cierra el codigo.
	 */
	void cerrar();
	/*
	 * reinicia el buffer usado, los proximos codigos generados se grabaran
	 * al principio del buffer
	 */
	void reiniciarBuffer();
	/*
	 * setea el buffer usado, los proximos codigos generados se grabaran
	 * al principio del buffer seteado
	 */
	void setContinuacionCompresion(unsigned*buffer,unsigned tamanio);
	/*
	 * Setea la salida para imprimir las estadisticas.
	 * <salida> esta lista para imprimir caracteres tipo texto, y es administrada exteriormente;
	 * devuelve la salida anterior, NULL si no habia una salida seteada previamente
	 */
	std::ostream* setSalida(std::ostream*salida){
		std::ostream* anterior=this->salida;
		this->salida=salida;
		return anterior;
	};
	/*
	 * Deja de usar la salida actual y la devuelve.
	 */
	std::ostream* quitarSalida(){
		std::ostream* anterior=this->salida;
		this->salida=NULL;
		return anterior;
	};
	unsigned* getbufferComprimido(){
		return buffer.getBuffer();
	};
	unsigned tamanioBufferComprimido(){
		return buffer.size();
	};
	void descomprimir(unsigned * buffer,std::string& descomprimido,unsigned tamanioComprimido);
	void setExtremos();
	void setCaracteres(unsigned caracteres);
	virtual ~Compresor();
	void imprimirEstadistica(){
		if(salida){
			(*salida)<<"   cantidad     |    veces que se "<<endl;
			(*salida)<<"bits emitidos   | emitio esa cantidad"<<endl;
			unsigned total=0;
			for(std::map<unsigned,unsigned>::iterator it=estadistica.begin();it!=estadistica.end();it++){
				(*salida)<<it->first<<" \t \t  "<<it->second<<endl;
				total+=it->second*it->first;
			}
			(*salida)<<"Total:"<<total<<"bits == "<<total/8<<" bytes "<<endl;
		}
		estadistica.clear();
	}

private:
	/*-----------------Metodos privados de Descompresion------------------------------------------------------------*/
	/*
	 * Luego de una emision se re-estructura el piso, el techo al igual que el codigo usando el siguiente para obtener
	 * bits necesarios debido a posibles overflow o underflow.
	 */
	void rearmarExtremos(unsigned*buffer,int &posBuffer,unsigned& codigo,unsigned &siguiente,unsigned char &contadorDeBits);
	/*
	 * Permite llevar a cabo la re-estructuracion del codigo debido a un overflow.
	 */
	void restructuracionOverflow(unsigned char cantidadIteraciones,unsigned& codigo,unsigned& siguiente);
	/*
	 * Permite llevar a cabo la re-estructuracion del codigo debido a un underflow.
	 */
	void restructuracionUnderflow(unsigned char cantidadIteraciones,unsigned& codigo,unsigned& siguiente);
};

#endif /* COMPRESOR_H_ */
