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
public:
	EstrategiaCompresion();
	void compresion();
	void compresion(Almacenamiento*almacen,string archivoComprimido,unsigned tamanio);
	void descompresion(Almacenamiento*almacen,string archivoComprimido);
	void compresionArbol(BSharpTree* arbol,string archivoComprimido,unsigned tamanio_buffer_comprimido);
	void compresionHash();
	virtual ~EstrategiaCompresion();
};
#endif /* ESTRATEGIACOMPRESION_H_ */
