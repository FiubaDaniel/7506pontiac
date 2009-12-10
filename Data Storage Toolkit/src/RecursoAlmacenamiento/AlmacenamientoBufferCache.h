/*
 * AlmacenamientoBufferCache.h
 *
 *  Created on: 10/12/2009
 *      Author: paulo
 */

#ifndef ALMACENAMIENTOBUFFERCACHE_H_
#define ALMACENAMIENTOBUFFERCACHE_H_
#include "../Almacenamiento/Almacenamiento.h"
#include "BufferCache.hpp"

class AlmacenamientoBufferCache :public Almacenamiento {
	Referencia posicion;
	Referencia tamanio_metadata;
	int nro_bloque;
	unsigned tamanio_bloque;
	Almacenamiento* almacen;
	BufferCache* buffer;
	void switchEstrategia();
public:
	AlmacenamientoBufferCache(Almacenamiento*archivo);
	/*utilizado por la estrategiaAlmacenamiento*/
	bool abrir(const char* nombre);
	void crear(const char *nombre);
	void escribir(const void* bytes,Referencia cantidad);
	void escribir(const void* unByte);
	void leer(void* bytes,Referencia cantidad);
	void leer(void* unBytes);
	void posicionar(Referencia posicion);
	void posicionarAlfinal();
	Referencia posicionActual();
	bool bien();
	bool fin();
	/*Limpia los flags de fin de archivo y error*/
	void clear();
	void cerrar();
	Almacenamiento*clonar();
	virtual ~AlmacenamientoBufferCache();
};

#endif /* ALMACENAMIENTOBUFFERCACHE_H_ */
