/*
 * main.cpp
 *
 *  Created on: 10/11/2009
 *      Author: paulo
 */

#include <iostream>
#include "Compresor/pruebas.h"
using namespace std;
#define MAXBITS 32
/*pone ceros desde leftoff una cant*/
#define RANGE(LEFTOFF,CANT) (~((~(UNOS>>(CANT)))>>(LEFTOFF)))
/*devuelve la cantidad de numeros copiados*/
int copiar(unsigned& dest,unsigned&fuente,int n ,int bit_w){
	fuente<<=MAXBITS-n;
	unsigned temp=fuente;
	if(n>=(MAXBITS-bit_w)){
		dest&=~(UNOS>>bit_w);
		temp>>=bit_w;
		dest|=temp;
		fuente<<=(MAXBITS-bit_w);
		n=(MAXBITS-bit_w);
	}else{
		dest&=RANGE(bit_w,n);
		temp>>=bit_w;
		temp&=~RANGE(bit_w,n);
		dest|=temp;
	}
	return n;
}

int main(){
/*	cout<<"Prueba1"<<endl;
	pruebacompresion1();
	cout<<"Prueba2"<<endl;
	pruebacompresion2();
		//
	cout<<"Prueba3"<<endl;
	pruebaCompresion3();*/
    cout<<"Prueba4"<<endl;

    //pruebaEstragiaCompresionAlmacenamiento();
    pruebaEstragiaCompresionAlmacenamiento1();
    //pruebaEstrategiaCompresionArbol();
	//pruebaCompresion5();
    return 0;
}
