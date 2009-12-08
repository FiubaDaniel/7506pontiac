/*
 * EstrategiaCompresion.h
 *
 *  Created on: 03/12/2009
 *      Author: daniel
 */

#ifndef ESTRATEGIACOMPRESION_H_
#define ESTRATEGIACOMPRESION_H_
#include "../Almacenamiento/Almacenamiento.h"
#include "../ArbolBSharp/BSharpTree.h"
#include "Compresor.h"
#include <fstream>

#include <string>

class EstrategiaCompresion {
	std::ostream * salida;
public:
	EstrategiaCompresion();
	void compresion();
	void setSalida(std::ostream*salida);
	void compresion(Almacenamiento*almacen,unsigned tamanio);
	bool descompresion(Almacenamiento*almacen);
	bool compresionArbol(BSharpTree* arbol,string archivoComprimido,unsigned tamanio_buffer_comprimido);
	bool descompresionArbol(BSharpTree*arbol,string archivoComprimido);
	bool compresionHash(string nombreIndice,unsigned tamanio_buffer_comprimido);
	bool descompresionHash(string nombreIndice);
	virtual ~EstrategiaCompresion();
};
#endif /* ESTRATEGIACOMPRESION_H_ */
