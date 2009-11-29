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

Compresor::Compresor(unsigned*array,unsigned tamanio): contenedor(array,tamanio){

	techo=UINT_MAX;
	piso=0;
	U=0;
}
Compresor::~Compresor() {
	std::cout<<contenedor.size()<<std::endl;
	// TODO Auto-generated destructor stub
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
void Compresor::push(char chr){
	//TODO CalcularPisoyTecho((unsigned char)chr);
	/*OVERFLOW*/
	char cont=overflow();
	if(cont>0){
		unsigned relleno=(   (MSB  &   piso  )==  0 ) ? UINT_MAX :  0x0  ;
		emitir(piso>>(MAX_BITS-1),1);
		piso<<=1;
		techo=(techo<<1)|0x1;
		cont--;
		/*emisiones por underflow*/
		while(U>0){
			if(MAX_BITS > U)
				emitir(  relleno>>(MAX_BITS-U)  ,  U );
			else
				emitir(  relleno  ,  MAX_BITS  );
			U-=MAX_BITS;
		}
		U=0;
		/*resto del overflow*/
		if(cont>0){
			emitir(  piso  >>  (MAX_BITS-cont) , cont);
			piso<<=cont;
			techo=(techo<<cont)|(UINT_MAX>>(MAX_BITS-cont));
		}
	}
	/*UNDERFLOW*/
	cont=underflow();
	if(cont>0){
		techo=((techo<<(cont+1))>>1)| MSB |( UINT_MAX>>(MAX_BITS-cont));
		piso=(piso<<(cont+1))>>1;
		U+=cont;
	}

}
void Compresor::cerrar(char chr){
	//CalcularPisoyTecho((unsigned char)chr);
	ultimoSimbolo=(unsigned char)chr;
	unsigned relleno=((MSB  &   piso)==  0 ) ? UINT_MAX :  0x0  ;
	emitir(piso>>(MAX_BITS-1),1);
	/*emisiones por underflow*/
	int tempU=U;
	while(tempU>0){
		if(MAX_BITS > tempU)
			emitir(  relleno>>(MAX_BITS-tempU)  ,  tempU );
		else
			emitir(  relleno  ,  MAX_BITS  );
		tempU-=MAX_BITS;
	}
	emitir((piso<<1)>>1,MAX_BITS-1);
	char bits_libres_emision=MAX_BITS -contenedor.write_offset();
	if(bits_libres_emision>0){
		emitir(0x1<<(bits_libres_emision-1),bits_libres_emision);
	}else{
		emitir(MSB,MAX_BITS);
	}
}
void Compresor::abrirCierre(){
/*	int cont_bits=0;
	unsigned pos=contenedor.size()-1;
	unsigned temp;
	contenedor.seek_r(0,pos);
	do{
		contenedor.read_menos_significativos(temp,MAX_BITS);
		pos--;
	}while(temp==0);
	while((temp&(0x1<<cont_bits))==0)
		cont_bits++;
	cont_bits++;
	contenedor.seek_r(0,contenedor.size()-2);
	cont_bits=U+cont_bits;
	//controlar desborde
	while(cont_bits>MAX_BITS){
		buffer--;//controlar desborde de buffer
		cont_bits-=MAX_BITS;
		tamanioBuffer--;	longitud=techo-piso;
		longitud+=1;
		total++;
		ocurrencias[ultimoSimbolo]++;
	}
	bits_libres_emision=cont_bits;
	*buffer>>=bits_libres_emision;*/
	/*OVERFLOW*/
	char cont=overflow();
	if(cont>0){
		unsigned relleno=(   (MSB  &   piso  )==  0 ) ? UINT_MAX :  0x0  ;
		emitir(piso>>(MAX_BITS-1),1);
		piso<<=1;
		techo=(techo<<1)|0x1;
		cont--;
		/*emisiones por underflow*/
		while(U>0){
			if(MAX_BITS > U)
				emitir(  relleno>>(MAX_BITS-U)  ,  U );
			else
				emitir(  relleno  ,  MAX_BITS  );
			U-=MAX_BITS;
		}
		U=0;
		/*resto del overflow*/
		if(cont>0){
			emitir(  piso  >>  (MAX_BITS-cont) , cont);
			piso<<=cont;
			techo=(techo<<cont)|(UINT_MAX>>(MAX_BITS-cont));
		}
	}
	/*UNDERFLOW*/
	cont=underflow();
	if(cont>0){
		techo=((techo<<(cont+1))>>1)| MSB |( UINT_MAX>>(MAX_BITS-cont));
		piso=(piso<<(cont+1))>>1;
		U+=cont;
	}
}
