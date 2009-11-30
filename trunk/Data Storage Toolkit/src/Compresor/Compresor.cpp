/*
 * Compresor.cpp
 *
 *  Created on: 25/11/2009
 *      Author: paulo
 */

#include "Compresor.h"
#include <cmath>

void imprimir(unsigned num){
	for(int j=31;j>=0;j--){
		std::cout<<(((num &(0x1<<j))==0)?0:1);
	}
}
void imprimirLSBs(unsigned num,int n){
	for(int j=n-1;j>=0;j--){
		std::cout<<(((num &(0x1<<j))==0)?0:1);
	}
}

Compresor::Compresor(unsigned*array,unsigned tamanio): buffer(array,tamanio){

	techo=UINT_MAX;
	piso=0;
	U=0;
}
Compresor::~Compresor() {
}
inline char Compresor::overflow(){
	unsigned comparacion=techo^piso;
	char cont=0;
	while((comparacion&MSB) == 0 and cont<MAX_BITS){
		comparacion<<=1;
		cont++;
	}
	return cont;
}
inline char Compresor::underflow(){
	char cont=0;
	if((techo&MSB)!=0 and (piso&MSB)==0){
		unsigned bit_comparado=MSB>>1;
		while((techo & bit_comparado)==0 and (piso&bit_comparado)!=0  and cont<MAX_BITS-1){
			bit_comparado>>=1;
			cont++;
		}
	}
	return cont;
}
void Compresor::push(char chr){}
void Compresor::cerrar(char chr){}
void Compresor::abrirCierre(){}

void Compresor::descomprimir(unsigned * buffer,std::list<unsigned char>& descomprimido,int tamanioComprimido){
	//Calculo Padding
	//Primero busco el byte donde comienza el padding.
	int nro_byte;
	for(nro_byte=(tamanioComprimido-1);buffer[nro_byte]==0;nro_byte--);
	int bit_comparado=0X1;
	int nro_bit;
	for(nro_bit=0;(bit_comparado&buffer[nro_byte])==0;nro_bit++)bit_comparado<<=1;
	//Calculo cantidad de bits de compresion desde de los 32 iniciales
	unsigned bitRestantes = ((nro_byte*8)-32)-(8-nro_bit);

	//Establezco condiciones iniciales para descomprimir
	unsigned codigoAdescomprimir = buffer[0];
	int posBuffer = 1;
	unsigned siguiente = buffer[posBuffer];

	//Comienza descompresion
	while(bitRestantes>0){
		unsigned char emision = tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		tabla.incremtarOcurrencia(this->ultimoSimbolo,emision);//Se reestructura la tabla segun la ultima emision
		this->ultimoSimbolo = emision;
		descomprimido.push_back(emision);
		rearmarExtremos(piso,techo,buffer,posBuffer,siguiente,bitRestantes);
	}
}

void Compresor::setExtremos(){
	this->techo = pow(2,32)-1;
	this->piso = 0;
}

void Compresor::rearmarExtremos(unsigned &piso,unsigned &techo,unsigned*buffer,int &posBuffer,unsigned &siguiente,unsigned &bitRestantes){

}
