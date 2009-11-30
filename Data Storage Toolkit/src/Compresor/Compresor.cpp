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
/************************MANEJO DE LA EMISIONES*****************************************************/
inline void Compresor::emitir_codigo(){
	/*OVERFLOW*/
	char cont=overflow();
	if(cont>0){
		unsigned relleno=(   (MSB  &   piso  )==  0 ) ? UNOS :  0x0  ;
		buffer.write(piso>>(MAX_BITS-1),1);
		piso<<=1;
		techo=(techo<<1)|0x1;
		cont--;
		/*emisiones por underflow*/
		while(U>0){
			if(MAX_BITS > U)
				buffer.write(  relleno>>(MAX_BITS-U)  ,  U );
			else
				buffer.write(  relleno  ,  MAX_BITS  );
			U-=MAX_BITS;
		}
		U=0;
		/*resto del overflow*/
		if(cont>0){
			buffer.write(  piso  >>  (MAX_BITS-cont) , cont);
			piso<<=cont;
			techo=(techo<<cont)|(UNOS>>(MAX_BITS-cont));
		}
	}
	/*UNDERFLOW*/
	cont=underflow();
	if(cont>0){
		techo=((techo<<(cont+1))>>1)| MSB |( UNOS>>(MAX_BITS-cont));
		piso=(piso<<(cont+1))>>1;
		U+=cont;
	}
}

inline void Compresor::cerrar_codigo(){
	/*emito primer bit del piso, luego el underflow , luego el resto del piso*/
	unsigned relleno=((MSB  &   piso)==  0 ) ? UNOS :  0x0  ;
	buffer.write(piso>>(MAX_BITS-1),1);
	int tempU=U;
	while(tempU>0){
		if(MAX_BITS > tempU)
			buffer.write(  relleno>>(MAX_BITS-tempU)  ,  tempU );
		else
			buffer.write(  relleno  ,  MAX_BITS  );
		tempU-=MAX_BITS;
	}
	buffer.write((piso<<1)>>1,MAX_BITS-1);

	/*padding*/
	char bits_libres_emision=MAX_BITS -buffer.tell_bits_offset_w();
	if(bits_libres_emision>0){
		buffer.write(0x1<<(bits_libres_emision-1),bits_libres_emision);
	}else{
		buffer.write(MSB,MAX_BITS);
	}
	/*relleno con 0*/
	while(buffer.tell_unsigned_write()<buffer.size()){
		buffer.fill(0x0);
	}
}

inline void Compresor::abrir_codigo(){
	unsigned pos=buffer.size();
	unsigned temp=0;
	while(temp==0){
		pos--;
		buffer.seek_r(0,pos);
		buffer.read(temp,MAX_BITS);
	};
	int cont_bits=0;
	while((temp&(0x1<<cont_bits))==0)
		cont_bits++;
	cont_bits++;
	pos--;
	cont_bits=U+cont_bits;
	while(cont_bits>MAX_BITS){
		cont_bits-=MAX_BITS;
		pos--;
	}
	buffer.seek_w(MAX_BITS-cont_bits,pos);
}
unsigned Compresor::comprimir(char*simbolos,unsigned tamanio){
	unsigned cant_emitidos=0;
	unsigned piso_anterior,techo_anterior;
	try{
		if(1/*tabla.vacia()*/){
			buffer.seek_w(0,0);
			tabla.obtenerExtremos(0,simbolo[cant_emitidos],piso,techo);
			piso_anterior=piso;
			techo_anterior=techo;
			emitir_codigo();
			cant_emitidos++;
		}
		while(cant_emitidos<tamanio){
			tabla.obtenerExtremos(ultimoSimbolo,simbolo[cant_emitidos],piso,techo);
			piso_anterior=piso;
			techo_anterior=techo;
			emitir_codigo();
			tabla.incremtarOcurrencia(ultimoSimbolo,simbolo[cant_emitidos]);
			ultimoSimbolo=simbolo[cant_emitidos];
			cant_emitidos++;
		}
	}catch(bitFileEOFException& e){
		piso=piso_anterior;
		techo=techo_anterior;
	};
	return cant_emitidos;
}
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

