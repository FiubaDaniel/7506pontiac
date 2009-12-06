/*
 * BitContainer.cpp
 *
 *  Created on: 28/11/2009
 *      Author: paulo
 */
#include "bitFile.h"

#define MAXBITS 32
#define UNOS (~unsigned(0x0))
void imprimir(unsigned num,char n){
	for(int j=n-1;j>=0;j--){
		std::cout<<(((num &(0x1<<j))==0)?0:1);
	}
	std::cout<<flush;
}


#define shift_to_left(a,cant) (((cant)<MAXBITS)?((a)<<(cant)):0x0)
#define shift_to_right(a,cant) (((cant)<MAXBITS)?((a)>>(cant)):0x0)

/*pone en uno los bits mas significativos desde el off en una canti de n
 * 0<=off<=31
 * 0<=n<=32
 */
unsigned rango_unos(int off,int n){
	return shift_to_right((~shift_to_right(UNOS,n)),off);
}
/*selecciona los n bits mas significativos de fuente,poniendolos a partir del offset indicado*/
unsigned seleccionar_bits_mas(unsigned fuente,int off ,int n){
	return (fuente>>off)&rango_unos(off,n);
}
/*pone en cero n bits de la fuente desde off el resto de los bits permancen iguales*/
unsigned apagar_bits(unsigned fuente,int off ,int n){
	return fuente&~rango_unos(off,n);
}
/*filtra n bits de fuente desde el off*/
unsigned filtrar_bits(unsigned fuente,int off ,int n){
	return fuente&rango_unos(off,n);
}
/*escribe n bits mas significativos de fuente, en dest a partir del offset*/
unsigned reemplazar_bits(unsigned dest,unsigned fuente,int off_dest,int n){
	return apagar_bits(dest,off_dest,n)|seleccionar_bits_mas(fuente,off_dest,n);
}
bitFile::bitFile(unsigned*buffer,unsigned tamanio){
	this->buffer=buffer;
	this->tamanio=tamanio;
	write_posicion=0;
	bit_w=0;
	read_posicion=0;
	bit_r=0;
};
bitFile::bitFile(){
	buffer=NULL;
	tamanio=0;
	write_posicion=0;
	bit_w=0;
	read_posicion=0;
	bit_r=0;
};
bitFile::~bitFile() {}
void bitFile::write(unsigned fuente,char cantidad)throw (bitFileEOFException){
	if(write_posicion>=tamanio){
		throw bitFileEOFException();
	}
	imprimir(fuente,cantidad);
	fuente<<=(MAX_BITS-cantidad);
	while(cantidad>0 and write_posicion<tamanio){
		int libres=MAX_BITS-bit_w;
		if( cantidad >= libres ){
			buffer[write_posicion]=reemplazar_bits(buffer[write_posicion],fuente,bit_w,libres);
			write_posicion++;
			bit_w=0;
			fuente<<=libres;
			cantidad-=libres;
		}else{
			buffer[write_posicion]=reemplazar_bits(buffer[write_posicion],fuente,bit_w,cantidad);
			fuente<<=cantidad;
			bit_w+=cantidad;
			cantidad=0;
		}
	}
	if(bit_w==MAX_BITS){
		bit_w=0;
		write_posicion++;
	}
}
void bitFile::fill(unsigned fuente){
	int cantidad=MAX_BITS;
	imprimir(fuente);
	while(cantidad>0 and write_posicion<tamanio){
		int libres=MAX_BITS-bit_w;
		if( cantidad >= libres ){

			buffer[write_posicion]=reemplazar_bits(buffer[write_posicion],fuente,bit_w,libres);

			write_posicion++;
			bit_w=0;
			fuente<<=libres;
			cantidad-=libres;
		}else{

			buffer[write_posicion]=reemplazar_bits(buffer[write_posicion],fuente,bit_w,cantidad);

			bit_w+=cantidad;
			cantidad=0;
		}
	}
}
void bitFile::read(unsigned &fuente,char cantidad)throw (bitFileEOFException){
	if(read_posicion>=tamanio){
		throw bitFileEOFException();
	}
	char bit_f_w=MAX_BITS-cantidad;

	while(bit_f_w < MAX_BITS and read_posicion<tamanio){
		int restantes=MAX_BITS-bit_r;
		if( cantidad >= restantes ){
			fuente=reemplazar_bits(fuente,buffer[read_posicion]<<bit_r,bit_f_w,restantes);
			read_posicion++;
			bit_r=0;
			bit_f_w+=restantes;
			cantidad-=restantes;
		}else{
			fuente=reemplazar_bits(fuente,buffer[read_posicion]<<bit_r,bit_f_w,cantidad);
			bit_r+=cantidad;
			bit_f_w=MAX_BITS;
			cantidad=0;
		}
	}
	if(bit_r==MAX_BITS){
		bit_r=0;
		read_posicion++;
	}
}
void bitFile::seek_r(char offset,unsigned posicion){
	read_posicion=posicion;
	bit_r=offset;
}
void bitFile::seek_w(char offset,unsigned posicion){
	write_posicion=posicion;
	bit_w=offset;
}
void bitFile::jump_bits_r(char offset){
	bit_r+=offset;
	if(bit_r>MAX_BITS){
		bit_r=bit_r%MAX_BITS;
		read_posicion+=offset/MAX_BITS;
	}
}
void bitFile::jump_bits_w(char offset){
	bit_w+=offset;
	if(bit_w>MAX_BITS){
		bit_w=bit_w%MAX_BITS;
		write_posicion+=offset/MAX_BITS;
	}
}
unsigned bitFile::tell_unsigned_r(){
	return read_posicion;
}
unsigned bitFile::tell_unsigned_write(){
	return write_posicion;
}
/*tamanio del contenedor en bytes*/
unsigned bitFile::size(){
	return tamanio;
}
/*cantidad de bits libres en el ultimo unsigned usado*/
char bitFile::tell_bits_offset_r(){
	return bit_r;
}
char bitFile::tell_bits_offset_w(){
	return bit_w;
}

void bitFile::setBuffer(unsigned*buffer,unsigned tamanio){
	this->tamanio=tamanio;
	this->buffer=buffer;
}
unsigned* bitFile::getBuffer(){
	return buffer;
}
