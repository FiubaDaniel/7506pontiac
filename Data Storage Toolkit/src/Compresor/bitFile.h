/*
 * BitContainer.h
 *
 *  Created on: 28/11/2009
 *  Author: paulo
 */
#ifndef BITFILE_H_
#define BITFILE_H_
#include <iostream>
#include <exception>
#include <cstddef>
#define MAXBITS 32
#define UNOS (~unsigned(0x0))
using namespace std;
void imprimir(unsigned num,char n=MAXBITS);
class bitFileEOFException: public std::exception{
public:
	bitFileEOFException() throw(){}
	bitFileEOFException(const exception&) throw(){}
	exception& operator= (const exception&) throw(){
		return *this;
	}
	virtual ~bitFileEOFException() throw(){}
	virtual const char* what() const throw(){
		return "Se trato de leer/escribir mas halla del fin del bitFile";
	}

};

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
	char bit_r;
	/*indice de los bits leidos respecto de <read_position>*/
	char bit_w;
public:
	static const char MAX_BITS=sizeof(unsigned)*8;
	bitFile();
	bitFile(unsigned*buffer,unsigned tamanio);
	virtual ~bitFile();
	/* Toma los <cantidad> bits de menos significativos de <fuente>
	 * y los graba en la posicion actual del <bitFile>
	 * incrementando la posicion de escritura.
	 * 0<=cantidad<=MAX_BITS
	 * en caso de que la <cantidad> supera los bits de espacio que quedan
	 * no se guarda nada.
	 */
	void write(unsigned fuente,char cantidad) throw (bitFileEOFException);
	/* Toma los <cantidad> bits siguiente de la posicion de lectura y los copia
	 * y los copia en los bits menos significativos de <destino>
	 * incrementando la posicion de escritura.
	 * 0<=cantidad<=MAX_BITS
	 * en caso de que la <cantidad> supera los bits de que puende ser leidos
	 * no se lee nada.
	 */
	void read(unsigned &destino,char cantidad) throw (bitFileEOFException);
	/* posiciona para la lectura en el unsigned <posicion>
	 * del <buffer> , a partir del bit <offset>
	 * 0<=offset<=MAX_BITS
	 */
	void seek_r(char offset,unsigned posicion);
	/* posiciona para la escritura en el unsigned <posicion>
	 * del <buffer> , a partir del bit <offset>
	 * 0<=offset<=MAX_BITS
	 */
	void seek_w(char offset,unsigned posicion);
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
	/*completa el unsigned donde se esta escribiendo con <fuente>
	 * si fuente desborda el unsigned donde se esta escribiendo
	 * se graban los bits de desborde en el siguiente unsigned, si existe un siguiente*/
	void fill(unsigned fuente);
	/* Seter y getter para el array donde se escriben los bits, el set no reposiciona
	 * el get devuelve una referencia al array usado, cualquier modificacion sobre este afecta
	 * el contenido del bitFile
	 */
	void setBuffer(unsigned*buffer,unsigned tamanio);
	unsigned* getBuffer();

};


#endif /* BITCONTAINER_H_ */
