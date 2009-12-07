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
	void compresion(Almacenamiento*almacen,unsigned tamanio);
	bool descompresion(Almacenamiento*almacen);
	void compresionArbol(BSharpTree* arbol,string archivoComprimido,unsigned tamanio_buffer_comprimido);
	void descompresionArbol(BSharpTree*arbol,string archivoComprimido);
	void compresionIndice(string nombre_inidice,string archivoComprimido,unsigned tamanio_buffer_comprimido);
	void descompresionInsdice(string nombre_indice,string nombre_comprimido);
	void compresionHash();
	virtual ~EstrategiaCompresion();
};
#endif /* ESTRATEGIACOMPRESION_H_ */
