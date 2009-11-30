/*
 * BitContainer.cpp
 *
 *  Created on: 28/11/2009
 *      Author: paulo
 */

#include "bitFile.h"

bitFile::bitFile(unsigned*buffer,unsigned tamanio){
	this->buffer=buffer;
	this->tamanio=tamanio;
	write_posicion=0;
	bit_write_offset=0;
	read_posicion=0;
	bit_read_offset=0;
};

bitFile::~bitFile() {}
void bitFile::write(unsigned fuente,char cantidad)throw (bitFileEOFException){
	int bits_restantes=(tamanio-write_posicion)*MAX_BITS-bit_write_offset;
	//unsigned *p=&buffer[write_posicion];
	if(bits_restantes > cantidad){
		fuente<<=MAX_BITS-cantidad;
		bits_restantes=MAX_BITS-bit_write_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				buffer[write_posicion]&=~(UNOS>>bit_write_offset);
				buffer[write_posicion]|=fuente>>bit_write_offset;
				fuente<<=bits_restantes;
				cantidad-=bits_restantes;
			}
			write_posicion++;
			bit_write_offset=0;
			//p=&buffer[write_posicion];
		}
		if(cantidad>0){
			unsigned aux=(UNOS<<(MAX_BITS-cantidad))>>bit_write_offset;
			buffer[write_posicion]&=~aux;
			buffer[write_posicion]|=fuente>>bit_write_offset;
			bit_write_offset+=cantidad;
		}
	}
}
void bitFile::read(unsigned &fuente,char cantidad)throw (bitFileEOFException){
	int bits_restantes=(tamanio-read_posicion)*MAX_BITS-bit_read_offset;
	if(bits_restantes>=cantidad){
		fuente&=~(UNOS>>(MAX_BITS-cantidad));
		//unsigned * p=&buffer[read_posicion];
		bits_restantes=MAX_BITS-bit_read_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				fuente|=(buffer[read_posicion]<<bit_read_offset)>>(MAX_BITS-cantidad);
				cantidad-=bits_restantes;
			}
			read_posicion++;
			bit_read_offset=0;
		}
		//p=&buffer[read_posicion];
		if(cantidad>0){
			fuente|=(buffer[read_posicion]<<bit_read_offset)>>(MAX_BITS-cantidad);
			bit_read_offset+=cantidad;
		}
	}
}
void bitFile::seek_r(char offset,unsigned posicion){
	read_posicion=posicion;
	bit_read_offset=offset;
}
void bitFile::seek_w(char offset,unsigned posicion){
	write_posicion=posicion;
	bit_write_offset=offset;
}
void bitFile::jump_bits_r(char offset){
	bit_read_offset+=offset;
	if(bit_read_offset>MAX_BITS){
		bit_read_offset=bit_read_offset%MAX_BITS;
		read_posicion+=offset/MAX_BITS;
	}
}
void bitFile::jump_bits_w(char offset){
	bit_write_offset+=offset;
	if(bit_write_offset>MAX_BITS){
		bit_write_offset=bit_write_offset%MAX_BITS;
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
	return bit_read_offset;
}
char bitFile::tell_bits_offset_w(){
	return bit_write_offset;
}
void bitFile::fill(unsigned fuente){
	unsigned * p=&buffer[write_posicion];
	if(write_posicion<tamanio){
		if(bit_write_offset<MAX_BITS){
			buffer[write_posicion]&=~(UNOS>>bit_write_offset);
			buffer[write_posicion]|=fuente>>bit_write_offset;
			fuente<<=(MAX_BITS-bit_write_offset);
		}
		write_posicion++;
		if(write_posicion<tamanio){
			p=&buffer[write_posicion];
			buffer[write_posicion]&=~(UNOS>>bit_write_offset);
			buffer[write_posicion]|=fuente;

		}
	}
}
void bitFile::setBuffer(unsigned*buffer,unsigned tamanio){
	this->tamanio=tamanio;
	this->buffer=buffer;
}
unsigned* bitFile::getBuffer(){
	return buffer;
}
