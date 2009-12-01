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
	techo=UNOS;
	piso=0;
	U=0;
	ultimoSimbolo=0;
	bitRestantes=0;
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
unsigned Compresor::comprimirPrimeros(char*simbolos,unsigned cantidad){
	unsigned cant_emitidos=0;
	unsigned piso_anterior,techo_anterior;
	try{
		if(tabla.vacia() and cantidad>1){
			piso_anterior=piso;
			techo_anterior=techo;
			buffer.seek_w(0,0);
			tabla.obtenerExtremos(0,simbolos[0],piso,techo);
			emitir_codigo();
			ultimoSimbolo=simbolos[0];
			cant_emitidos++;
		}
		//-1 por q el simobolo q cierrar no se emite
		while(cant_emitidos<cantidad-1){
			piso_anterior=piso;
			techo_anterior=techo;
			tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
			emitir_codigo();
			tabla.incremtarOcurrencia(ultimoSimbolo,simbolos[cant_emitidos]);
			ultimoSimbolo=simbolos[cant_emitidos];
			cant_emitidos++;
		}
		cerrador=simbolos[cant_emitidos];
	}catch(bitFileEOFException& e){
		/*asumo que el codigo actual preserba el ultimoSimbolo*/
		piso=piso_anterior;
		techo=techo_anterior;
	};
	return cant_emitidos;
}
bool Compresor::agregar(char*simbolos,unsigned cantidad){
	unsigned cant_emitidos=0;
	unsigned piso_anterior,techo_anterior;
	int U_anterior=U;
	unsigned pos=buffer.tell_unsigned_write();
	char offset=buffer.tell_bits_offset_w();
	unsigned char ultimo=ultimoSimbolo;
	unsigned char cerrador_anterior=cerrador;
	try{
		/* emito el ultimo */
		piso_anterior=piso;
		techo_anterior=techo;
		tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
		emitir_codigo();
		tabla.incremtarOcurrencia(ultimoSimbolo,cerrador);
		ultimoSimbolo=cerrador;
		//-1 por q el simobolo q cierrar no se emite
		while(cant_emitidos!=cantidad-1){
			piso_anterior=piso;
			techo_anterior=techo;
			tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
			emitir_codigo();
			tabla.incremtarOcurrencia(ultimoSimbolo,simbolos[cant_emitidos]);
			ultimoSimbolo=simbolos[cant_emitidos];
			cant_emitidos++;
		}
		cerrador=simbolos[cant_emitidos];
	}catch(bitFileEOFException& e){
		buffer.seek_w(offset,pos);
		ultimoSimbolo=ultimo;
		cerrador=cerrador_anterior;
		piso=piso_anterior;
		techo=techo_anterior;
		U=U_anterior;
		if(cant_emitidos!=0){
			tabla.decremetarOcurrencia(ultimo,simbolos[0]);
			for(unsigned i=0;i<cant_emitidos-1;i++){
				tabla.decremetarOcurrencia(simbolos[i],simbolos[i+1]);
			}
		}
		return false;
	}
	return true;
}
void Compresor::cerrar(){
	tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
	cerrar_codigo();
	tabla.incremtarOcurrencia(ultimoSimbolo,cerrador);
	ultimoSimbolo=cerrador;
}
void Compresor::setContinuacionCompresion(unsigned*buffer,unsigned tamanio){
	this->buffer.setBuffer(buffer,tamanio);
	this->buffer.seek_r(0,0);
}
/*---------------------------------------Descompresion--------------------------------------------------------*/
void Compresor::descomprimir(unsigned * buffer,std::string& descomprimido,int tamanioComprimido){
	//Calculo Padding
	//Primero busco el byte donde comienza el padding.
	int nro_byte;
	for(nro_byte=(tamanioComprimido-1);buffer[nro_byte]==0;nro_byte--);
	int bit_comparado=0X1;
	int nro_bit;
	for(nro_bit=0;(bit_comparado&buffer[nro_byte])==0;nro_bit++)bit_comparado<<=1;
	//Calculo cantidad de bits de compresion desde de los 32 iniciales
	this->bitRestantes = ((nro_byte*8)-32)-(8-nro_bit);

	//Establezco condiciones iniciales para descomprimir
	unsigned codigoAdescomprimir = buffer[0];
	int posBuffer = 1;
	unsigned siguiente = buffer[posBuffer];
	unsigned contadorDeBits=32;
	//Comienza descompresion
	while(this->bitRestantes>0){
		unsigned char emision = tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		tabla.incremtarOcurrencia(this->ultimoSimbolo,emision);//Se reestructura la tabla segun la ultima emision
		this->ultimoSimbolo = emision;
		descomprimido.push_back(emision);
		rearmarExtremos(buffer,posBuffer,codigoAdescomprimir,siguiente,contadorDeBits);
	}
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
	this->bitRestantes = ((nro_byte*8)-32)-(8-nro_bit);

	//Establezco condiciones iniciales para descomprimir
	unsigned codigoAdescomprimir = buffer[0];
	int posBuffer = 1;
	unsigned siguiente = buffer[posBuffer];
	unsigned contadorDeBits=32;
	//Comienza descompresion
	while(this->bitRestantes>0){
		unsigned char emision = tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		tabla.incremtarOcurrencia(this->ultimoSimbolo,emision);//Se reestructura la tabla segun la ultima emision
		this->ultimoSimbolo = emision;
		descomprimido.push_back(emision);
		rearmarExtremos(buffer,posBuffer,codigoAdescomprimir,siguiente,contadorDeBits);
	}
}

