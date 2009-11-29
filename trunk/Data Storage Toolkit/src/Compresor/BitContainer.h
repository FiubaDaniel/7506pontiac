/*
 * BitContainer.h
 *
 *  Created on: 28/11/2009
 *      Author: paulo
 */

#ifndef BITCONTAINER_H_
#define BITCONTAINER_H_

class BitContainer {
	static const char MAX_BITS=sizeof(unsigned)*8;
	unsigned* buffer;
	unsigned ultimo;
	unsigned write_posicion;
	unsigned read_posicion;
	char bit_read_offset;
	char bit_write_offset;
public:
	BitContainer(unsigned*buffer,unsigned tamanio);
	void write_menos_significativos(unsigned fuente,char cantidad);
	void write_mas_significativos(unsigned fuente,char cantidad);
	void read_menos_significativos(unsigned &destino,char cantidad);
	void read_mas_significativos(unsigned &destino,char cantidad);
	void seek_r(char offset,unsigned posicion);
	void seeg_w(char offset,unsigned posicion);
	void jump_r(char offset);
	void jump_w(char offset);
	unsigned posicion_read();
	unsigned posicion_write();
	/* tamanio del contenedor en bytes*/
	unsigned size();
	/* cantidad de bits libres en el ultimo unsigned usado*/
	char read_offset();
	char write_offset();
	virtual ~BitContainer();
};

#endif /* BITCONTAINER_H_ */
