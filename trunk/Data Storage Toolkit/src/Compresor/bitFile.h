/*
 * BitContainer.h
 *
 *  Created on: 28/11/2009
 *  Author: paulo
 */
#ifndef BITFILE_H_
#define BITFILE_H_

class bitFile {
	/*Array de unsigned donde se escriben/leen los bits*/
	unsigned* buffer;
	/*tamanio del <buffer>, en cantidad de unsigned's*/
	unsigned tamanio;
	/*indice del unsigned donde se esta escribiendo*/
	unsigned write_posicion;
	/*indice del unsigned donde se esta leyendo*/
	unsigned read_posicion;
	/*indice de los bits escritos respecto de <write_position>*/
	char bit_read_offset;
	/*indice de los bits leidos respecto de <read_position>*/
	char bit_write_offset;
public:
	static const char MAX_BITS=sizeof(unsigned)*8;
	bitFile(unsigned*buffer,unsigned tamanio);
	virtual ~bitFile();
	/* Toma los <cantidad> bits de menos significativos de <fuente>
	 * y los graba en la posicion actual del <bitFile>
	 * incrementando la posicion de escritura.
	 * 0<=cantidad<=MAX_BITS
	 */
	void write(unsigned fuente,char cantidad);
	/* Toma los <cantidad> bits siguiente de la posicion de lectura y los copia
	 * y los copia en los bits menos significativos de <destino>
	 * incrementando la posicion de escritura.
	 * 0<=cantidad<=MAX_BITS
	 */
	void read(unsigned &destino,char cantidad);
	/* posiciona para la lectura en el unsigned <posicion>
	 * del <buffer> , a partir del bit <offset>
	 * 0<=offset<=MAX_BITS
	 */
	void seek_r(char offset,unsigned posicion);
	/* posiciona para la escritura en el unsigned <posicion>
	 * del <buffer> , a partir del bit <offset>
	 * 0<=offset<=MAX_BITS
	 */
	void seeg_w(char offset,unsigned posicion);
	/*salta de la posicion actual de lectura <offset> bits*/
	void jump_bits_r(char offset);
	/*salta de la posicion actual de escritura <offset> bits*/
	void jump_bits_w(char offset);
	/*devuelve la posicion del unsigned donde se esta leyendo*/
	unsigned tell_unsigned_r();
	/*devuelve la posicion del unsigned donde se esta escribiendo*/
	unsigned tell_unsigned_write();
	/* tamanio del contenedor en unsigned*/
	unsigned size();
	/* posicion de lectura en bits respecto del unsigned donde se esta leyendo*/
	char tell_bits_offset_r();
	/* posicion de escritura en bits respecto del unsigned donde se esta leyendo*/
	char tell_bits_offset_w();
	/*TODO eliminar estos metodos*/
	void write_mas_significativos(unsigned fuente,char cantidad);
	void read_mas_significativos(unsigned &destino,char cantidad);
};

#endif /* BITCONTAINER_H_ */