void Compresor::setExtremos(){
	this->techo = pow(2,32)-1;
	this->piso = 0;
}

void Compresor::rearmarExtremos(unsigned*buffer,int &posBuffer,unsigned& codigo,unsigned &siguiente,unsigned &contadorDeBits){
	unsigned auxiliar;
	//Primero overflow
	unsigned char cont=overflow();
	if(cont>0){
		this->piso=(this->piso<<cont);
		this->techo=(this->techo<<cont)|(UNOS>>(MAX_BITS-cont));
		//Restructuro el codigo
		auxiliar = cont;
		if(cont>=contadorDeBits){
			/*
			 * Si cont es mayor a la cantidad de bit que tiene siguiente se prosesan
			 * los bit que posea y se deja para procesar fuera aquellos que no se pudo seteando el
			 * siguiente.
			 */
			auxiliar = cont-contadorDeBits;
			//Acomodo codigo
			restructuracionOverflow(contadorDeBits,codigo,siguiente);
			//Setteo
			contadorDeBits=8;
			siguiente=buffer[posBuffer];
			posBuffer=posBuffer+1;
			this->bitRestantes=this->bitRestantes-auxiliar;
		}
		//Acomodo codigo usando auxiliar
		restructuracionOverflow(auxiliar,codigo,siguiente);
		//Setteo
		contadorDeBits=contadorDeBits-auxiliar;
		this->bitRestantes=this->bitRestantes-auxiliar;
	}
	//Luego underflow (despues de overflow puede haber underflow
	cont= underflow();
	if(cont>0){
		this->piso=(this->piso&MSB)|((this->piso<<(cont+1))>>1);
		this->techo=(this->techo&MSB)|((this->techo<<(cont+1))>>1)|(UNOS>>(MAX_BITS-cont));
		//Reestructuro codigo
		auxiliar=cont;
		if(cont>=contadorDeBits){
			auxiliar = cont-contadorDeBits;
			restructuracionUnderflow(contadorDeBits,codigo,siguiente);
			contadorDeBits=8;
			siguiente=buffer[posBuffer];
			posBuffer=posBuffer+1;
			this->bitRestantes=this->bitRestantes-auxiliar;
		}
		restructuracionUnderflow(auxiliar,codigo,siguiente);
		contadorDeBits=contadorDeBits-auxiliar;
		this->bitRestantes=this->bitRestantes-auxiliar;
	}
}

void Compresor::restructuracionOverflow(unsigned cantidadIteraciones,unsigned& codigo,unsigned& siguiente){
	while(cantidadIteraciones>0){
		codigo = ((siguiente|MSB)>>(MAX_BITS-1))|(codigo<<1);
		siguiente<<=1;
		cantidadIteraciones-=1;
	}
}

void Compresor::restructuracionUnderflow(unsigned cantidadIteraciones,unsigned& codigo,unsigned& siguiente){
	while(cantidadIteraciones>0){
		/*
		 * 1-) Rescato el primero de siguiente, para dejarlo como ultimo
		 * 2-) Rescato el primero de codigo y elimino el segundo.
		 * 3-) Meto al final de codigo el primero de siguiente.
		 */
			codigo = ((siguiente|MSB)>>(MAX_BITS-1))|((codigo|MSB)|((codigo<<2)>>1));
			siguiente<<=1;
			cantidadIteraciones-=1;
		}
}

