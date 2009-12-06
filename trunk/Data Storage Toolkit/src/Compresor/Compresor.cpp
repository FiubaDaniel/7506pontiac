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

Compresor::Compresor(){
	salida=NULL;
	techo=UNOS;
	piso=0;
	U=0;
	ultimoSimbolo=0;
	bitRestantes=0;
}
Compresor::Compresor(unsigned*array,unsigned tamanio): buffer(array,tamanio){
	salida=NULL;
	techo=UNOS;
	piso=0;
	U=0;
	ultimoSimbolo=0;
	bitRestantes=0;
}
Compresor::~Compresor() {}

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
	if(salida){
		(*salida)<<cont+U<<"\t";
	}
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
	/*char bits_libres_emision=MAX_BITS -buffer.tell_bits_offset_w();
	if(bits_libres_emision>0){
		buffer.write(0x1<<(bits_libres_emision-1),bits_libres_emision);
	}else{
		buffer.write(MSB,MAX_BITS);
	}*/
	char bits_paddin=MAX_BITS -buffer.tell_bits_offset_w();
	buffer.fill(MSB);
	unsigned posicion=buffer.tell_unsigned_write();
	/*relleno con 0*/
	while(buffer.tell_unsigned_write()<buffer.size()){
		buffer.fill(0x0);
	}
	if(salida){
		(*salida)<< MAX_BITS+U <<" fin "<< (int)bits_paddin <<" paddin "<< (buffer.size()-posicion)*MAX_BITS <<" 0's";
	}
	// agrego U=0;
	U=0;
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

