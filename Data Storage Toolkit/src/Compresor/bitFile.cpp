/*
 * BitContainer.cpp
 *
 *  Created on: 28/11/2009
 *      Author: paulo
 */

#include "bitFile.h"

bitFile::bitFile(unsigned*buffer,unsigned tamanio){
	this->buffer=buffer;
	ultimo=tamanio-1;
	write_posicion=0;
	bit_write_offset=0;
	read_posicion=0;
	bit_read_offset=0;
};

bitFile::~bitFile() {}
void bitFile::write_menos_significativos(unsigned fuente,char cantidad){
	int bits_restantes=(ultimo-write_posicion+1)*MAX_BITS-bit_write_offset;
	if(bits_restantes > cantidad){
		fuente<<=MAX_BITS-cantidad;
		bits_restantes=MAX_BITS-bit_write_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				buffer[write_posicion]&=~(0xFFFFFFFF>>bit_write_offset);
				buffer[write_posicion]|=fuente>>bit_write_offset;
				fuente<<=bits_restantes;
				cantidad-=bits_restantes;
			}
			write_posicion++;
			bit_write_offset=0;
		}
		if(cantidad>0){
			unsigned aux=(0xFFFFFFFF<<(MAX_BITS-cantidad))>>bit_write_offset;
			buffer[write_posicion]&=~aux;
			buffer[write_posicion]|=fuente>>bit_write_offset;
			bit_write_offset+=cantidad;
		}
	}
}
void bitFile::write_mas_significativos(unsigned fuente,char cantidad){
	int bits_restantes=(ultimo-write_posicion+1)*MAX_BITS-bit_write_offset;
	if(bits_restantes > cantidad){
		bits_restantes=MAX_BITS-bit_write_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				buffer[write_posicion]|=fuente>>bit_write_offset;
				fuente<<=bits_restantes;
				cantidad-=bits_restantes;
			}
			write_posicion++;
			bit_write_offset=0;
		}
		if(cantidad>0){
			buffer[write_posicion]|=fuente>>bit_write_offset;
			bit_write_offset+=cantidad;
		}
	}
}
void bitFile::read_menos_significativos(unsigned &fuente,char cantidad){
	int bits_restantes=(ultimo-read_posicion+1)*MAX_BITS-bit_read_offset;
	if(bits_restantes>cantidad){
		bits_restantes=MAX_BITS-bit_read_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				fuente|=(buffer[read_posicion]<<bit_read_offset)>>(MAX_BITS-cantidad);
				cantidad-=bits_restantes;
			}
			read_posicion++;
			bit_read_offset=0;
		}
		if(cantidad>0){
			fuente|=(buffer[read_posicion]<<bit_read_offset)>>(MAX_BITS-cantidad);
			bit_read_offset+=cantidad;
		}
	}
}
void bitFile::read_mas_significativos(unsigned &fuente,char cantidad){
	int bits_restantes=(ultimo-read_posicion+1)*MAX_BITS-bit_read_offset;
	if(bits_restantes>cantidad){
		bits_restantes=MAX_BITS-bit_read_offset;
		if(bits_restantes<cantidad){
			if(bits_restantes>0){
				fuente|=buffer[read_posicion]<<bit_read_offset;
				cantidad-=bits_restantes;
			}
			bit_read_offset=0;
			read_posicion++;
		}else{
			bits_restantes=0;
		}
		if(cantidad>0){
			/* cambia la semantica de "bits_restantes" ahora significa
			 * la cantidad ya escrita
			 */
			fuente|=(buffer[read_posicion]<<bit_read_offset)>>bits_restantes;
			bit_read_offset+=cantidad;
		}
	}
}
void bitFile::seek_r(char offset,unsigned posicion){
	read_posicion=posicion;
	bit_read_offset=offset;
}
void bitFile::seeg_w(char offset,unsigned posicion){
	write_posicion=posicion;
	bit_write_offset=offset;
}
void bitFile::jump_r(char offset){
	bit_read_offset+=offset;
	if(bit_read_offset>MAX_BITS){
		bit_read_offset=bit_read_offset%MAX_BITS;
		read_posicion+=offset/MAX_BITS;
	}
}
void bitFile::jump_w(char offset){
	bit_write_offset+=offset;
	if(bit_write_offset>MAX_BITS){
		bit_write_offset=bit_write_offset%MAX_BITS;
		write_posicion+=offset/MAX_BITS;
	}
}
unsigned bitFile::posicion_read(){
	return read_posicion;
}
unsigned bitFile::posicion_write(){
	return write_posicion;
}
/*tamanio del contenedor en bytes*/
unsigned bitFile::size(){
	return ultimo+1;
}
/*cantidad de bits libres en el ultimo unsigned usado*/
char bitFile::read_offset(){
	return bit_read_offset;
}
char bitFile::write_offset(){
	return bit_write_offset;
}
/*int bits_extra=(ultimo-write_posicion)*MAX_BITS;
if(bits_extra+bits_libres<cantidad){
	if(bits_libres<cantidad){
		if(bits_libres>0){
			cantidad-=bits_libres;
			buffer[write_posicion]<<=bits_libres;
			buffer[write_posicion]|=fuente>>cantidad;
		}
		write_posicion++;
		bits_libres=MAX_BITS;
	}
	if(cantidad>0){
		buffer[write_posicion]<<=cantidad;
		buffer[write_posicion]|=fuente;
		bits_libres-=cantidad;
	}
}*/
/*	int bits_extra=(ultimo-write_posicion)*MAX_BITS;
if(bits_extra+bits_libres<cantidad){
	if(bits_libres<cantidad){
		if(bits_libres>0){
			buffer[write_posicion]<<=bits_libres;
			buffer[write_posicion]|=fuente>>(MAX_BITS-bits_libres);
			cantidad-=bits_libres;
		}
		write_posicion++;
		bits_libres=MAX_BITS;
	}
	if(cantidad>0){
		buffer[write_posicion]<<=cantidad;
		buffer[write_posicion]|=fuente>>(MAX_BITS-cantidad);
		bits_libres-=cantidad;
	}
}*/
/*		buffer[write_posicion]|=fuente>>bit_write_offset;
	cantidad=cantidad-(MAX_BITS-bit_write_offset);
	if(cantidad>=0){
		write_posicion++;
		if(cantidad>0){
			fuente<<=(MAX_BITS-bit_write_offset);
			buffer[write_posicion]|=fuente;
		}
		bit_write_offset=cantidad;
	}else{
		cantidad*=-1;
		bit_write_offset+=cantidad;
	}*/
/*		buffer[write_posicion]|=fuente>>bit_write_offset;
		cantidad=cantidad-(MAX_BITS-bit_write_offset);
		if(cantidad>=0){
			write_posicion++;
			if(cantidad>0){
				fuente<<=(MAX_BITS-bit_write_offset);
				buffer[write_posicion]|=fuente;
			}
			bit_write_offset=cantidad;
		}else{
			cantidad*=-1;
			bit_write_offset+=cantidad;
		}*/