unsigned Compresor::comprimirPrimeros(unsigned char*simbolos,unsigned cantidad){

	unsigned cant_emitidos=0;
	unsigned piso_anterior,techo_anterior;
	unsigned pos=buffer.tell_unsigned_write();
	char offset=buffer.tell_bits_offset_w();
	try{
		if(salida){
			(*salida)<<"Caracteres en nro ASCCI"<<std::endl;
			(*salida)<<"#bits  Caracter"<<std::endl;
		}
		while(cant_emitidos<cantidad-1){
			piso_anterior=piso;
			techo_anterior=techo;
			tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
			emitir_codigo();
			if(tabla.esEscape()){
				if(salida){
					(*salida)<<" "<<"ESC"<<std::endl;
				}
				tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
				emitir_codigo();
			}
			if(salida){
				(*salida)<<" ";
				if(isalnum(simbolos[cant_emitidos]))
					(*salida)<<simbolos[cant_emitidos];
				else
					(*salida)<<"("<<(int)simbolos[cant_emitidos]<<")";
				(*salida)<<std::endl;
			}
			tabla.incremtarOcurrencia(ultimoSimbolo,simbolos[cant_emitidos]);
			//tabla.imprimir();std::cout<<std::endl;
			ultimoSimbolo=simbolos[cant_emitidos];
			cant_emitidos++;
		}
		cerrador=simbolos[cant_emitidos];
		unsigned bits_necesarios=MAX_BITS+U;
		if((buffer.size()-buffer.tell_unsigned_write())*MAX_BITS-buffer.tell_bits_offset_w() < bits_necesarios){
			throw bitFileEOFException();
		}
	}catch(bitFileEOFException e){
		buffer.seek_w(offset,pos);
		piso=piso_anterior;
		techo=techo_anterior;
	};
	return cant_emitidos+1;
}
bool Compresor::agregar(unsigned char*simbolos,unsigned cantidad){
	unsigned cant_emitidos=0,piso_anterior=piso,techo_anterior=techo;
	unsigned char ultimo_anterior=ultimoSimbolo, cerrador_anterior=cerrador;
	int U_anterior=U;
	unsigned pos=buffer.tell_unsigned_write();
	char offset=buffer.tell_bits_offset_w();
	//PPMC::TablaPPMC*clon=tabla.clonar();
	TablaOrden1*clon=tabla.clonar();
	try{
		//tabla.imprimir();std::cout<<std::endl;
		if(salida){
			(*salida)<<"Tratando de agregar:"<<std::endl;
			(*salida)<<"#bits  Caracter"<<std::endl;
		}
		/* emito el ultimo */
		tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
		emitir_codigo();
		if(tabla.esEscape()){
			if(salida){
				(*salida)<<" "<<"ESC"<<std::endl;
			}
			tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
			emitir_codigo();
		}
		if(salida){
			(*salida)<<" ";
			if(isalnum(cerrador))
				(*salida)<<cerrador;
			else
				(*salida)<<"("<<cerrador<<")";
			(*salida)<<std::endl;
		}
		tabla.incremtarOcurrencia(ultimoSimbolo,cerrador);
		//tabla.imprimir();std::cout<<std::endl;//todo aca
		ultimoSimbolo=cerrador;
		/**/
		while(cant_emitidos<cantidad-1){
			tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
			emitir_codigo();
			if(tabla.esEscape()){
				if(salida){
					(*salida)<<" ESC"<<std::endl;
				}
				tabla.obtenerExtremos(ultimoSimbolo,simbolos[cant_emitidos],piso,techo);
				emitir_codigo();
			}
			if(salida){
				(*salida)<<" ";
				if(isalnum(simbolos[cant_emitidos]))
					(*salida)<<simbolos[cant_emitidos];
				else
					(*salida)<<"("<<(int)simbolos[cant_emitidos]<<")";
				(*salida)<<std::endl;
			}
			tabla.incremtarOcurrencia(ultimoSimbolo,simbolos[cant_emitidos]);
			ultimoSimbolo=simbolos[cant_emitidos];
			cant_emitidos++;
		}
		cerrador=simbolos[cant_emitidos];

		unsigned bits_necesarios=MAX_BITS+U;
		if((buffer.size()-buffer.tell_unsigned_write())*MAX_BITS-buffer.tell_bits_offset_w() < bits_necesarios){
			throw bitFileEOFException();
		}
	}catch(bitFileEOFException e){
		if(salida){
			(*salida)<<"Demaciados Caracteres recuperando estado anterior"<<std::endl;
		}
		buffer.seek_w(offset,pos);
		ultimoSimbolo=ultimo_anterior;
		cerrador=cerrador_anterior;
		piso=piso_anterior;
		techo=techo_anterior;
		U=U_anterior;
		tabla.copiar(*clon);
		//tabla.imprimir();std::cout<<std::endl;
		delete clon;
		return false;
	}
	delete clon;
	return true;
}
void Compresor::cerrar(){
	tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
	/*nuevo*/
	if(tabla.esEscape()){
		emitir_codigo();
		if(salida){
			(*salida)<<" ESC"<<std::endl;
		}
		tabla.obtenerExtremos(ultimoSimbolo,cerrador,piso,techo);
	}
	/*fin nuevo*/

	cerrar_codigo();
	if(salida){
		(*salida)<<"\t ";
		if(isalnum(cerrador))
			(*salida)<<cerrador;
		else
			(*salida)<<(int)cerrador;
		(*salida)<<std::endl;
	}
	tabla.incremtarOcurrencia(ultimoSimbolo,cerrador);
	ultimoSimbolo=cerrador;
	std::cout<<" "<<int(ultimoSimbolo)<<std::endl;
}
void Compresor::reiniciarBuffer(){
	buffer.seek_w(0,0);
	piso=0;
	techo=UNOS;
	U=0;
}
void Compresor::setContinuacionCompresion(unsigned*buffer,unsigned tamanio){
	this->buffer.setBuffer(buffer,tamanio);
	this->buffer.seek_w(0,0);
	piso=0;
	techo=UNOS;
	U=0;
}
/*---------------------------------------Descompresion--------------------------------------------------------*/
void Compresor::descomprimir(unsigned * buffer,std::string& descomprimido,int tamanioComprimido){
	this->setExtremos();
	//Calculo Padding
	//Primero busco el byte donde comienza el padding.
	int nro_unsigned;
	for(nro_unsigned=(tamanioComprimido-1);buffer[nro_unsigned]==0;nro_unsigned--);
	/*if(nro_unsigned==(tamanioComprimido-2)){
		this->bitRestantes = ((nro_unsigned-1)*MAX_BITS)-1;
	}else{*/
	int bit_comparado=0X1;
	int nro_bit;
	for(nro_bit=0;(bit_comparado&buffer[nro_unsigned])==0;nro_bit++)bit_comparado<<=1;
	nro_bit++;//TODO nuevo paddin+1
	//Calculo cantidad de bits de compresion desde de los 32 iniciales
	this->bitRestantes = (nro_unsigned*MAX_BITS)-nro_bit;

	//Establezco condiciones iniciales para descomprimir
	unsigned codigoAdescomprimir = buffer[0];
	imprimirLSBs(codigoAdescomprimir,MAX_BITS);
	int posBuffer = 1;
	unsigned siguiente = buffer[posBuffer];
	unsigned char contadorDeBits=MAX_BITS;
	//Comienza descompresion
	int cant = 1;
	//while(cant<100){
	while(this->bitRestantes>=0){
		unsigned char emision = tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		if(tabla.esEscape()){
			rearmarExtremos(buffer,posBuffer,codigoAdescomprimir,siguiente,contadorDeBits);
			emision=tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		}
		tabla.incremtarOcurrencia(this->ultimoSimbolo,emision);
		//std::cout<<cant<<std::endl;
		this->ultimoSimbolo = emision;
		descomprimido.push_back(emision);
		rearmarExtremos(buffer,posBuffer,codigoAdescomprimir,siguiente,contadorDeBits);
		cant++;
	}
	cout<<"Esto es bit restantes "<<(int)this->bitRestantes<<endl;
	//TODO NUEVO if
	/*if(bitRestantes==0){
		unsigned char emision = tabla.calcularEmision(piso,techo,codigoAdescomprimir,this->ultimoSimbolo);
		tabla.incremtarOcurrencia(this->ultimoSimbolo,emision);
		ultimoSimbolo = emision;
		descomprimido.push_back(emision);
	}*/
	std::cout<<"  "<<int(ultimoSimbolo)<<std::endl;
}
/*-----------------------------------------------------------------------------------------------------------------*/
void Compresor::setExtremos(){
	this->techo = UNOS;
	this->piso = 0;
}

void Compresor::rearmarExtremos(unsigned*buffer,int &posBuffer,unsigned& codigo,unsigned &siguiente,unsigned char &contadorDeBits){
	unsigned char auxiliar;
	//Primero overflow
	unsigned char cont=overflow();
	if(cont>0){
		this->piso=(this->piso<<cont);
		this->techo=(this->techo<<cont)|(UNOS>>(MAX_BITS-cont));
		//Restructuro el codigo
		auxiliar = cont;
		if(cont>contadorDeBits){
			/*
			 * Si cont es mayor a la cantidad de bit que tiene siguiente se prosesan
			 * los bit que posea y se deja para procesar fuera aquellos que no se pudo seteando el
			 * siguiente.
			 */
			if(contadorDeBits>0){
				auxiliar = cont-contadorDeBits;
				//Acomodo codigo
				restructuracionOverflow(contadorDeBits,codigo,siguiente);
				//Setteo
			}
			posBuffer=posBuffer+1;
			siguiente=buffer[posBuffer];
			this->bitRestantes=this->bitRestantes-contadorDeBits;
			contadorDeBits=MAX_BITS;
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
		if(cont>contadorDeBits){
			if(contadorDeBits>0){
				auxiliar = cont-contadorDeBits;
				restructuracionUnderflow(contadorDeBits,codigo,siguiente);
			}
			posBuffer=posBuffer+1;
			siguiente=buffer[posBuffer];
			this->bitRestantes=this->bitRestantes-contadorDeBits;;
			contadorDeBits=MAX_BITS;
		}
		restructuracionUnderflow(auxiliar,codigo,siguiente);
		contadorDeBits=contadorDeBits-auxiliar;
		this->bitRestantes=this->bitRestantes-auxiliar;
	}
}

void Compresor::restructuracionOverflow(unsigned char cantidadIteraciones,unsigned& codigo,unsigned& siguiente){
	imprimirLSBs(siguiente>>(MAX_BITS-cantidadIteraciones),cantidadIteraciones);
	codigo = (siguiente>>(MAX_BITS-cantidadIteraciones))|(codigo<<cantidadIteraciones);
	siguiente<<=cantidadIteraciones;
}

void Compresor::restructuracionUnderflow(unsigned char cantidadIteraciones,unsigned& codigo,unsigned& siguiente){
	imprimirLSBs(siguiente>>(MAX_BITS-cantidadIteraciones),cantidadIteraciones);
	codigo=(codigo & MSB )|((codigo<<(cantidadIteraciones+1))>>1)|(siguiente>>(MAX_BITS-cantidadIteraciones));
	siguiente<<=cantidadIteraciones;
}

